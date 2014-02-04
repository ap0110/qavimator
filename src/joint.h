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

#ifndef JOINT_H
#define JOINT_H

#include <QObject>

#include <QMap>
#include <QVector3D>

typedef enum class
{
  XYZ, XZY,
  YXZ, YZX,
  ZXY, ZYX
} RotationOrder;

class KeyframeData
{
  public:
    KeyframeData(int frame, float xPosition, float yPosition, float zPosition,
                 float xRotation, float yRotation, float zRotation);
    ~KeyframeData();

    int frameNumber() const;
    void setFrameNumber(int frame);

    const QVector3D& position() const;
    const QVector3D& rotation() const;
    void setPosition(const QVector3D& position);
    void setRotation(const QVector3D& rotation);

    bool easeIn() const;
    bool easeOut() const;
    void setEaseIn(bool on);
    void setEaseOut(bool on);

  private:
    int m_frameNumber;

    QVector3D m_position;
    QVector3D m_rotation;

    bool m_easeIn;
    bool m_easeOut;
};

class Joint : public QObject
{
  Q_OBJECT

  public:
    Joint(QObject* parent = nullptr, const QString& name = "joint");
    ~Joint();

    const QString& name() const;
    const QVector3D& head() const;
    const QVector3D& tail(int index) const;
    void setHead(const QVector3D& head);
    void setTails(const QList<QVector3D>& tails);

    int numChildren() const;
    QSharedPointer<Joint> child(int num);
    void setChildren(const QList<QSharedPointer<Joint>>& children);
    void addChild(QSharedPointer<Joint> child);
    void insertChild(int index, QSharedPointer<Joint> child);
    int removeChild(QSharedPointer<Joint> child);

    void setKeyframes(const QMap<int, KeyframeData>& keyframes);
    void setKeyframe(int frame, float xPosition, float yPosition, float zPosition, float xRotation, float yRotation, float zRotation);
    bool removeKeyframe(int frame);
    bool setKeyframePosition(int frame, const QVector3D& position);
    bool setKeyframeRotation(int frame, const QVector3D& rotation);
    void insertFrame(int frame, int maxFrameNumber);
    void deleteFrame(int frame);
    int numKeyframes() const;

    void setMaxFrameNumber(int maxFrameNumber);

  private:
    QString m_name;
    QVector3D m_head;
    QList<QVector3D> m_tails;

    QList<QSharedPointer<Joint>> m_children;

    QMap<int, KeyframeData> m_keyframes;
};

#endif
