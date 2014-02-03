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

#include "joint.h"

#include "newanimation.h"

NewAnimation::NewAnimation(QObject* parent,
                           const QSharedPointer<Joint>& rootJoint,
                           const int& maxFrameNumber,
                           const float& frameTime)
  : QObject(parent),
    m_rootJoint(rootJoint),
    m_maxFrameNumber(maxFrameNumber),
    m_frameTime(frameTime),
    m_loopInPoint(1),
    m_loopOutPoint(maxFrameNumber),
    m_isDirty(false)
{
  // TODO Assert root joint is not null
}

NewAnimation::~NewAnimation()
{
}

int NewAnimation::maxFrameNumber() const
{
  return m_maxFrameNumber;
}

void NewAnimation::setMaxFrameNumber(int maxFrameNumber)
{
  m_maxFrameNumber = maxFrameNumber;
  m_rootJoint->setMaxFrameNumber(maxFrameNumber);
}
