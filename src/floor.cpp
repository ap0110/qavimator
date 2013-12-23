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

#include <QtOpenGL/QGLWidget>

#include "settings.h"

#include "floor.h"

Floor::Floor(QObject* parent)
  : QObject(parent)
{
  frameProtected = false;
}

Floor::~Floor()
{
}

void Floor::draw()
{
  float alpha = (100 - Settings::floorTranslucency()) / 100.0;

  glEnable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);
  for (int i = -10; i < 10; i++)
  {
    for (int j = -10; j < 10; j++)
    {
      if ((i + j) % 2)
      {
        if (frameProtected)
          glColor4f(0.3, 0.0, 0.0, alpha);
        else
          glColor4f(0.1, 0.1, 0.1, alpha);
      }
      else
      {
        if (frameProtected)
          glColor4f(0.8, 0.0, 0.0, alpha);
        else
          glColor4f(0.6, 0.6, 0.6, alpha);
      }

      glVertex3f(i * 40, 0, j * 40);
      glVertex3f(i * 40, 0, (j + 1) * 40);
      glVertex3f((i + 1) * 40, 0, (j + 1) * 40);
      glVertex3f((i + 1) * 40, 0, j * 40);
    }
  }

  glEnd();
}

// set the current frame's visual protection status
void Floor::protectFrame(bool on)
{
  // only redraw if we need to
  if (frameProtected != on)
  {
    frameProtected = on;
    emit repaint();
  }
}
