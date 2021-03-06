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

#ifndef NEWANIMATION_H
#define NEWANIMATION_H

#include <QObject>

#include <QSharedPointer>

#include "avatar.h"

class Joint;

class NewAnimation : public QObject
{
  Q_OBJECT

  public:
    NewAnimation(QObject* parent = nullptr,
                 const QSharedPointer<Joint>& rootJoint = QSharedPointer<Joint>(),
                 const int& maxFrameNumber = 2,
                 const float& frameTime = 0.1f);
    ~NewAnimation();

    int maxFrameNumber() const;
    void setMaxFrameNumber(int maxFrameNumber);

    int loopInPoint() const;
    int loopOutPoint() const;
    void setLoopInPoint(int frame);
    void setLoopOutPoint(int frame);

  private:
    QSharedPointer<Joint> m_rootJoint;

    Avatar m_avatar;

    int m_maxFrameNumber;
    float m_frameTime;

    int m_loopInPoint;
    int m_loopOutPoint;

    bool m_isDirty;
};

#endif
