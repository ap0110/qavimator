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

#include <QScopedPointer>
#include <QSharedPointer>

#include "newanimation.h"
#include "joint.h"
#include "nullable.h"

#include "bvhparser.h"

BvhFrame::BvhFrame(const int& frameNumber)
  : m_frameNumber(frameNumber)
{
}

BvhFrame::~BvhFrame()
{
}

const QVector3D& BvhFrame::position() const
{
  return m_position;
}

const QVector3D& BvhFrame::rotation() const
{
  return m_rotation;
}

void BvhFrame::setPosition(const QVector3D& position)
{
  m_position = position;
}

void BvhFrame::setRotation(const QVector3D& rotation)
{
  m_rotation = rotation;
}

KeyframeData BvhFrame::toKeyframeData() const
{
  return KeyframeData(m_frameNumber, m_position.x(), m_position.y(), m_position.z(),
                      m_rotation.x(), m_rotation.y(), m_rotation.z());
}

BvhJoint::BvhJoint(const QString& name)
  : m_name(name)
{
}

BvhJoint::~BvhJoint()
{
}

const QList<QSharedPointer<BvhJoint>> BvhJoint::children() const
{
  return m_children;
}

void BvhJoint::addChild(QSharedPointer<BvhJoint> child)
{
  m_children.append(child);
}

const QVector3D& BvhJoint::head() const
{
  return m_head;
}

const QVector3D& BvhJoint::tail(int index) const
{
  return m_tails.at(index);
}

void BvhJoint::setHead(const QVector3D& head)
{
  m_head = head;
}

void BvhJoint::addTail(const QVector3D& tail)
{
  m_tails.append(tail);
}

const QList<Channel> BvhJoint::channels() const
{
  return m_channels;
}

void BvhJoint::addChannel(Channel channel)
{
  m_channels.append(channel);
}

void BvhJoint::addFrame(const BvhFrame& frame)
{
  m_frames.append(frame);
}

QSharedPointer<Joint> BvhJoint::toJoint()
{
  QSharedPointer<Joint> joint(new Joint(nullptr, m_name));

  QMap<int, KeyframeData> keyframes;
  for (int i = 0; i < m_frames.size(); i++)
  {
    keyframes.insert(i, m_frames.at(i).toKeyframeData());
  }

  QList<QSharedPointer<Joint>> children;
  for (auto iter = m_children.cbegin(); iter != m_children.cend(); iter++)
  {
    children.append((*iter)->toJoint());
  }

  joint->setHead(m_head);
  joint->setTails(m_tails);
  joint->setKeyframes(keyframes);
  joint->setChildren(children);

  return joint;
}

BvhParser::BvhParser(const QString& bvhData)
  : m_bvhTokens(bvhData.simplified().split(' ')),
    m_bvhTokensIterator(m_bvhTokens.constBegin())
{
}

BvhParser::~BvhParser()
{
}

QSharedPointer<NewAnimation> BvhParser::parse()
{
  QSharedPointer<NewAnimation> animation;

  QSharedPointer<Joint> rootJoint;
  float frameTime = parseBvhData(rootJoint);

  if (frameTime > 0.0f)
  {
    animation.reset(new NewAnimation(nullptr,
                                     rootJoint,
                                     rootJoint->numKeyframes() - 1,
                                     frameTime));
  }

  return animation;
}

float BvhParser::parseBvhData(QSharedPointer<Joint>& rootJoint)
{
  float frameTime = -1.0f;

  bool isHierarchyParsed = false;
  bool isMotionParsed = false;
  QSharedPointer<BvhJoint> rootBvhJoint;

  while (hasNext())
  {
    next();
    if (!isHierarchyParsed && isEqualIgnoreCase(token(), "HIERARCHY"))
    {
      isHierarchyParsed = true;
      parseHierarchy(rootBvhJoint);
    }
    else if ((!rootBvhJoint.isNull())
             && !isMotionParsed
             && isEqualIgnoreCase(token(), "MOTION"))
    {
      isMotionParsed = true;
      frameTime = parseMotion(rootBvhJoint);
    }
    else if (isHierarchyParsed && isMotionParsed)
    {
      // TODO Check for, and parse, any legacy AVM data
    }
    else
    {
      // TODO Error: Should have been HIERARCHY or MOTION or AVM data
      break;
    }
  }

  if (!rootBvhJoint.isNull())
  {
    rootJoint = rootBvhJoint->toJoint();
  }
  return frameTime;
}

