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

#include <QObject>

typedef enum
{
  XYZ, XZY,
  YXZ, YZX,
  ZXY, ZYX
} RotationOrder;

class KeyframeData
{
  public:
    KeyframeData();
    KeyframeData(int frame, QVector3D &position, QVector3D &rotation);
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
    Joint(const QString& name, int maxFrameNumber = 0, QObject* parent = 0);
    ~Joint();

    const QString& name() const;
    const QVector3D* head() const;
    const QVector3D* tail(int index) const;
    void setHead(QVector3D* head);
    void addTail(QVector3D* tail);

    int numChildren() const;
    Joint* child(int num);
    void addChild(Joint* child);
    void insertChild(Joint* child, int index);
    void removeChild(Joint* child);

    void setKeyframe(int frame, QVector3D& position, QVector3D& rotation);
    bool removeKeyframe(int frame);
    bool setKeyframePosition(int frame, const QVector3D& position);
    bool setKeyframeRotation(int frame, const QVector3D& rotation);
    void insertFrame(int frame);
    void deleteFrame(int frame);
    int numKeyframes() const;

    int maxFrameNumber() const;
    void setMaxFrameNumber(int maxFrameNumber);

  private:
    QString m_name;
    QScopedPointer<QVector3D> m_head;
    QList<QVector3D*> m_tails;

    QList<Joint*> m_children;

    QMap<int, KeyframeData> m_keyframes;
    int m_maxFrameNumber;
};
