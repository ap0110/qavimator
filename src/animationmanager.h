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

#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QObject>

class Animation;

class AnimationManager : public QObject
{
  Q_OBJECT

  public:
    AnimationManager(QObject* parent = 0);
    ~AnimationManager();

    //*** TODO Temporary methods during code organization ***

    int count() const;
    Animation* const at(int index) const;
    bool contains(Animation* const animation) const;
    bool isEmpty() const;
    int indexOf(Animation* const animation) const;

    Animation* first();
    void append(Animation* const animation);
    void deleteAll();
    void clear();

    void setAnimation(Animation* animation);
    Animation* getAnimation();

    //*** End of temporary methods ***

  private:
    QList<Animation*> animationList;
    Animation* selectedAnimation; // this is the "currently selected" animation
};

#endif
