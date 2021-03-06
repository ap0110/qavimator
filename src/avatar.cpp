/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree   *
 *   Zi Ree @ SecondLife   *
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

#include "avatar.h"

Avatar::Avatar(QObject* parent, float scale, FigureType figureType)
  : QObject(parent),
    m_scale(scale),
    m_figureType(figureType)
{
}

Avatar::~Avatar()
{
}

float Avatar::scale() const
{
  return m_scale;
}

void Avatar::setScale(float scale)
{
  m_scale = scale;
}

FigureType Avatar::figureType() const
{
  return m_figureType;
}

void Avatar::setFigureType(FigureType figureType)
{
  m_figureType = figureType;
}
