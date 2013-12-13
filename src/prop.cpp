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
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <QVector3D>

#include "prop.h"

Prop::Prop(unsigned int propId,PropType newType,const QString& newName)
{
  id=propId;
  setType(newType);
  propName=newName;

  m_position.reset(new QVector3D());
  m_scale.reset(new QVector3D());
  m_rotation.reset(new QVector3D());

  attachmentPoint=0;
}

Prop::~Prop()
{
}

void Prop::setType(PropType newType)
{
  type=newType;
}

const QString& Prop::name() const
{
  return propName;
}

unsigned int Prop::isAttached() const
{
  return attachmentPoint;
}

float Prop::xPosition() const
{
  return m_position->x();
}

float Prop::yPosition() const
{
  return m_position->y();
}

float Prop::zPosition() const
{
  return m_position->z();
}

float Prop::xScale() const
{
  return m_scale->x();
}

float Prop::yScale() const
{
  return m_scale->y();
}

float Prop::zScale() const
{
  return m_scale->z();
}

float Prop::xRotation() const
{
  return m_rotation->x();
}

float Prop::yRotation() const
{
  return m_rotation->y();
}

float Prop::zRotation() const
{
  return m_rotation->z();
}

void Prop::setPosition(double xp,double yp,double zp)
{
  m_position->setX(xp);
  m_position->setY(yp);
  m_position->setZ(zp);
}

void Prop::setScale(double scx,double scy,double scz)
{
  m_scale->setX(scx);
  m_scale->setY(scy);
  m_scale->setZ(scz);
}

void Prop::setRotation(double rx,double ry,double rz)
{
  m_rotation->setX(rx);
  m_rotation->setY(ry);
  m_rotation->setZ(rz);
}

void Prop::draw(State state) const
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glPushMatrix();

  glTranslatef(m_position->x(), m_position->y(), m_position->z());

  glRotatef(m_rotation->x(), 1, 0, 0);
  glRotatef(m_rotation->y(), 0, 1, 0);
  glRotatef(m_rotation->z(), 0, 0, 1);

  // load prop's id, so we can pick it later
  glLoadName(id);

  if(state==Normal)
    glColor4f(0.3,0.4,1.0, 1);
  else if(state==Highlighted)
    glColor4f(0.4, 0.5, 0.3, 1);
  else
    glColor4f(0.6, 0.3, 0.3, 1);

  // each prop type has its own base sizes and positions
  if(type==Box)
  {
    glScalef(m_scale->x(), m_scale->y(), m_scale->z());
    glutSolidCube(1);
  }
  else if(type==Sphere)
  {
    glScalef(m_scale->x() / 2, m_scale->y() / 2, m_scale->z() / 2);
    glutSolidSphere(1,16,16);
  }
  else if(type==Cone)
  {
    glTranslatef(0,0,-5);
    glScalef(m_scale->x() / 2, m_scale->y() / 2, m_scale->z() / 2);
    glutSolidCone(1,2,16,16);
  }
  else if(type==Torus)
  {
    glScalef(m_scale->x() / 4, m_scale->y() / 4, m_scale->z() / 2);
    glutSolidTorus(1,1,16,16);
  }

  glPopMatrix();
}

void Prop::attach(unsigned int where)
{
  attachmentPoint=where;
  if(where)
  {
    setPosition(0,0,0);
  }
  else
  {
    setPosition(10,40,10);
  }
}
