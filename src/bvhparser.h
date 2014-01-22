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
  POSITION_X, POSITION_Y, POSITION_Z,
  ROTATION_X, ROTATION_Y, ROTATION_Z
} Channel;

class BvhFrame
{
  public:
    BvhFrame();
    ~BvhFrame();

    const QVector3D* position() const;
    const QVector3D* rotation() const;

    void setPosition(float x, float y, float z);
    void setRotation(float x, float y, float z);

  private:
    QScopedPointer<QVector3D> m_position;
    QScopedPointer<QVector3D> m_rotation;
};

class BvhJoint
{
  public:
    BvhJoint(const QString& name);
    ~BvhJoint();

    const QList<BvhJoint*> children() const;
    void addChild(QScopedPointer<BvhJoint>& child);

    const QVector3D* head() const;
    const QList<QVector3D*> tail(int index) const;
    void setHead(float x, float y, float z);
    void addTail(float x, float y, float z);

    const QList<Channel> channels() const;
    void addChannel(Channel channel);

    void setMaxFrameCount(int count);
    void addFrame(QScopedPointer<BvhFrame>& frame);

    Joint* toJoint();

  private:
    QString m_name;
    QScopedPointer<QVector3D> m_head;
    QList<QVector3D*> m_tailList;

    QList<BvhJoint*> m_children;

    QList<Channel> m_channels;

    int m_maxFrameCount;
    QList<BvhFrame*> m_frames;
};

class BvhParser
{
  public:
    BvhParser(const QString& bvhData);
    ~BvhParser();

    Animation* parseBvhData();

  private:
    void parseHierarchy(QScopedPointer<BvhJoint>& rootJoint);
    void parseMotion(const QScopedPointer<BvhJoint>& rootJoint);
    void parseJoint(const QScopedPointer<BvhJoint>& joint);
    void parseChannels(const QScopedPointer<BvhJoint>& joint);
    void parseFrame(BvhJoint* joint);

    bool hasNext();
    void next();
    bool hasPrevious();
    void previous();

    const QString& token();
    const QString& nextToken();
    void expectToken(const QString& expected);
    void expectNextToken(const QString& expected);

    bool isEqualIgnoreCase(const QString& lhs, const QString& rhs) const;
    int parseInt(const QString& integerString) const;
    float parseFloat(const QString& floatString) const;

    QScopedPointer<QStringListIterator> m_parser;

    QStringList m_tokenList;
    QString m_token;
};
