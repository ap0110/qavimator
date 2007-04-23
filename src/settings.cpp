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

#include <iostream>

#include "settings.h"

static bool m_fog=true;
static int  m_floorTranslucency=25;

Settings::Settings()
{
  // should never be accessed
}

Settings::~Settings()
{
  // should never be accessed
}

void Settings::setFog(bool on)                 { m_fog=on; }
bool Settings::fog()                           { return m_fog; }

void Settings::setFloorTranslucency(int value) { m_floorTranslucency=value; }
int  Settings::floorTranslucency()             { return m_floorTranslucency; }
