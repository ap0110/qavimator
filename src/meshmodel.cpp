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

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "meshmodel.h"

MeshModel::MeshModel(QSharedPointer<Mesh> mesh) :
  QObject(nullptr),
  m_mesh(mesh),
  m_modelView(1.0f, 1.0f, 1.0f)
{
}

void MeshModel::scale(float x, float y, float z)
{
  m_modelView.setX(x);
  m_modelView.setY(y);
  m_modelView.setZ(z);
}

void MeshModel::draw() const
{
  glPushMatrix();

  // TODO Handle only scaling for now
  glScalef(m_modelView.x(), m_modelView.y(), m_modelView.z());
  m_mesh->draw();

  glPopMatrix();
}
