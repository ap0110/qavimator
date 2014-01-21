/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree                                          *
 *   Zi Ree @ SecondLife                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "animation.h"
#include "joint.h"

#include "bvhparser.h"

BvhFrame::BvhFrame()
{
}

BvhFrame::~BvhFrame()
{
}

const QVector3D* BvhFrame::position() const
{
  return m_position.data();
}

const QVector3D* BvhFrame::rotation() const
{
  return m_rotation.data();
}

void BvhFrame::setPosition(QVector3D* position)
{
  m_position.reset(position);
}

void BvhFrame::setRotation(QVector3D* rotation)
{
  m_rotation.reset(rotation);
}

BvhJoint::BvhJoint(const QString& name)
{
  m_name = name;
}

BvhJoint::~BvhJoint()
{
}

const QList<BvhJoint*> BvhJoint::children() const
{
  return m_children;
}

void BvhJoint::addChild(BvhJoint* child)
{
  m_children.append(child);
}

const QVector3D* BvhJoint::head() const
{
  return m_head.data();
}

const QList<QVector3D*> BvhJoint::tail(int index) const
{
  return m_tailList;
}

void BvhJoint::setHead(QVector3D* head)
{
  m_head.reset(head);
}

void BvhJoint::addTail(QVector3D* tail)
{
  m_tailList.append(tail);
}

const QList<Channel>& BvhJoint::channels() const
{
  return m_channels;
}

void BvhJoint::addChannel(Channel channel)
{
  m_channels.append(channel);
}

void BvhJoint::setMaxFrameCount(int count)
{
  m_maxFrameCount = count;
  m_frames.reserve(count);
}

void BvhJoint::addFrame(BvhFrame* frame)
{
  m_frames.append(frame);
}

Joint* BvhJoint::toJoint()
{

}

BvhParser::BvhParser(const QString& bvhData)
{
  m_tokenList = bvhData.simplified().split(' ');
  m_parser.reset(new QStringListIterator(m_tokenList));
}

BvhParser::~BvhParser()
{
}

Animation* BvhParser::parseBvhData()
{
  bool isHierarchyParsed = false;
  bool isMotionParsed = false;
  BvhJoint* rootJoint = NULL;

  while (hasNext())
  {
    next();
    if (!isHierarchyParsed && isEqual(token(), "HIERARCHY"))
    {
      isHierarchyParsed = true;
      rootJoint = parseHierarchy();
    }
    else if (!isMotionParsed && isEqual(token(), "MOTION"))
    {
      isMotionParsed = true;
      parseMotion(rootJoint);
    }
    else if (isHierarchyParsed && isMotionParsed)
    {
      // TODO Check for, and parse, any legacy AVM data
    }
    else
    {
      // TODO Error: Should have been HIERARCHY or MOTION or AVM data
      return NULL;
    }
  }
  return NULL;
}

BvhJoint* BvhParser::parseHierarchy()
{
  // Limit to one hierarchy for now
  bool isRootParsed = false;

  BvhJoint* rootJoint = NULL;
  while (hasNext())
  {
    next();
    if (!isRootParsed && isEqual(token(), "ROOT"))
    {
      isRootParsed = true;
      rootJoint = new BvhJoint(nextToken());
      expectNextToken("{");
      parseJoint(rootJoint);
    }
    else if (isEqual(token(), "MOTION"))
    {
      previous();
      break;
    }
    else
    {
      // TODO Error: Should have been ROOT or MOTION
    }
  }
  return rootJoint;
}

void BvhParser::parseMotion(BvhJoint* rootJoint)
{
  bool* ok;
  int frameCount = 0;
  float frameTime = 0.0f;

  expectNextToken("Frames:");
  frameCount = nextToken().toInt(ok);
  if (!ok)
  {
    // TODO Handle error
  }

  expectNextToken("Frame");
  expectNextToken("Time:");
  frameTime = nextToken().toFloat(ok);
  if (!ok)
  {
    // TODO Handle error
  }

  // Parse all the frames
  for (int i = 0; i < frameCount; i++)
  {
    parseFrame(rootJoint);
  }
}

void BvhParser::parseJoint(BvhJoint* joint)
{
  bool hasOffset = false;
  bool hasChannels = false;
  bool hasEndSite = false;

  while (hasNext())
  {
    next();

    // TODO Replace if-else chain with string switch
    //  or something that does better than a linear search
    if (!hasOffset && isEqual(token(), "OFFSET"))
    {
      hasOffset = true;
      joint->setHead(parseOffset());
    }
    else if (!hasChannels && isEqual(token(), "CHANNELS"))
    {
      hasChannels = true;
      parseChannels(joint);
    }
    else if (isEqual(token(), "JOINT"))
    {
      BvhJoint* childJoint = new BvhJoint(nextToken());

      expectNextToken("{");
      parseJoint(childJoint);
      joint->addChild(childJoint);
      // Head of the child will be tail of the parent
      const QVector3D tail = *childJoint->head();
      joint->addTail(new QVector3D(tail.x(), tail.y(), tail.z()));
    }
    else if (isEqual(token(), "}"))
    {
      // End of Joint data, so return
      return;
    }
    else if (!hasEndSite && isEqual(token(), "END"))
    {
      hasEndSite = true;

      expectNextToken("SITE");
      expectNextToken("{");
      expectNextToken("OFFSET");

      joint->addTail(parseOffset());

      expectNextToken("}");
    }
    else
    {
      // TODO Handle Error: Unexpected token
    }
  }
}