void BvhParser::parseHierarchy(QSharedPointer<BvhJoint>& rootJoint)
{
  // Limit to one hierarchy for now
  bool isRootParsed = false;

  while (hasNext())
  {
    next();
    if (!isRootParsed && isEqualIgnoreCase(token(), "ROOT"))
    {
      isRootParsed = true;
      rootJoint.reset(new BvhJoint(nextToken()));
      expectNextToken("{");
      parseJoint(rootJoint);
    }
    else if (isEqualIgnoreCase(token(), "MOTION"))
    {
      previous();
      break;
    }
    else
    {
      // TODO Error: Should have been ROOT or MOTION
    }
  }
}

float BvhParser::parseMotion(const QSharedPointer<BvhJoint>& rootJoint)
{
  int frameCount = 0;
  float frameTime = 0.0f;

  expectNextToken("Frames:");
  frameCount = parseInt(nextToken());

  expectNextToken("Frame");
  expectNextToken("Time:");
  frameTime = parseFloat(nextToken());

  // Parse all the frames
  for (int i = 0; i < frameCount; i++)
  {
    parseFrame(rootJoint, i);
  }

  return frameTime;
}

void BvhParser::parseJoint(const QSharedPointer<BvhJoint>& joint)
{
  bool hasOffset = false;
  bool hasChannels = false;
  bool hasEndSite = false;

  while (hasNext())
  {
    next();

    // TODO Replace if-else chain with string switch
    //  or something that does better than a linear search
    if (!hasOffset && isEqualIgnoreCase(token(), "OFFSET"))
    {
      hasOffset = true;

      float x = parseFloat(nextToken());
      float y = parseFloat(nextToken());
      float z = parseFloat(nextToken());
      joint->setHead(QVector3D(x, y, z));
    }
    else if (!hasChannels && isEqualIgnoreCase(token(), "CHANNELS"))
    {
      hasChannels = true;
      parseChannels(joint);
    }
    else if (isEqualIgnoreCase(token(), "JOINT"))
    {
      QSharedPointer<BvhJoint> childJoint(new BvhJoint(nextToken()));

      expectNextToken("{");
      parseJoint(childJoint);
      joint->addChild(childJoint);
      // Head of the child will be tail of the parent
      joint->addTail(childJoint->head());
    }
    else if (isEqualIgnoreCase(token(), "}"))
    {
      // End of Joint data, so return
      return;
    }
    else if (!hasEndSite && isEqualIgnoreCase(token(), "END"))
    {
      hasEndSite = true;

      expectNextToken("SITE");
      expectNextToken("{");
      expectNextToken("OFFSET");


      float x = parseFloat(nextToken());
      float y = parseFloat(nextToken());
      float z = parseFloat(nextToken());
      joint->addTail(QVector3D(x, y, z));

      expectNextToken("}");
    }
    else
    {
      // TODO Handle Error: Unexpected token
    }
  }
}

void BvhParser::parseChannels(const QSharedPointer<BvhJoint>& joint)
{
  int numChannels = parseInt(nextToken());

  for (; numChannels > 0; numChannels--)
  {
    next();
    if (token().endsWith("POSITION", Qt::CaseInsensitive))
    {
      switch (token().at(0).toUpper().unicode())
      {
        case 'X':
          joint->addChannel(Channel::POSITION_X);
          break;
        case 'Y':
          joint->addChannel(Channel::POSITION_Y);
          break;
        case 'Z':
          joint->addChannel(Channel::POSITION_Z);
          break;
      }
    }
    else if (token().endsWith("ROTATION", Qt::CaseInsensitive))
    {
      switch (token().at(0).toUpper().unicode())
      {
        case 'X':
          joint->addChannel(Channel::ROTATION_X);
          break;
        case 'Y':
          joint->addChannel(Channel::ROTATION_Y);
          break;
        case 'Z':
          joint->addChannel(Channel::ROTATION_Z);
          break;
      }
    }
    else
    {
      // TODO Handle Error: Unexpected token
    }
  }
}

