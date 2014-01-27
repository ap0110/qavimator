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
#include <QScopedPointer>
#include <QSharedPointer>
#include <QVector3D>

#include "joint.h"

KeyframeData::KeyframeData()
{
}

KeyframeData::KeyframeData(int frame, float xPosition, float yPosition, float zPosition,
                           float xRotation, float yRotation, float zRotation)
{
  m_frameNumber = frame;
  m_position.reset(new QVector3D(xPosition, yPosition, zPosition));
  m_rotation.reset(new QVector3D(xRotation, yRotation, zRotation));
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

QVector3D KeyframeData::position() const
{
  return *m_position;
}

QVector3D KeyframeData::rotation() const
{
  return *m_rotation;
}

void KeyframeData::setPosition(float x, float y, float z)
{
  m_position.reset(new QVector3D(x, y, z));
}

void KeyframeData::setRotation(float x, float y, float z)
{
  m_rotation.reset(new QVector3D(x, y, z));
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
}

const QString& Joint::name() const
{
  return m_name;
}

QVector3D Joint::head() const
{
  return *m_head;
}

QVector3D Joint::tail(int index) const
{
  return *(m_tails.at(index));
}

void Joint::setHead(float x, float y, float z)
{
  m_head.reset(new QVector3D(x, y, z));
}

void Joint::addTail(float x, float y, float z)
{
  m_tails.append(QSharedPointer<QVector3D>(new QVector3D(x, y, z)));
}

int Joint::numChildren() const
{
  return m_children.size();
}

QSharedPointer<Joint> Joint::child(int num)
{
  return m_children.at(num);
}

void Joint::addChild(QSharedPointer<Joint> child)
{
  m_children.append(child);
}

void Joint::insertChild(int index, QSharedPointer<Joint> child)
{
  m_children.insert(index, child);
}

int Joint::removeChild(QSharedPointer<Joint> child)
{
  return m_children.removeAll(child);
}

void Joint::setKeyframe(int frame, float xPosition, float yPosition, float zPosition,
                        float xRotation, float yRotation, float zRotation)
{
  m_keyframes.insert(frame,QSharedPointer<KeyframeData>(
                       new KeyframeData(frame,
                                        xPosition, yPosition, zPosition,
                                        xRotation, yRotation, zRotation)));
}

bool Joint::removeKeyframe(int frame)
{
  return (m_keyframes.remove(frame) > 0);
}

bool Joint::setKeyframePosition(int frame, float x, float y, float z)
{
  QMap<int, QSharedPointer<KeyframeData> >::iterator iter = m_keyframes.find(frame);
  if (iter != m_keyframes.end())
  {
    (*iter)->setPosition(x, y, z);
    return true;
  }
  else
  {
    return false;
  }
}

bool Joint::setKeyframeRotation(int frame, float x, float y, float z)
{
  QMap<int, QSharedPointer<KeyframeData> >::iterator iter = m_keyframes.find(frame);
  if (iter != m_keyframes.end())
  {
    (*iter)->setRotation(x, y, z);
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

  QMap<int, QSharedPointer<KeyframeData> >::iterator iter = m_keyframes.end();

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
      (*iter)->setFrameNumber(iter.key());
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

  QMap<int, QSharedPointer<KeyframeData> >::iterator iter = m_keyframes.lowerBound(frame);
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
    (*iter)->setFrameNumber(iter.key());
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
  QMap<int, QSharedPointer<KeyframeData> >::iterator iter = m_keyframes.end();
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