QVector3D* BvhParser::parseOffset()
{
  bool* ok = new bool;

  float x = nextToken().toFloat(ok);
  if (!ok)
  {
    // TODO Handle error
  }

  float y = nextToken().toFloat(ok);
  if (!ok)
  {
    // TODO Handle error
  }

  float z = nextToken().toFloat(ok);
  if (!ok)
  {
    //  TODO Handle error
  }

  return new QVector3D(x, y, z);
}

void BvhParser::parseChannels(BvhJoint* joint)
{
  bool* ok;
  int numChannels = nextToken().toInt(ok);
  if (!ok)
  {
    // TODO Handle Error: Unexpected token
  }

  for (; numChannels > 0; numChannels--)
  {
    next();
    if (token().endsWith("POSITION", Qt::CaseInsensitive))
    {
      switch (token().at(0).toUpper().unicode())
      {
        case 'X':
          joint->addChannel(POSITION_X);
          break;
        case 'Y':
          joint->addChannel(POSITION_Y);
          break;
        case 'Z':
          joint->addChannel(POSITION_Z);
          break;
      }
    }
    else if (token().endsWith("ROTATION", Qt::CaseInsensitive))
    {
      switch (token().at(0).toUpper().unicode())
      {
        case 'X':
          joint->addChannel(ROTATION_X);
          break;
        case 'Y':
          joint->addChannel(ROTATION_Y);
          break;
        case 'Z':
          joint->addChannel(ROTATION_Z);
          break;
      }
    }
    else
    {
      // TODO Handle Error: Unexpected token
    }
  }
}

void BvhParser::parseFrame(BvhJoint* joint)
{
  bool* ok;

  QScopedPointer<float> positionX;
  QScopedPointer<float> positionY;
  QScopedPointer<float> positionZ;
  QScopedPointer<float> rotationX;
  QScopedPointer<float> rotationY;
  QScopedPointer<float> rotationZ;

  for (QList<Channel>::const_iterator iter = joint->channels().constBegin();
       iter != joint->channels().constEnd();
       iter++)
  {
    switch (*iter)
    {
      case POSITION_X:
        positionX.reset(new float(nextToken().toFloat(ok)));
        break;
      case POSITION_Y:
        positionY.reset(new float(nextToken().toFloat(ok)));
        break;
      case POSITION_Z:
        positionZ.reset(new float(nextToken().toFloat(ok)));
        break;
      case ROTATION_X:
        rotationX.reset(new float(nextToken().toFloat(ok)));
        break;
      case ROTATION_Y:
        rotationY.reset(new float(nextToken().toFloat(ok)));
        break;
      case ROTATION_Z:
        rotationZ.reset(new float(nextToken().toFloat(ok)));
        break;
      default:
        // TODO Handle error
        break;
    }
    if (!ok)
    {
      // TODO Handle error
    }
  }

  BvhFrame* frame = new BvhFrame();
  if (!positionX.isNull())
  {
    frame->setPosition(new QVector3D(*positionX, *positionY, *positionZ));
  }
  if (!rotationX.isNull())
  {
    frame->setRotation(new QVector3D(*rotationX, *rotationY, *rotationZ));
  }

  joint->addFrame(frame);

  // Recurse through the BvhJoint hierarchy
  for (QList<BvhJoint*>::const_iterator iter = joint->children().constBegin();
       iter != joint->children().constEnd();
       iter++)
  {
    parseFrame(*iter);
  }
}

bool BvhParser::hasNext()
{
  return m_parser->hasNext();
}

void BvhParser::next()
{
  if (m_parser->hasNext())
  {
    m_token = m_parser->next();
  }
  else
  {
    // TODO Handle Error: Unexpected End of File
    m_token = QString();
  }
}

bool BvhParser::hasPrevious()
{
  return m_parser->hasPrevious();
}

void BvhParser::previous()
{
  if (m_parser->hasPrevious())
  {
    m_token = m_parser->previous();
  }
  else
  {
    // TODO Handle Error: Unexpected beginning of file
    m_token = QString();
  }
}

const QString& BvhParser::token()
{
  return m_token;
}

const QString&BvhParser::nextToken()
{
  next();
  return m_token;
}

void BvhParser::expectToken(const QString& expected)
{
  if (!isEqual(token(), expected))
  {
    // TODO Handle Error: Unexpected token
  }
}

void BvhParser::expectNextToken(const QString& expected)
{
  if (!isEqual(nextToken(), expected))
  {
    // TODO Handle Error: Unexpected token
  }
}

bool BvhParser::isEqual(const QString& lhs, const QString& rhs) const
{
  return lhs.compare(rhs, Qt::CaseInsensitive) == 0;
}
