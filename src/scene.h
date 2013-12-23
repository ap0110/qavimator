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

#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "prop.h"

class Animation;
class Floor;
class Props;

class Scene : public QObject
{
  Q_OBJECT

  public:
    Scene(QObject* parent = 0);
    ~Scene();

    // Sets an animation "active"
    void selectAnimation(int index);

    // this is for setting a single-pose animation. It will clear all other
    // current animations, and become the only active one
    void setAnimation(Animation* animation);

    // this is for adding subsequent animations after the first call to
    // setAnimation
    void addAnimation(Animation* animation);

    // This function clears the animations
    void clear();

    // These functions are re-implemented here so that every animation's
    // frame data can be changed at once
    void setFrame(int frame);
    void stepForward();
    void setFPS(int fps);

    // getAnimation returns the *current* animation
    Animation* getAnimation() { return selectedAnimation; }
    Animation* getAnimation(unsigned int index) { return animationList.at(index); }
    int getIndexOfAnimation(Animation* animation);
    int getCountOfAnimations();

    void drawFloor();

    // TODO Temporary method while logic is moved around
    Props* props();

    const Prop* addProp(Prop::PropType type,
                        double x, double y, double z,
                        double xs, double ys, double zs,
                        double xr, double yr, double zr,
                        int attach);
    Prop* getPropByName(const QString& lookName);
    Prop* getPropById(unsigned int id);
    void deleteProp(Prop* prop);
    void clearProps();

  signals:
    void repaint();
    void animationSelected(Animation* animation);

  public slots:
    void protectFrame(bool on);

  private:
    QList<Animation*> animationList;
    Animation* selectedAnimation; // this is the "currently selected" animation

    Floor* floor;
    Props* m_props;
};

#endif
