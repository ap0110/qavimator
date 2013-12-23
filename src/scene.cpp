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

#include "animation.h"
#include "floor.h"

#include "scene.h"

Scene::Scene(QObject* parent)
  : QObject(parent)
{
  selectedAnimation = NULL;
  floor = new Floor(this);
}

Scene::~Scene()
{
  clear();
}

void Scene::selectAnimation(int index)
{
  if (index < animationList.count())
  {
    selectedAnimation = animationList.at(index);
    emit animationSelected(getAnimation());
    emit repaint();
  }
}

void Scene::setAnimation(Animation* animation)
{
  clear();

  selectedAnimation = animation;
  animationList.append(animation);
  connect(animation, SIGNAL(frameChanged()), this, SIGNAL(repaint()));
  emit repaint();
}

// Adds a new animation without overriding others, and sets it current
void Scene::addAnimation(Animation* animation)
{
  if (!animationList.contains(animation))
  {
    animationList.append(animation);
    selectedAnimation = animation; // set it as the current one
    if (!animationList.isEmpty() && animation != animationList.first())
      animation->setFrame(animationList.first()->getFrame());

    connect(animation, SIGNAL(frameChanged()), this, SIGNAL(repaint()));
    emit repaint();
  }
}

void Scene::clear()
{
  qDeleteAll(animationList);
  animationList.clear();
  selectedAnimation = NULL;
}

void Scene::setFrame(int frame)
{
  for (int i = 0; i < animationList.count(); i++)
  {
    animationList.at(i)->setFrame(frame);
  }
}

void Scene::stepForward()
{
  for (int i = 0; i < animationList.count(); i++)
  {
    animationList.at(i)->stepForward();
  }
}

void Scene::setFPS(int fps)
{
  for (int i = 0; i < animationList.count(); i++)
  {
    animationList.at(i)->setFPS(fps);
  }
}

int Scene::getIndexOfAnimation(Animation* animation)
{
  return animationList.indexOf(animation);
}

int Scene::getCountOfAnimations()
{
  return animationList.count();
}

void Scene::drawFloor()
{
  floor->draw();
}

void Scene::protectFrame(bool on)
{
  floor->protectFrame(on);
}
