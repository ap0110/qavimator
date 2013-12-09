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

#include "props.h"

Props::Props(QObject* parent)
  : QObject(parent)
{
  nextPropId = OBJECT_START;
}

Props::~Props()
{
}

unsigned int Props::getSelectedPropId()
{
  return propSelected;
}

const QString Props::getSelectedPropName() const
{
  for(unsigned int index=0;index< (unsigned int) propList.count();index++)
    if(propList.at(index)->id==propSelected) return propList.at(index)->name();
  return QString();
}

Prop* Props::getPropByName(const QString &lookName)
{
  for(unsigned int index=0;index< (unsigned int) propList.count();index++)
  {
    Prop* prop=propList.at(index);
    if(prop->name()==lookName) return prop;
  }

  return 0;
}

Prop* Props::getPropById(unsigned int id)
{
  for(unsigned int index=0;index< (unsigned int) propList.count();index++)
  {
    Prop* prop=propList.at(index);
    if(prop->id==id) return prop;
  }

  return 0;
}

Prop* Props::at(int i) const
{
  return propList.at(i);
}

int Props::count() const
{
  return propList.count();
}

Prop* Props::addProp(Prop::PropType type,
                    double x, double y, double z,
                    double xs, double ys, double zs,
                    double xr, double yr, double zr,
                    int attach)
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

  Prop* newProp=new Prop(nextPropId,type,name);

  nextPropId++;

  newProp->attach(attach);

  newProp->setPosition(x,y,z);
  newProp->setRotation(xr,yr,zr);
  newProp->setScale(xs,ys,zs);

  propList.append(newProp);

  return newProp;
}

void Props::deleteProp(Prop *prop)
{
  propList.removeAll(prop);
  delete prop;
}

void Props::clearProps()
{
  while(propList.count())
  {
    Prop* prop=propList.at(0);
    propList.removeAll(prop);
    delete prop;
  }
}

void Props::selectProp(unsigned int id)
{
  propSelected = id;
}
