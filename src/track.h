/***************************************************************************
 *   Copyright (C) 2008 by Zi Ree   *
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

#ifndef TRACK_H
#define TRACK_H

class BVHNode;

class Track
{
  public:
    typedef enum
    {
      TYPE_ROTATION=0,
      TYPE_POSITION=1
    } TrackType;

    Track(BVHNode* trackNode,int num,TrackType trackType);
    ~Track();

    bool isEndSite();
    int getNum();
    BVHNode* getNode();

  protected:
    int num;
    bool endSite;       // true if this track is the last before the "Site" node
    TrackType type;
    BVHNode* node;
};

#endif // TRACK_H
