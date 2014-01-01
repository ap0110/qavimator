/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree                                          *
 *   Zi Ree @ SecondLife                                                   *
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

#include "animationmanager.h"

AnimationManager::AnimationManager(QObject* parent)
  : QObject(parent)
{
  selectedAnimation = NULL;
}

AnimationManager::~AnimationManager()
{
  clear();
}

bool AnimationManager::selectAnimation(int index)
{
  if (index < animationList.count())
  {
    selectedAnimation = animationList.at(index);
    return true;
  }
  else
  {
    return false;
  }
}

void AnimationManager::clear()
{
  qDeleteAll(animationList);
  animationList.clear();
  selectedAnimation = NULL;
}

void AnimationManager::setFrame(int frame)
{
  for (int i = 0; i < animationList.count(); i++)
  {
    animationList.at(i)->setFrame(frame);
  }
}

void AnimationManager::stepForward()
{
  for (int i = 0; i < animationList.count(); i++)
  {
    animationList.at(i)->stepForward();
  }
}

void AnimationManager::setFPS(int fps)
{
  for (int i = 0; i < animationList.count(); i++)
  {
    animationList.at(i)->setFPS(fps);
  }
}

int AnimationManager::count() const
{
  return animationList.count();
}

Animation* const AnimationManager::at(int index) const
{
  return animationList.at(index);
}

bool AnimationManager::contains(Animation* const animation) const
{
  return animationList.contains(animation);
}

bool AnimationManager::isEmpty() const
{
  return animationList.isEmpty();
}

int AnimationManager::indexOf(Animation* const animation) const
{
  return animationList.indexOf(animation);
}

Animation* AnimationManager::first()
{
  return animationList.first();
}

void AnimationManager::append(Animation* const animation)
{
  animationList.append(animation);
}

void AnimationManager::setAnimation(Animation* animation)
{
  selectedAnimation = animation;
}

Animation* AnimationManager::getAnimation()
{
  return selectedAnimation;
}
