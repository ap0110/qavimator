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
