/***************************************************************************
 *   Copyright (C) 2007 by Zi Ree   *
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

#ifndef BVHNODE_H
#define BVHNODE_H

#include <QtCore>

#include <qstring.h>
#include <QVector3D>

#define MAX_FRAMES 1800

typedef enum { BVH_POS, BVH_ROOT, BVH_JOINT, BVH_END, BVH_NO_SL } BVHNodeType;
typedef enum { BVH_XPOS, BVH_YPOS, BVH_ZPOS, BVH_XROT, BVH_YROT, BVH_ZROT } BVHChannelType;
typedef enum { BVH_XYZ=1, BVH_ZYX, BVH_XZY, BVH_YZX, BVH_YXZ, BVH_ZXY} BVHOrderType;

class FrameData
{
  public:
    FrameData();
    FrameData(int frame, QVector3D position, QVector3D rotation);
    ~FrameData();

    int frameNumber() const;
    void setFrameNumber(int frame);

    QVector3D position() const;
    QVector3D rotation() const;
    void setPosition(const QVector3D& position);
    void setRotation(const QVector3D& rotation);

    bool easeIn() const;
    bool easeOut() const;
    void setEaseIn(bool state);
    void setEaseOut(bool state);

    // for debugging purposes, dumps all frame data to debug console
    void dump() const;

  protected:
    unsigned int m_frameNumber;

    QVector3D m_rotation;
    QVector3D m_position;

    bool m_easeIn;
    bool m_easeOut;
};

class BVHNode
{
  public:
    BVHNode(const QString& name);
    ~BVHNode();

    const QString& name() const;
    int numChildren() const;
    BVHNode* child(int num);
    void addChild(BVHNode* newChild);
    void insertChild(BVHNode* newChild,int index);
    void removeChild(BVHNode* child);

    const FrameData frameData(int frame) const;
    const FrameData keyframeDataByIndex(int index) const;
    const QList<int> keyframeList() const;

    void addKeyframe(int frame, QVector3D position, QVector3D rotation);
    void deleteKeyframe(int frame);
    void setKeyframePosition(int frame, const QVector3D& position);
    void setKeyframeRotation(int frame, const QVector3D& rotation);
    void insertFrame(int frame); // moves all key frames starting at "frame" one frame further
    void deleteFrame(int frame); // removes frame at position and moves all further frames one down
    bool isKeyframe(int frame) const;
    int numKeyframes() const;

    const FrameData getKeyframeBefore(int frame) const;
    const FrameData getNextKeyframe(int frame) const;

    void setEaseIn(int frame,bool state);
    void setEaseOut(int frame,bool state);
    bool easeIn(int frame);
    bool easeOut(int frame);

    const QVector3D* getCachedRotation(int frame);
    const QVector3D* getCachedPosition(int frame);
    void cacheRotation(QVector3D* rotation);
    void cachePosition(QVector3D* position);
    void flushFrameCache();

    bool compareFrames(int key1,int key2) const;
    void optimize();

    void dumpKeyframes();

    // set and get mirror nodes for this node
    void setMirror(BVHNode* mirror,unsigned int index);
    BVHNode* getMirror() const;
    unsigned int getMirrorIndex() const;

    // mirrors the rotations in a node and swaps the tracks' keyframes if needed
    void mirror();

    BVHNodeType type;
    double offset[3];
    int numChannels;
    BVHChannelType channelType[6];
    BVHOrderType channelOrder;
    double channelMin[6];
    double channelMax[6];

  protected:
    void setName(const QString& newName);
    double interpolate(double from,double to,int steps,int pos,bool easeOut,bool easeIn) const;

    int getKeyframeNumberBefore(int frame) const;
    int getKeyframeNumberAfter(int frame) const;

    // mirrors the keyframes inside of this node
    void mirrorKeys();

    QString m_name;

    // this node's mirror, if applicable
    BVHNode* mirrorPart;
    unsigned int mirrorIndex;

    QList<BVHNode*> children;
    QMap<int,FrameData> keyframes;

    // rotation/position cache on load, will be cleared once the animation is loaded
    QList<QVector3D*> rotations;
    QList<QVector3D*> positions;
};

#endif
