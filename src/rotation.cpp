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

#include "rotation.h"

Rotation::Rotation()
{
  x=0;
  y=0;
  z=0;
}

Rotation::Rotation(double rx,double ry, double rz)
{
  x=rx;
  y=ry;
  z=rz;
}

Rotation Rotation::difference(const Rotation& rot1,const Rotation& rot2)
{
  return Rotation(rot2.x-rot1.x,rot2.y-rot1.y,rot2.z-rot1.z);
}

Rotation::~Rotation()
{
}
