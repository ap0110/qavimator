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
    if (!isHierarchyParsed && token().compare("HIERARCHY", Qt::CaseInsensitive) == 0)
    {
      isHierarchyParsed = true;
      parseHierarchy();
    }
    else if (!isMotionParsed && token().compare("MOTION", Qt::CaseInsensitive) == 0)
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

Joint* BvhParser::parseHierarchy()
{
  // Limit to one hierarchy for now
  bool isRootParsed = false;

  Joint* result = NULL;
  while (hasNext())
  {
    next();
    if (!isRootParsed && token().compare("ROOT", Qt::CaseInsensitive) == 0)
    {
      isRootParsed = true;
      next();
      result = new Joint(token());
      next();
      if (token() == "{")
      {
        QVector3D* tail = parseJoint(result);
        result->setTail(tail);
      }
      else
      {
        // TODO Handle error: Empty ROOT
        return NULL;
      }
    }
    else if (token().compare("MOTION", Qt::CaseInsensitive) == 0)
    {
      previous();
      break;
    }
    else
    {
      // TODO Error: Should have been ROOT or MOTION
    }
  }
  return result;
}

QVector3D* BvhParser::parseJoint(Joint* joint)
{
  bool hasOffset = false;
  bool hasChannels = false;

  while (hasNext())
  {
    next();

    // TODO Replace if-else chain with string switch
    //  or something that does better than a linear search
    if (token().compare("OFFSET", Qt::CaseInsensitive) == 0)
    {

    }
    else if (token().compare("CHANNELS", Qt::CaseInsensitive) == 0)
    {

    }
    else if (token().compare("JOINT", Qt::CaseInsensitive) == 0)
    {

    }
    else if (token().compare("END", Qt::CaseInsensitive) == 0)
    {
      // TODO Ensure next token is "SITE"
    }
    else
    {

    }
  }
  return NULL;
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
    m_token = QString();
  }
}

const QString& BvhParser::token()
{
  return m_token;
}
