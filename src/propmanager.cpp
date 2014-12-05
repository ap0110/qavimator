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

#include "prop.h"

#include "propmanager.h"

PropManager::PropManager(int nextPropId, QObject* parent)
  : QObject(parent),
    m_nextPropId(nextPropId)
{
}

PropManager::~PropManager()
{
}

unsigned int PropManager::getSelectedPropId()
{
  return propSelected;
}

Prop* PropManager::getPropByName(const QString &lookName)
{
  for(unsigned int index=0;index< (unsigned int) propList.count();index++)
  {
    Prop* prop=propList.at(index);
    if(prop->name()==lookName) return prop;
  }

  return 0;
}

Prop* PropManager::getPropById(unsigned int id)
{
  for(unsigned int index=0;index< (unsigned int) propList.count();index++)
  {
    Prop* prop=propList.at(index);
    if(prop->id() == id)
      return prop;
  }

  return 0;
}

Prop* PropManager::at(int i) const
{
  return propList.at(i);
}

int PropManager::count() const
{
  return propList.count();
}

Prop* PropManager::addProp(Prop::PropType type,
                    double x, double y, double z,
                    double xs, double ys, double zs,
                    double xr, double yr, double zr,
                    int attach, QSharedPointer<Mesh> mesh)
{
  QString name;
  QString baseName;

  if(type==Prop::Box) baseName="Box";
  else if(type==Prop::Sphere) baseName="Sphere";
  else if(type==Prop::Cone) baseName="Cone";
  else if(type==Prop::Torus) baseName="Torus";

  int objectNum=0;
  do
  {
    name=baseName+" "+QString::number(objectNum++);
  } while(getPropByName(name));

  Prop* newProp = new Prop(m_nextPropId, type, name, mesh);

  m_nextPropId++;

  newProp->attach(attach);

  newProp->setPosition(x,y,z);
  newProp->setRotation(xr,yr,zr);
  newProp->setScale(xs,ys,zs);

  propList.append(newProp);

  return newProp;
}

void PropManager::deleteProp(Prop *prop)
{
  propList.removeAll(prop);
  delete prop;
}

void PropManager::clearProps()
{
  while(propList.count())
  {
    Prop* prop=propList.at(0);
    propList.removeAll(prop);
    delete prop;
  }
}

void PropManager::selectProp(unsigned int id)
{
  propSelected = id;
}