void BvhParser::parseFrame(const QSharedPointer<BvhJoint>& joint, int frameNumber)
{
  Nullable<float> xPosition;
  Nullable<float> yPosition;
  Nullable<float> zPosition;
  Nullable<float> xRotation;
  Nullable<float> yRotation;
  Nullable<float> zRotation;

  for (auto iter = joint->channels().constBegin(); iter != joint->channels().constEnd(); iter++)
  {
    switch (*iter)
    {
      case Channel::POSITION_X:
        xPosition = parseFloat(nextToken());
        break;
      case Channel::POSITION_Y:
        yPosition = parseFloat(nextToken());
        break;
      case Channel::POSITION_Z:
        zPosition = parseFloat(nextToken());
        break;
      case Channel::ROTATION_X:
        xRotation = parseFloat(nextToken());
        break;
      case Channel::ROTATION_Y:
        yRotation = parseFloat(nextToken());
        break;
      case Channel::ROTATION_Z:
        zRotation = parseFloat(nextToken());
        break;
      default:
        // TODO Handle error
        break;
    }
  }

  BvhFrame frame(frameNumber);
  if (xPosition.hasValue() && yPosition.hasValue() && zPosition.hasValue())
  {
    frame.setPosition(QVector3D(xPosition.value(), yPosition.value(), zPosition.value()));
  }
  if (xRotation.hasValue() && yRotation.hasValue() && zRotation.hasValue())
  {
    frame.setRotation(QVector3D(xRotation.value(), yRotation.value(), zRotation.value()));
  }

  joint->addFrame(frame);

  // Recurse through the BvhJoint hierarchy
  for (auto iter = joint->children().constBegin(); iter != joint->children().constEnd(); iter++)
  {
    parseFrame(*iter, frameNumber);
  }
}

bool BvhParser::hasNext()
{
  return m_bvhTokensIterator != m_bvhTokens.constEnd();
}

void BvhParser::next()
{
  if (m_bvhTokensIterator != m_bvhTokens.constEnd())
  {
    m_bvhTokensIterator++;
  }
  else
  {
    // TODO Handle Error: Unexpected End of File
  }
}

bool BvhParser::hasPrevious()
{
  return m_bvhTokensIterator != m_bvhTokens.constBegin();
}

void BvhParser::previous()
{
  if (m_bvhTokensIterator != m_bvhTokens.constBegin())
  {
    m_bvhTokensIterator--;
  }
  else
  {
    // TODO Handle Error: Unexpected beginning of file
  }
}

const QString& BvhParser::token()
{
  return *m_bvhTokensIterator;
}

const QString& BvhParser::nextToken()
{
  next();
  return *m_bvhTokensIterator;
}

void BvhParser::expectToken(const QString& expected)
{
  if (!isEqualIgnoreCase(token(), expected))
  {
    // TODO Handle Error: Unexpected token
  }
}

void BvhParser::expectNextToken(const QString& expected)
{
  if (!isEqualIgnoreCase(nextToken(), expected))
  {
    // TODO Handle Error: Unexpected token
  }
}

bool BvhParser::isEqualIgnoreCase(const QString& lhs, const QString& rhs) const
{
  return lhs.compare(rhs, Qt::CaseInsensitive) == 0;
}

int BvhParser::parseInt(const QString& integerString) const
{
  bool ok;
  int result = integerString.toInt(&ok);
  if (!ok)
  {
    // TODO Handle error
  }
  return result;
}

float BvhParser::parseFloat(const QString& floatString) const
{
  bool ok;
  float result = floatString.toFloat(&ok);
  if (!ok)
  {
    // TODO Handle error
  }
  return result;
}
