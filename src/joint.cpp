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

#include <QMap>
#include <QVector3D>

#include "joint.h"

KeyframeData::KeyframeData()
{
}

KeyframeData::KeyframeData(int frame, QVector3D& position, QVector3D& rotation)
{
  m_frameNumber = frame;
  m_position = position;
  m_rotation = rotation;
}

KeyframeData::~KeyframeData()
{
}

int KeyframeData::frameNumber() const
{
  return m_frameNumber;
}

void KeyframeData::setFrameNumber(int frame)
{
  m_frameNumber = frame;
}

const QVector3D& KeyframeData::position() const
{
  return m_position;
}

const QVector3D& KeyframeData::rotation() const
{
  return m_rotation;
}

void KeyframeData::setPosition(const QVector3D& position)
{
  m_position = position;
}

void KeyframeData::setRotation(const QVector3D& rotation)
{
  m_rotation = rotation;
}

bool KeyframeData::easeIn() const
{
  return m_easeIn;
}

bool KeyframeData::easeOut() const
{
  return m_easeOut;
}

void KeyframeData::setEaseIn(bool on)
{
  m_easeIn = on;
}

void KeyframeData::setEaseOut(bool on)
{
  m_easeOut = on;
}

Joint::Joint(const QString& name, int maxFrameNumber, QObject* parent)
  : QObject(parent)
{
  m_name = name;
  m_maxFrameNumber = maxFrameNumber;
}

Joint::~Joint()
{
  qDeleteAll(m_tails);
}

const QString& Joint::name() const
{
  return m_name;
}

const QVector3D* Joint::head() const
{
  return m_head.data();
}

const QVector3D* Joint::tail(int index) const
{
  return m_tails.at(index);
}

void Joint::setHead(QVector3D* head)
{
  if (head == NULL)
  {
    // TODO Error: Head cannot be NULL
  }
  m_head.reset(head);
}

void Joint::addTail(QVector3D* tail)
{
  m_tails.append(tail);
}

int Joint::numChildren() const
{
  return m_children.size();
}

Joint* Joint::child(int num)
{
  return m_children.at(num);
}

void Joint::addChild(Joint* child)
{
  m_children.append(child);
}

void Joint::insertChild(Joint* child, int index)
{
  m_children.insert(index, child);
}

void Joint::removeChild(Joint* child)
{
  m_children.removeAll(child);
}

void Joint::setKeyframe(int frame, QVector3D& position, QVector3D& rotation)
{
  m_keyframes.insert(frame, KeyframeData(frame, position, rotation));
}

bool Joint::removeKeyframe(int frame)
{
  return (m_keyframes.remove(frame) > 0);
}

bool Joint::setKeyframePosition(int frame, const QVector3D& position)
{
  QMap<int, KeyframeData>::iterator iter = m_keyframes.find(frame);
  if (iter != m_keyframes.end())
  {
    iter->setPosition(position);
    return true;
  }
  else
  {
    return false;
  }
}

bool Joint::setKeyframeRotation(int frame, const QVector3D& rotation)
{
  QMap<int, KeyframeData>::iterator iter = m_keyframes.find(frame);
  if (iter != m_keyframes.end())
  {
    iter->setRotation(rotation);
    return true;
  }
  else
  {
    return false;
  }
}

void Joint::insertFrame(int frame)
{
  // If there are no keyframes, then do nothing
  if (m_keyframes.empty())
  {
    return;
  }

  QMap<int, KeyframeData>::iterator iter = m_keyframes.end();

  // Start with the last keyframe
  iter--;
  // Update all keyframes containing a frame number greater than or equal to frame
  while (iter.key() >= frame)
  {
    // If the frame number is less than the maximum,
    //  then increment it and re-insert
    if (iter.key() < m_maxFrameNumber)
    {
      iter = m_keyframes.insert(iter + 1, iter.key() + 1, *iter);
      iter->setFrameNumber(iter.key());
      iter--;
    }
    // Erase the keyframe containing either an outdated or invalid frame number
    iter = m_keyframes.erase(iter);

    // If we are at the first keyframe, then exit the loop
    if (iter == m_keyframes.begin())
    {
      break;
    }
    // Otherwise, move backwards one and keep updating keyframes
    else
    {
      iter--;
    }
  }
}

void Joint::deleteFrame(int frame)
{
  // If there are no keyframes, then do nothing
  if (m_keyframes.empty())
  {
    return;
  }

  QMap<int, KeyframeData>::iterator iter = m_keyframes.lowerBound(frame);
  // If the frame we are deleting is a keyframe, then erase without re-inserting
  if (iter != m_keyframes.end() && iter.key() == frame)
  {
    iter = m_keyframes.erase(iter);
  }
  // Update all keyframes containing a frame number greater than frame
  while (iter != m_keyframes.end())
  {
    // Re-insert the current keyframe with a decremented key
    iter = m_keyframes.insert(iter, iter.key() - 1, *iter);
    // Update the keyframe's frame number with its key
    iter->setFrameNumber(iter.key());
    // Erase the keyframe containing an outdated frame number
    iter = m_keyframes.erase(iter + 1);
  }
}

int Joint::numKeyframes() const
{
  return m_keyframes.size();
}

int Joint::maxFrameNumber() const
{
  return m_maxFrameNumber;
}

void Joint::setMaxFrameNumber(int maxFrameNumber)
{
  m_maxFrameNumber = maxFrameNumber;

  // Erase any keyframes containing a frame number greater than the maximum
  QMap<int, KeyframeData>::iterator iter = m_keyframes.end();
  while (iter != m_keyframes.begin())
  {
    iter--;
    if (iter.key() > maxFrameNumber)
    {
      iter = m_keyframes.erase(iter);
    }
    else
    {
      break;
    }
  }
}
