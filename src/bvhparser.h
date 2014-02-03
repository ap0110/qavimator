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

#ifndef BVHPARSER_H
#define BVHPARSER_H

#include <QStringListIterator>
#include <QVector3D>

class Joint;
class KeyframeData;
class NewAnimation;

typedef enum class
{
  POSITION_X, POSITION_Y, POSITION_Z,
  ROTATION_X, ROTATION_Y, ROTATION_Z
} Channel;

class BvhFrame
{
  public:
    BvhFrame(const int& frameNumber);
    ~BvhFrame();

    const QVector3D& position() const;
    const QVector3D& rotation() const;

    void setPosition(const QVector3D& position);
    void setRotation(const QVector3D& rotation);

    KeyframeData toKeyframeData() const;

  private:
    int m_frameNumber;

    QVector3D m_position;
    QVector3D m_rotation;
};

class BvhJoint
{
  public:
    BvhJoint(const QString& name);
    ~BvhJoint();

    const QList<QSharedPointer<BvhJoint>> children() const;
    void addChild(QSharedPointer<BvhJoint> child);

    const QVector3D& head() const;
    const QVector3D& tail(int index) const;
    void setHead(const QVector3D& head);
    void addTail(const QVector3D& tail);

    const QList<Channel> channels() const;
    void addChannel(Channel channel);

    void addFrame(const BvhFrame& frame);

    QSharedPointer<Joint> toJoint();

  private:
    QString m_name;
    QVector3D m_head;
    QList<QVector3D> m_tails;

    QList<QSharedPointer<BvhJoint>> m_children;

    QList<Channel> m_channels;

    QList<BvhFrame> m_frames;
};

class BvhParser
{
  public:
    BvhParser(const QString& bvhData);
    ~BvhParser();

    QSharedPointer<NewAnimation> parse();

  private:
    float parseBvhData(QSharedPointer<Joint>& rootJoint);
    void parseHierarchy(QSharedPointer<BvhJoint>& rootJoint);
    float parseMotion(const QSharedPointer<BvhJoint>& rootJoint);
    void parseJoint(const QSharedPointer<BvhJoint>& joint);
    void parseChannels(const QSharedPointer<BvhJoint>& joint);
    void parseFrame(const QSharedPointer<BvhJoint>& joint, const int& frameNumber);

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

    QStringList m_bvhTokens;
    QStringList::const_iterator m_bvhTokensIterator;
};

#endif
