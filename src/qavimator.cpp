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

#include "propmanager.h"
#include "scene.h"

#include "qavimator.h"

QAvimator::QAvimator(QObject* parent) :
  QObject(parent)
{
}

void QAvimator::setScene(Scene* scene)
{
  m_scene = scene;
}

void QAvimator::newProp(Prop::PropType propType, QSharedPointer<Mesh> mesh)
{
  const Prop* prop = m_scene->addProp(propType, mesh);
  if (prop != nullptr)
  {
    emit newPropUi(prop->name());
  }
}

void QAvimator::selectProp(int index)
{
  const Prop* prop = static_cast<Prop*>(m_scene->propManager()->at(index));
  bool isSelected = (prop != nullptr);
  int attachmentPoint = isSelected ? prop->isAttached() : 0;

  if (prop != nullptr)
  {
    m_scene->propManager()->selectProp(prop->id());
    emit enableAvatarRotationUi(false);
    emit enableAvatarPositionUi(false);
    emit updatePropSpinsUi(prop->position(), prop->rotation(), prop->scale());
  }
  emit enablePropsUi(isSelected);
  emit selectPropUi(isSelected, attachmentPoint);
}

void QAvimator::attachProp(int attachmentPoint)
{
  auto prop = selectedProp();
  prop->attach(attachmentPoint);
  emit updatePropSpinsUi(prop->position(), prop->rotation(), prop->scale());
  emit repaint();
}

void QAvimator::propPositionChanged(double xPosition, double yPosition, double zPosition)
{
  auto prop = selectedProp();
  if (prop != nullptr)
  {
    prop->setPosition(xPosition, yPosition, zPosition);
    emit repaint();
  }
}

void QAvimator::propScaleChanged(double xScale, double yScale, double zScale)
{
  auto prop = selectedProp();
  if (prop != nullptr)
  {
    prop->setScale(xScale, yScale, zScale);
    emit repaint();
  }
}

void QAvimator::propRotationChanged(double xRotation, double yRotation, double zRotation)
{
  auto prop = selectedProp();
  if (prop != nullptr)
  {
    prop->setRotation(xRotation, yRotation, zRotation);
    emit repaint();
  }
}

void QAvimator::deleteProp()
{
  auto prop = selectedProp();
  if (prop != nullptr)
  {
    const QString propName = prop->name();
    m_scene->deleteProp(prop);
    emit deletePropUi(propName);
  }
}

void QAvimator::clearProps()
{
  m_scene->clearProps();
  emit clearPropsUi();
  selectProp(-1);
}

Prop* QAvimator::selectedProp()
{
  auto selectedPropId = m_scene->propManager()->getSelectedPropId();
  return m_scene->getPropById(selectedPropId);
}
