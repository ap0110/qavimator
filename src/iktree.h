/*
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Copyright (C) 2006 by Vinay Pulim.
 * All rights reserved.
 *
 */

#ifndef IKTREE_H
#define IKTREE_H

// #include "bvh.h"
#include "bvhnode.h"

#include "MT_Quaternion.h"

struct IKEffectorList
{
  int num;
  // Size of the array is the maximum number of effectors
  int index[5];
};

struct IKBone
{
  BVHNode *node;
  double weight;
  MT_Vector3 offset;
  MT_Vector3 pos;
  MT_Quaternion lRot;  // local rotation
  MT_Quaternion gRot;  // global rotation
  int numChildren;
  // Size of the array is the maximum number of children
  int child[4];
};

class IKTree
{
  public:
    IKTree(BVHNode *root = NULL);

    void set(BVHNode *root);
    void setGoal(int frame, const QString& name);
    void solve(int frame);

  protected:
    enum {AXIS_X, AXIS_Y, AXIS_Z};

    int numBones;
    // Size of the array is the maximum number of bones
    IKBone bone[50];

    void reset(int frame);
    void addJoint(BVHNode *node);
    void solveJoint(int frame, int i, IKEffectorList &effList);
    void toEuler(MT_Quaternion &q, BVHOrderType order, double &x, double &y, double &z);
    void updateBones(int startIndex);
};

#endif
