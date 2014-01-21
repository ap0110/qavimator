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

#include <QStringListIterator>

class Animation;
class Joint;

typedef enum
{
  ORDER_NONE,
  ORDER_XYZ,
  ORDER_XZY,
  ORDER_YXZ,
  ORDER_YZX,
  ORDER_ZXY,
  ORDER_ZYX
} AxisOrder;

class BvhJoint
{
  public:
    BvhJoint(const QString& name);
    ~BvhJoint();

    void addChild(BvhJoint* child);

    const QVector3D* head() const;
    const QList<QVector3D*> tail(int index) const;
    void setHead(QVector3D* head);
    void addTail(QVector3D* tail);

    const bool& hasPosition() const;
    const AxisOrder& rotationOrder() const;
    void setHasPosition(bool hasPosition);
    void setRotationOrder(AxisOrder rotationOrder);

    Joint* toJoint();

  private:
    QString m_name;
    QVector3D* m_head;
    QList<QVector3D*> m_tailList;

    QList<BvhJoint*> m_children;

    bool m_hasPosition;
    AxisOrder m_rotationOrder;
    QList<QVector3D*> m_positionFrames;
    QList<QVector3D*> m_rotationFrames;
};

class BvhParser
{
  public:
    BvhParser(const QString& bvhData);
    ~BvhParser();

    Animation* parseBvhData();

  private:
    BvhJoint* parseHierarchy();
    void parseJoint(BvhJoint* joint);
    QVector3D* parseOffset();
    void parseChannels(BvhJoint* joint);

    bool hasNext();
    void next();
    bool hasPrevious();
    void previous();

    const QString& token();
    const QString& nextToken();
    void expectToken(const QString& expected);
    void expectNextToken(const QString& expected);
    bool isEqual(const QString& lhs, const QString& rhs) const;

    QScopedPointer<QStringListIterator> m_parser;

    QStringList m_tokenList;
    QString m_token;
};
