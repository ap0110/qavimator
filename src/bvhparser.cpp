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

BvhJoint::BvhJoint(const QString& name)
{
  m_name = name;
  m_hasPosition = false;
  m_rotationOrder = ORDER_NONE;
}

BvhJoint::~BvhJoint()
{
}

void BvhJoint::addChild(BvhJoint* child)
{
  m_children.append(child);
}

const QVector3D* BvhJoint::head() const
{
  return m_head;
}

const QList<QVector3D*> BvhJoint::tail(int index) const
{
  return m_tailList;
}

void BvhJoint::setHead(QVector3D* head)
{
  m_head = head;
}

void BvhJoint::addTail(QVector3D* tail)
{
  m_tailList.append(tail);
}

const bool& BvhJoint::hasPosition() const
{
  return m_hasPosition;
}

const AxisOrder& BvhJoint::rotationOrder() const
{
  return m_rotationOrder;
}

void BvhJoint::setHasPosition(bool hasPosition)
{
  m_hasPosition = hasPosition;
}

void BvhJoint::setRotationOrder(AxisOrder rotationOrder)
{
  m_rotationOrder = rotationOrder;
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
  while (hasNext())
  {
    next();
    if (!isHierarchyParsed && isEqual(token(), "HIERARCHY"))
    {
      isHierarchyParsed = true;
      BvhJoint* rootJoint = parseHierarchy();
    }
    else if (!isMotionParsed && isEqual(token(), "MOTION"))
    {
      isMotionParsed = true;
      // TODO Get motion data
    }
    else
    {
      // TODO Error: Should have been HIERARCHY or MOTION
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
  QList<char> positionOrder;
  QList<char> rotationOrder;

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
      positionOrder.append(token().at(0).toUpper().unicode());
    }
    else if (token().endsWith("ROTATION", Qt::CaseInsensitive))
    {
      rotationOrder.append(token().at(0).toUpper().unicode());
    }
    else
    {
      // TODO Handle Error: Unexpected token
    }
  }

  if (!positionOrder.isEmpty())
  {
    if (positionOrder.length() != 3)
    {
      // TODO Handle error: unexpected token
    }
    // Currently, enforce XYZ order for position
    if (positionOrder.at(0) != 'X'
        || positionOrder.at(1) != 'Y'
        || positionOrder.at(2) != 'Z')
    {
      // TODO Handle error: position order must be XYZ
    }

    joint->setHasPosition(true);
  }
  if (!rotationOrder.isEmpty())
  {
    if (rotationOrder.length() != 3)
    {
      // TODO Handle error: unexpected token
    }

    switch (rotationOrder.at(0))
    {
      case 'X':
        switch (rotationOrder.at(1))
        {
          // XYZ
          case 'Y':
            if (rotationOrder.at(2) == 'Z')
            {
              joint->setRotationOrder(ORDER_XYZ);
            }
            else
            {
              // TODO Handle error: unexpected token
            }
            break;
          // XZY
          case 'Z':
            if (rotationOrder.at(2) == 'Y')
            {
              joint->setRotationOrder(ORDER_XZY);
            }
            else
            {
              // TODO Handle error: unexpected token
            }
            break;
          default:
            // TODO Handle error: unexpected token
            break;
        }
        break;
      case 'Y':
        switch (rotationOrder.at(1))
        {
          // YXZ
          case 'X':
            if (rotationOrder.at(2) == 'Z')
            {
              joint->setRotationOrder(ORDER_YXZ);
            }
            else
            {
              // TODO Handle error: unexpected token
            }
            break;
          // YZX
          case 'Z':
            if (rotationOrder.at(2) == 'X')
            {
              joint->setRotationOrder(ORDER_YZX);
            }
            else
            {
              // TODO Handle error: unexpected token
            }
            break;
          default:
            // TODO Handle error: unexpected token
            break;
        }
        break;
      case 'Z':
        switch (rotationOrder.at(1))
        {
          // ZXY
          case 'X':
            if (rotationOrder.at(2) == 'Y')
            {
              joint->setRotationOrder(ORDER_ZXY);
            }
            else
            {
              // TODO Handle error: unexpected token
            }
            break;
          // ZYX
          case 'Y':
            if (rotationOrder.at(2) == 'X')
            {
              joint->setRotationOrder(ORDER_ZYX);
            }
            else
            {
              // TODO Handle error: unexpected token
            }
            break;
          default:
            // TODO Handle error: unexpected token
            break;
        }
        break;
      default:
        // TODO Handle error: unexpected token
        break;
    }
  }
  else
  {
    // TODO Handle error: unexpected token
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
