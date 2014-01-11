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

#include <QStringList>

#include "animation.h"

#include "bvhparser.h"

BvhParser::BvhParser(const QString& bvhData)
{
  m_tokenList = bvhData.simplified().split(' ');
}

BvhParser::~BvhParser()
{
}

Animation* BvhParser::parseBvhData()
{
  QStringList::const_iterator parser = m_tokenList.cbegin();

  while (parser != m_tokenList.cend())
  {
    if ((*parser) == "HIERARCHY")
    {
      // TODO Get hierarchy data (do not allow multiple hierarchies)
    }
    else if ((*parser) == "MOTION")
    {
      // TODO Get motion data
    }
    else
    {
      // TODO Error: Should have been HIERARCHY, MOTION, or end-of-file
      return NULL;
    }
  }
  return NULL;
}
