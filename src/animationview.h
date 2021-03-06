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
 ***************************************************************************

 * Copyright (C) 2006 by Vinay Pulim.
 * ported to Qt by Zi Ree
 * features added by Darkside Eldrich
 * All rights reserved.             */

/**
  @author Vinay Pulim
  @author Zi Ree <Zi Ree @ SecondLife>
  @author Darkside Eldrich
*/

#ifndef ANIMATIONVIEW_H
#define ANIMATIONVIEW_H

#include <QtOpenGL/QGLWidget>

#include "camera.h"
#include "prop.h"
#include "bvh.h"

class QMouseEvent;
class Animation;
class PropManager;
class Scene;

class AnimationView : public QGLWidget
{
  Q_OBJECT

  public:
    AnimationView(QWidget* parent=0, const char* name=0, Animation* anim=0);
    ~AnimationView();

    // TODO Temporary method while logic is moving around
    Scene* initializeScene(QObject* parent);

    // exports the BVH class handler (ugly, need to find a better way)
    BVH* getBVH() const;

    void selectPart(BVHNode* node);
    void selectProp();
    BVHNode* getSelectedPart();
    unsigned int getSelectedPartIndex();
    // const QString getPartName(int index);

    // TODO Temporary method to set mesh models for removing GLUT
    void setMeshModels(QSharedPointer<Mesh> cubeMesh, QSharedPointer<Mesh> sphereMesh, QSharedPointer<Mesh> coneMesh);

  signals:
    void partClicked(BVHNode* node, QVector3D rotation, RotationLimits rotLimit, QVector3D position);
    void partClicked(int part);
    void propClicked(Prop* prop);

    void partDragged(BVHNode* node,double changeX,double changeY,double changeZ);

    void propDragged(Prop* prop,double changeX,double changeY,double changeZ);
    void propRotated(Prop* prop,double changeX,double changeY,double changeZ);
    void propScaled(Prop* prop,double changeX,double changeY,double changeZ);

    void backgroundClicked();
    void animationSelected(Animation* animation);

    void storeCameraPosition(int num);
    void restoreCameraPosition(int num);

  public slots:
    void resetCamera();
    void selectPart(int part);

  protected slots:
    void draw();

  protected:
    enum
    {
      MODE_PARTS,
      MODE_SKELETON,
      MODE_ROT_AXES
    };

    virtual void paintGL();
    virtual void paintOverlayGL();
    virtual void initializeGL();
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void resizeEvent(QResizeEvent* newSize);

    void drawAnimations();
    void drawFigure(Animation* anim, unsigned int index);
    void drawPart(Animation* anim, unsigned int index, int frame, BVHNode *motion,
                  BVHNode *joints, int mode);
    void drawProps();
    void drawProp(const Prop* prop) const;
    void drawDragHandles(const Prop* prop) const;

    BVH* bvh;

    bool leftMouseButton;
    char modifier;

    QPoint clickPos;           // holds the mouse click position for dragging
    QPoint returnPos;          // holds the mouse position to return to after dragging

    QStringList figureFiles;   // holds the names of the BVH files for male/female skeleton models

    double changeX, changeY, changeZ;
    BVHNode* joints[Animation::NUM_FIGURES];

    bool selecting;
    unsigned int selectName;
    unsigned int partHighlighted;
    unsigned int partSelected;
    unsigned int mirrorSelected;
    unsigned int propDragging;  // holds the drag handle id

    int dragX, dragY;           // holds the last mouse drag offset
    int oldDragX, oldDragY;     // holds the mouse position before the last drag

    int drawMode;
    bool xSelect, ySelect, zSelect;

    void setProjection();
    void setModelView();
    void setBodyMaterial();
    void clearSelected();
    int pickPart(int x, int y);
    void drawCircle(int axis, float radius, int width);

  private:
    enum class OpenGlIds
    {
      // defines where we start counting opengl ids for parts with multiple animations
      // first animation counts 0-ANIMATION_INCREMENT-1, next ANIMATION_INCREMENT++
      AnimationIncrement = 100,

      ObjectStart = 8000,
      DragHandlStart = 9000,
      DragHandleX = 9000,
      DragHandleY = 9001,
      DragHandleZ = 9002,
      ScaleHandleX = 9003,
      ScaleHandleY = 9004,
      ScaleHandleZ = 9005,
      RotateHandleX = 9006,
      RotateHandleY = 9007,
      RotateHandleZ = 9008
    };

    enum class ModifierKeys
    {
      Shift = 1,
      Ctrl = 2,
      Alt = 4
    };

    // TODO Temporary methods while logic is moved around
    PropManager* propManager() const;
    Camera* camera() const;

    // TODO Temporary pointer to scene while logic is being moved around
    Scene* m_scene;

    // TODO Temporary mesh models to remove GLUT
    QScopedPointer<MeshModel> m_cubeMeshModel;
    QScopedPointer<MeshModel> m_sphereMeshModel;
    QScopedPointer<MeshModel> m_coneMeshModel;
};

#endif
