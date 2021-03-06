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

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QScopedPointer>
#include <QSharedPointer>

#include "aboutdialog.h"
#include "animationview.h"
#include "constants.h"
#include "prop.h"
#include "qavimator.h"
#include "scene.h"
#include "timeline.h"
#include "timelineview.h"
#include "updatersettings.h"
#include "usersettings.h"
#include "usersettingsdialog.h"

#include "qavimatorwindow.h"
#include "ui_qavimatorwindow.h"
#include "whatsnewdialog.h"

QAvimatorWindow::QAvimatorWindow() :
  QMainWindow(nullptr),
  ui(new Ui::QAvimatorWindow),
  m_qavimator(new QAvimator(this)),
  updateChecker(nullptr, this),
  cubeMesh(new Mesh(Constants::cubeMeshFilePath(), Mesh::Shape::CUBE)),
  sphereMesh(new Mesh(Constants::sphereMeshFilePath(), Mesh::Shape::SPHERE)),
  coneMesh(new Mesh(Constants::coneMeshFilePath(), Mesh::Shape::CONE)),
  torusMesh(new Mesh(Constants::torusMeshFilePath(), Mesh::Shape::TORUS)),
  m_animFilter("Animation Files (*.avm *.bvh)"),
  m_propFilter("Props (*.prp)"),
  m_untitledName("Untitled.avm"),
  m_precision(100)
{
  nodeMapping <<  0
              <<  1 <<  2 <<  3 <<  4 << 5
              <<  7 <<  8 <<  9 << 10
              << 12 << 13 << 14 << 15
              << 17 << 18 << 19
              << 21 << 22 << 23;

  QApplication::setOrganizationName(Constants::organizationName());
  QApplication::setOrganizationDomain(Constants::organizationDomain());
  QApplication::setApplicationName(Constants::applicationName());
  QApplication::setApplicationVersion(Constants::versionNumberString());

  ui->setupUi(this);

  setWindowTitle("qavimator");
  setAttribute(Qt::WA_DeleteOnClose);
  frameDataValid=false;
  currentPart=0;
  longestRunningTime=0.0;
  scene = ui->animationView->initializeScene(this);
  m_qavimator->setScene(scene);
  ui->animationView->setMeshModels(cubeMesh, sphereMesh, coneMesh);

  // prepare play button icons
  stopIcon=QIcon(":/icons/icons/stop.png");
  playIcon=QIcon(":/icons/icons/play.png");
  loopIcon=QIcon(":/icons/icons/loop.png");

  // playback stopped by default
  setPlaystate(PLAYSTATE_STOPPED);

  UserSettings::readSettings();

  resize(UserSettings::mainWindowWidth(),UserSettings::mainWindowHeight());

  ui->optionsLoopAction->setChecked(UserSettings::loop());
  ui->optionsSkeletonAction->setChecked(UserSettings::skeleton());
  ui->optionsJointLimitsAction->setChecked(UserSettings::jointLimits());
  ui->optionsShowTimelineAction->setChecked(UserSettings::showTimelinePanel());
  ui->optionsProtectFirstFrameAction->setChecked(UserSettings::protectFirstFrame());

  if(!UserSettings::showTimelinePanel()) ui->timelineView->hide();

  //ui->propNameCombo->setModel(scene->propManager()->addressOfPropModel());

  connect(m_qavimator.data(), &QAvimator::repaint, ui->animationView, static_cast<void (AnimationView::*)()>(&AnimationView::repaint));
  connect(scene, SIGNAL(repaint()), ui->animationView, SLOT(repaint()));
  connect(scene, SIGNAL(animationSelected(Animation*)), this, SLOT(selectAnimation(Animation*)));
  connect(this, SIGNAL(protectFrame(bool)), scene, SLOT(protectFrame(bool)));

  connect(ui->animationView, SIGNAL(storeCameraPosition(int)), scene, SLOT(storeCameraPosition(int)));
  connect(ui->animationView, SIGNAL(restoreCameraPosition(int)), scene, SLOT(restoreCameraPosition(int)));

  connect(ui->animationView,SIGNAL(partClicked(BVHNode*,
                                           QVector3D,
                                           RotationLimits,
                                           QVector3D)),
                     this,SLOT(partClicked(BVHNode*,
                                           QVector3D,
                                           RotationLimits,
                                           QVector3D)));

  connect(ui->animationView,SIGNAL(partDragged(BVHNode*,double,double,double)),
                     this,SLOT(partDragged(BVHNode*,double,double,double)));

  connect(ui->animationView,SIGNAL(propClicked(Prop*)),this,SLOT(propClicked(Prop*)));

  connect(ui->animationView,SIGNAL(propDragged(Prop*,double,double,double)),
                     this,SLOT(propDragged(Prop*,double,double,double)));

  connect(ui->animationView,SIGNAL(propScaled(Prop*,double,double,double)),
                     this,SLOT(propScaled(Prop*,double,double,double)));

  connect(ui->animationView,SIGNAL(propRotated(Prop*,double,double,double)),
                     this,SLOT(propRotated(Prop*,double,double,double)));

  connect(ui->animationView,SIGNAL(backgroundClicked()),this,SLOT(backgroundClicked()));
  connect(ui->animationView,SIGNAL(animationSelected(Animation*)),this,SLOT(selectAnimation(Animation*)));

  connect(this, &QAvimatorWindow::newProp, m_qavimator.data(), &QAvimator::newProp);
  connect(this, static_cast<void (QAvimatorWindow::*)(int)>(&QAvimatorWindow::selectProp), m_qavimator.data(), &QAvimator::selectProp);
  connect(this, &QAvimatorWindow::attachProp, m_qavimator.data(), &QAvimator::attachProp);
  connect(this, &QAvimatorWindow::propPositionChanged, m_qavimator.data(), &QAvimator::propPositionChanged);
  connect(this, &QAvimatorWindow::propScaleChanged, m_qavimator.data(), &QAvimator::propScaleChanged);
  connect(this, &QAvimatorWindow::propRotationChanged, m_qavimator.data(), &QAvimator::propRotationChanged);
  connect(this, &QAvimatorWindow::deleteProp, m_qavimator.data(), &QAvimator::deleteProp);
  connect(this, &QAvimatorWindow::clearProps, m_qavimator.data(), &QAvimator::clearProps);
  connect(m_qavimator.data(), &QAvimator::newPropUi, this, &QAvimatorWindow::onNewProp);
  connect(m_qavimator.data(), &QAvimator::selectPropUi, this, &QAvimatorWindow::onSelectProp);
  connect(m_qavimator.data(), &QAvimator::deletePropUi, this, &QAvimatorWindow::onDeleteProp);
  connect(m_qavimator.data(), &QAvimator::clearPropsUi, this, &QAvimatorWindow::onClearProps);
  connect(m_qavimator.data(), &QAvimator::enablePropsUi, this, &QAvimatorWindow::enableProps);
  connect(m_qavimator.data(), &QAvimator::enableAvatarRotationUi, this, &QAvimatorWindow::enableAvatarRotation);
  connect(m_qavimator.data(), &QAvimator::enableAvatarPositionUi, this, &QAvimatorWindow::enableAvatarPosition);
  connect(m_qavimator.data(), &QAvimator::updatePropSpinsUi, this, &QAvimatorWindow::updatePropSpins);

  connect(this,SIGNAL(enableEaseInOut(bool)),ui->easeInOutGroup,SLOT(setEnabled(bool)));

  connect(&timer,SIGNAL(timeout()),this,SLOT(frameTimeout()));

  connect(this,SIGNAL(resetCamera()),ui->animationView,SLOT(resetCamera()));

  connect(ui->animationView,SIGNAL(partClicked(int)),ui->timelineView,SLOT(selectTrack(int)));
  connect(ui->animationView,SIGNAL(backgroundClicked()),ui->timelineView,SLOT(backgroundClicked()));

  timeline=ui->timelineView->getTimeline();
  connect(timeline,SIGNAL(positionCenter(int)),ui->timelineView,SLOT(scrollTo(int)));
  connect(timeline,SIGNAL(trackClicked(int)),ui->animationView,SLOT(selectPart(int)));

  ui->xRotationSlider->setPageStep(10 * m_precision);
  ui->yRotationSlider->setPageStep(10 * m_precision);
  ui->zRotationSlider->setPageStep(10 * m_precision);
  ui->xPositionSlider->setPageStep(10 * m_precision);
  ui->yPositionSlider->setPageStep(10 * m_precision);
  ui->zPositionSlider->setPageStep(10 * m_precision);

  ui->currentFrameSlider->setPageStep(1);

  if (!updateChecker.hasNews())
  {
    ui->helpWhatsNewAction->setVisible(false);
  }

  QStringList args = QApplication::arguments();
  if(args.size()>1)
  {
    fileOpen(args[1]);
  }

  // if opening of files didn't work or no files were specified on the
  // command line, open a new one
  if(openFiles.count()==0) fileNew();

  updateInputs();
}

QAvimatorWindow::~QAvimatorWindow()
{
  fileExit();
}

void QAvimatorWindow::queueAfterShow()
{
  // QTimer creates a timerEvent
  // - This is a convenient way to queue "afterShow" to
  //   run after the main window shows
  QTimer::singleShot(0, this, SLOT(afterShow()));
}

void QAvimatorWindow::afterShow()
{
  updateChecker.onStartup();
}

// slot gets called by AnimationView::mousePressEvent()
void QAvimatorWindow::partClicked(BVHNode* node, QVector3D rotation, RotationLimits limits, QVector3D position)
{
  ui->avatarPropsTab->setCurrentIndex(0);
  enableProps(false);

  if(!node)
  {
    qDebug("qavimator::partClicked(node,...): node==0!");
    return;
  }

  if(node)
  {
    qDebug("qavimator::partClicked(node): %s",node->name().toLatin1().constData());
    currentPart=node;

    for(int index=0;index<ui->editPartCombo->count();index++)
      if(ui->editPartCombo->itemText(index)==currentPart->name()) ui->editPartCombo->setCurrentIndex(index);

    // do not send signals for moving sliders while changing the slider limits
    ui->xRotationSlider->blockSignals(true);
    ui->yRotationSlider->blockSignals(true);
    ui->zRotationSlider->blockSignals(true);

    // hip gets a full 360 degree limit, all others according to SL.lim
    // maybe this shouldn't be like this to allow for multi rotation spins
    if(currentPart->type==BVH_ROOT)
    {
      ui->xRotationSlider->setRange(-359 * m_precision, 359 * m_precision);
      ui->yRotationSlider->setRange(-359 * m_precision, 359 * m_precision);
      ui->zRotationSlider->setRange(-359 * m_precision, 359 * m_precision);
    }
    else
    {
      ui->xRotationSlider->setRange((int)(limits.minimum.x() * m_precision), (int)(limits.maximum.x() * m_precision));
      ui->yRotationSlider->setRange((int)(limits.minimum.y() * m_precision), (int)(limits.maximum.y() * m_precision));
      ui->zRotationSlider->setRange((int)(limits.minimum.z() * m_precision), (int)(limits.maximum.z() * m_precision));
    }

    // re-enable signals
    ui->xRotationSlider->blockSignals(false);
    ui->yRotationSlider->blockSignals(false);
    ui->zRotationSlider->blockSignals(false);

    setX(rotation.x());
    setY(rotation.y());
    setZ(rotation.z());

    if(node->type==BVH_POS)
      enableAvatarRotation(false);
    else
      enableAvatarRotation(!protect);

    setXPos(position.x());
    setYPos(position.y());
    setZPos(position.z());

    // show the user if this part has a key frame here
    updateKeyBtn();
  }
}

// slot gets called by AnimationView::mouseMoveEvent()
void QAvimatorWindow::partDragged(BVHNode* node,double x,double y,double z)
{
  if(node)
  {
    // check if this frame is protected
    if(!protect)
    {
      // get animation object
      Animation* anim=scene->getAnimation();
      // get rotation values for selected part
      QVector3D rotation = anim->getRotation(node);
      // get rotation limits for part
      RotationLimits rotLimits=anim->getRotationLimits(node);

      // calculate new rotation (x, y, z are the modifiers)
      double newX = rotation.x() + x;
      double newY = rotation.y() + y;
      double newZ = rotation.z() + z;

      double xMin=rotLimits.minimum.x();
      double yMin=rotLimits.minimum.y();
      double zMin=rotLimits.minimum.z();
      double xMax=rotLimits.maximum.x();
      double yMax=rotLimits.maximum.y();
      double zMax=rotLimits.maximum.z();

      if(newX<xMin) newX=xMin;
      if(newX>xMax) newX=xMax;
      if(newY<yMin) newY=yMin;
      if(newY>yMax) newY=yMax;
      if(newZ<zMin) newZ=zMin;
      if(newZ>zMax) newZ=zMax;

      setX(newX);
      setY(newY);
      setZ(newZ);

      scene->getAnimation()->setRotation(node,newX,newY,newZ);
      ui->animationView->repaint();

      updateKeyBtn();
    }
  }
  else qDebug("qavimator::partDragged(): node==0!");
}

// slot gets called by AnimationView::propClicked()
void QAvimatorWindow::propClicked(Prop* prop)
{
  ui->avatarPropsTab->setCurrentIndex(1);

  // update prop name combo box
  for(int index=0;index<ui->propNameCombo->count();index++)
    if(ui->propNameCombo->itemText(index)==prop->name()) ui->propNameCombo->setCurrentIndex(index);

  // update prop value spinboxes
  selectProp();
}

// slot gets called by AnimationView::mouseMoveEvent()
void QAvimatorWindow::propDragged(Prop* prop,double x,double y,double z)
{
  double newX = prop->xPosition() + x;
  double newY = prop->yPosition() + y;
  double newZ = prop->zPosition() + z;
  prop->setPosition(newX, newY, newZ);
  updatePropSpins(prop->position(), prop->rotation(), prop->scale());
}

// slot gets called by AnimationView::mouseMoveEvent()
void QAvimatorWindow::propScaled(Prop* prop,double x,double y,double z)
{
  double newX = prop->xScale() + x;
  double newY = prop->yScale() + y;
  double newZ = prop->zScale() + z;
  prop->setScale(newX, newY, newZ);
  updatePropSpins(prop->position(), prop->rotation(), prop->scale());
}

// slot gets called by AnimationView::mouseMoveEvent()
void QAvimatorWindow::propRotated(Prop* prop,double x,double y,double z)
{
  double newX = prop->xRotation() + x;
  double newY = prop->yRotation() + y;
  double newZ = prop->zRotation() + z;
  prop->setRotation(newX, newY, newZ);
  updatePropSpins(prop->position(), prop->rotation(), prop->scale());
}

// slot gets called by AnimationView::mouseButtonClicked()
void QAvimatorWindow::backgroundClicked()
{
  currentPart=0;

  enableAvatarRotation(false);
  enableProps(false);
  emit enableEaseInOut(false);
  ui->editPartCombo->setCurrentIndex(0);
  updateKeyBtn();
}

// slot gets called by body part dropdown list
void QAvimatorWindow::partChoice()
{
  // get node number from entry list in combo box
  int nodeNumber=nodeMapping[ui->editPartCombo->currentIndex()];
  // selectPart will fire partClicked signal, so we don't bother
  // about updating controls or currentPart pointer ourselves here
  ui->animationView->selectPart(nodeNumber);
  ui->timelineView->selectTrack(nodeNumber);

  ui->animationView->setFocus();
  enableProps(false);
}

// gets called whenever a body part rotation slider is moved
void QAvimatorWindow::rotationSlider(const QObject* slider)
{
  Animation* anim=scene->getAnimation();
  QVector3D rotation = anim->getRotation(currentPart);

  double x = rotation.x();
  double y = rotation.y();
  double z = rotation.z();

  if(slider==ui->xRotationSlider)
  {
    x=getX();
    setX(x);
  }
  else if(slider==ui->yRotationSlider)
  {
    y=getY();
    setY(y);
  }
  else if(slider==ui->zRotationSlider)
  {
    z=getZ();
    setZ(z);
  }

  if(ui->animationView->getSelectedPart())
  {
    anim->setRotation(ui->animationView->getSelectedPart(),x,y,z);
    ui->animationView->repaint();
  }

  updateKeyBtn();
}

// gets called whenever a body part rotation value field gets changed
void QAvimatorWindow::rotationValue()
{
  double x=ui->xRotationEdit->text().toDouble();
  double y=ui->yRotationEdit->text().toDouble();
  double z=ui->zRotationEdit->text().toDouble();

  double min_x = ui->xRotationSlider->minimum() / m_precision;
  double min_y = ui->yRotationSlider->minimum() / m_precision;
  double min_z = ui->zRotationSlider->minimum() / m_precision;

  double max_x = ui->xRotationSlider->maximum() / m_precision;
  double max_y = ui->yRotationSlider->maximum() / m_precision;
  double max_z = ui->zRotationSlider->maximum() / m_precision;

  if(x<min_x) x=min_x;
  if(y<min_y) y=min_y;
  if(z<min_z) z=min_z;

  if(x>max_x) x=max_x;
  if(y>max_y) y=max_y;
  if(z>max_z) z=max_z;

  setX(x);
  setY(y);
  setZ(z);

  Animation* anim=scene->getAnimation();
  if(ui->animationView->getSelectedPart())
  {
    anim->setRotation(ui->animationView->getSelectedPart(), x, y, z);
    ui->animationView->repaint();
  }

  updateKeyBtn();
}

void QAvimatorWindow::positionSlider(const QObject* slider)
{
  Animation* anim=scene->getAnimation();
  QVector3D position = anim->getPosition();

  double x = position.x();
  double y = position.y();
  double z = position.z();

  if(slider==ui->xPositionSlider)
  {
    x=getXPos();
    setXPos(x);
  }
  else if(slider==ui->yPositionSlider)
  {
    y=getYPos();
    setYPos(y);
  }
  else if(slider==ui->zPositionSlider)
  {
    z=getZPos();
    setZPos(z);
  }

  scene->getAnimation()->setPosition(x,y,z);
  ui->animationView->repaint();

  updateKeyBtn();
}

void QAvimatorWindow::positionValue()
{
  qDebug("qavimator::positionValue()");

  double x=ui->xPositionEdit->text().toDouble();
  double y=ui->yPositionEdit->text().toDouble();
  double z=ui->zPositionEdit->text().toDouble();

  double min_x = ui->xPositionSlider->minimum() / m_precision;
  double min_y = ui->yPositionSlider->minimum() / m_precision;
  double min_z = ui->zPositionSlider->minimum() / m_precision;

  double max_x = ui->xPositionSlider->maximum() / m_precision;
  double max_y = ui->yPositionSlider->maximum() / m_precision;
  double max_z = ui->zPositionSlider->maximum() / m_precision;

  if(x<min_x) x=min_x;
  if(y<min_y) y=min_y;
  if(z<min_z) z=min_z;

  if(x>max_x) x=max_x;
  if(y>max_y) y=max_y;
  if(z>max_z) z= max_z;

  setXPos(x);
  setYPos(y);
  setZPos(z);

  scene->getAnimation()->setPosition(x,y,z);
  ui->animationView->repaint();

  updateKeyBtn();
}

void QAvimatorWindow::updateInputs()
{
  // deactivate redraw to reduce interface "jitter" during updating
  setUpdatesEnabled(false);

  Animation* anim=scene->getAnimation();
  if (anim)
  {
    if(currentPart != NULL)
    {
      QVector3D rotation = anim->getRotation(currentPart);

      double x = rotation.x();
      double y = rotation.y();
      double z = rotation.z();

      RotationLimits rotLimits=anim->getRotationLimits(currentPart);

      double xMin=rotLimits.minimum.x();
      double yMin=rotLimits.minimum.y();
      double zMin=rotLimits.minimum.z();
      double xMax=rotLimits.maximum.x();
      double yMax=rotLimits.maximum.y();
      double zMax=rotLimits.maximum.z();

      if(currentPart->type==BVH_ROOT)
      {
        ui->xRotationSlider->setRange(-359 * m_precision, 359 * m_precision);
        ui->yRotationSlider->setRange(-359 * m_precision, 359 * m_precision);
        ui->zRotationSlider->setRange(-359 * m_precision, 359 * m_precision);
      }
      else
      {
        ui->xRotationSlider->setRange((int)(xMin * m_precision), (int)(xMax * m_precision));
        ui->yRotationSlider->setRange((int)(yMin * m_precision), (int)(yMax * m_precision));
        ui->zRotationSlider->setRange((int)(zMin * m_precision), (int)(zMax * m_precision));
      }

      setX(x);
      setY(y);
      setZ(z);
    }
    else
    {
      enableAvatarRotation(false);
    }

    enableAvatarPosition(!protect);
    QVector3D position = anim->getPosition();

    setXPos(position.x());
    setYPos(position.y());
    setZPos(position.z());

    // we do that in nextPlaystate() now
    //  playButton->setIcon(playing ? stopIcon : playIcon);

    ui->framesSpin->setValue(anim->getNumberOfFrames());
    ui->currentFrameSlider->setMaximum(anim->getNumberOfFrames()-1);
    ui->fpsSpin->setValue(anim->fps());

    // prevent feedback loop
    ui->scaleSpin->blockSignals(true);
    ui->scaleSpin->setValue(anim->getAvatarScale()*100.0+0.1);  // +0.1 to overcome rounding errors
    ui->scaleSpin->blockSignals(false);

    updateKeyBtn();
  }

  if(playstate==PLAYSTATE_STOPPED)
    emit enableInputs(true);
  else
    emit enableInputs(false);

  if (frameDataValid)
    ui->editPasteAction->setEnabled(true);
  else
    ui->editPasteAction->setEnabled(false);

  if(ui->propNameCombo->count())
    enableProps(true);
  else
    enableProps(false);

  // reactivate redraw
  setUpdatesEnabled(true);
}

void QAvimatorWindow::updateKeyBtn()
{
  Animation* anim=scene->getAnimation();
  qDebug("qavimator::updateKeyBtn(): anim=%lx",(unsigned long) anim);

  // make sure we don't send a toggle signal on display update
  ui->keyframeButton->blockSignals(true);

  int frame=anim->getFrame();
  int partIndex=ui->animationView->getSelectedPartIndex();
  bool hasKeyframe=anim->isKeyFrame(partIndex,frame);
  ui->keyframeButton->setDown(hasKeyframe);

  // re-enable toggle signal
  ui->keyframeButton->blockSignals(false);

  if(hasKeyframe && currentPart != NULL)
  {
    emit enableEaseInOut(true);
    ui->easeInCheck->setChecked(anim->easeIn(currentPart,anim->getFrame()));
    ui->easeOutCheck->setChecked(anim->easeOut(currentPart,anim->getFrame()));
  }
  else
    emit enableEaseInOut(false);

//  timeline->repaint();
}

void QAvimatorWindow::enableInputs(bool state)
{
  // protection overrides state for keyframe button
  if(protect) state=false;
  ui->keyframeButton->setEnabled(state);

  // do not enable rotation if we have no part selected
  if(currentPart == NULL) state=false;
  enableAvatarRotation(state);
}

void QAvimatorWindow::frameTimeout()
{
  // only if we are still playing
  if(playstate!=PLAYSTATE_STOPPED)
  {
    Animation* anim=scene->getAnimation();
    if(anim)
    {
      // don't show protected frames color on playback to avoid flicker
      emit protectFrame(false);
      // cycle through frames, restart at looping point
      scene->stepForward();

      if(anim->getFrame()==(anim->getNumberOfFrames()-1) && playstate==PLAYSTATE_PLAYING)
      {
        timer.stop();
        setPlaystate(PLAYSTATE_STOPPED);
      }

      updateInputs();
      return;
    }
  }
}

void QAvimatorWindow::nextPlaystate()
{
//  qDebug("qavimator::nextPlaystate(): current playstate %d",(int) playstate);

  Animation* anim=scene->getAnimation();

  switch(playstate)
  {
    case PLAYSTATE_STOPPED:
    {
      // if we're suposed to loop and the current frame is not past loop out point
      if(UserSettings::loop() && anim->getFrame()<anim->getLoopOutPoint())
      {
        // start looping animation
        setPlaystate(PLAYSTATE_LOOPING);
        anim->setPlaystate(PLAYSTATE_LOOPING);
      }
      else
      {
        // start one-shot animation
        setPlaystate(PLAYSTATE_PLAYING);
        anim->setPlaystate(PLAYSTATE_PLAYING);
      }

      timer.start((int)(1.0/anim->fps()*1000.0));
      break;
    }
    case PLAYSTATE_LOOPING:
    {
      setPlaystate(PLAYSTATE_PLAYING);
      anim->setPlaystate(PLAYSTATE_PLAYING);
      break;
    }
    case PLAYSTATE_PLAYING:
    {
      // take care of locks, key frames ...
      frameSlider(ui->currentFrameSlider->value());
      setPlaystate(PLAYSTATE_STOPPED);
      anim->setPlaystate(PLAYSTATE_STOPPED);

      break;
    }
    default:
      qDebug("qavimator::nextPlaystate(): unknown playstate %d",(int) playstate);
  }

  updateInputs();
}

void QAvimatorWindow::setFPS(int fps)
{
  qDebug("qavimator::setFPS(%d)",fps);

  Animation* anim=scene->getAnimation();
  if(!anim) return;

  // sanity check
  if(fps<1) fps=1;
  else if(fps>50) fps=50;

  anim->setFPS(fps);
  calculateLongestRunningTime();
}

void QAvimatorWindow::frameSlider(int position)
{
  // check if we are at the first frame and if it's protected
  if(position==0 && UserSettings::protectFirstFrame()) protect=true;
  else protect=false;

  emit protectFrame(protect);
  setPlaystate(PLAYSTATE_STOPPED);
  scene->setFrame(position);

  updateInputs();
}

void QAvimatorWindow::setAvatarShape(int shape)
{
  Animation* anim=scene->getAnimation();
  if(!anim) return;

  if(shape==0)
    anim->setFigureType(Animation::FIGURE_FEMALE);
  else
    anim->setFigureType(Animation::FIGURE_MALE);
  ui->animationView->repaint();
}

void QAvimatorWindow::setAvatarScale(int percent)
{
  scene->getAnimation()->setAvatarScale(percent/100.0);
  ui->animationView->repaint();
}

void QAvimatorWindow::numFramesChanged(int num)
{
  if(num<1) num=1;
  Animation* anim=scene->getAnimation();
  anim->setNumberOfFrames(num);
  calculateLongestRunningTime();

  // re-check loop boundaries (if loop set at all)
  if(anim->getLoopInPoint()!=-1)
  {
    setLoopInPoint(anim->getLoopInPoint()+1);
    setLoopOutPoint(anim->getLoopOutPoint()+1);
  }

  updateInputs();
}

void QAvimatorWindow::easeInChanged(int change)
{
  bool ease=false;
  if(change==Qt::Checked) ease=true;

  Animation* anim=scene->getAnimation();
  anim->setEaseIn(currentPart,anim->getFrame(),ease);
}

void QAvimatorWindow::easeOutChanged(int change)
{
  bool ease=false;
  if(change==Qt::Checked) ease=true;

  Animation* anim=scene->getAnimation();
  anim->setEaseOut(currentPart,anim->getFrame(),ease);
}

// ------ Menu Action Slots (Callbacks) -----------

// Menu action: File / New
void QAvimatorWindow::fileNew()
{
  if(!clearOpenFiles()) return;
  emit clearProps();

  Animation* anim=new Animation(ui->animationView->getBVH());

  // set timeline animation first, because ...
  timeline->setAnimation(anim);
  // ... setting animation here will delete all old animations
  scene->setAnimation(anim);
  selectAnimation(anim);

  // add new animation to internal list
  animationIds.append(anim);
  calculateLongestRunningTime();
  // add new animation to combo box
  addToOpenFiles(m_untitledName);

  if(UserSettings::protectFirstFrame())
  {
//    qDebug("qavimator::fileNew(): adding loop points for protected frame 1 animation");
    // skip first frame, since it's protected anyway
    anim->setFrame(1);
    setCurrentFrame(1);
    setLoopInPoint(2);
  }
  else
  {
//    qDebug("qavimator::fileNew(): adding loop points for unprotected frame 1 animation");
    anim->setFrame(0);
    setCurrentFrame(0);
    setLoopInPoint(1);
  }
  setLoopOutPoint(anim->getNumberOfFrames());

  // show frame as unprotected
  emit protectFrame(false);
  protect=false;

  // FIXME: code duplication
  connect(anim,SIGNAL(currentFrame(int)),this,SLOT(setCurrentFrame(int)));

  ui->editPartCombo->setCurrentIndex(1);

  setPlaystate(PLAYSTATE_STOPPED);

  updateInputs();
  updateFps();

  enableAvatarRotation(false);
  enableAvatarPosition(true);
  enableProps(false);

  anim->setDirty(false);
}

QString QAvimatorWindow::selectFileToOpen(const QString& caption)
{
   //// For some unknown reason passing "this" locks up the OSX qavimator window. Possibly a QT4 bug, needs investigation
#ifdef __APPLE__
   QString file = QFileDialog::getOpenFileName(NULL, caption, UserSettings::lastPath(), m_animFilter);
#else
   QString file = QFileDialog::getOpenFileName(this, caption, UserSettings::lastPath(), m_animFilter);
#endif
  if(!file.isEmpty())
  {
    QFileInfo fileInfo(file);
    if(!fileInfo.exists())
    {
      QMessageBox::warning(this,QObject::tr("Load Animation File"),QObject::tr("<qt>Animation file not found:<br />%1</qt>").arg(file));
      file=QString::null;
    }
    else
      UserSettings::setLastPath(fileInfo.path());
  }

  return file;
}

// Menu action: File / Open ...
void QAvimatorWindow::fileOpen()
{
  fileOpen(QString::null);
}

void QAvimatorWindow::fileOpen(const QString& name)
{
  QString file=name;

  if(file.isEmpty())
    file=selectFileToOpen(tr("Select Animation File to Load"));

  if(!file.isEmpty())
  {
    if(!clearOpenFiles()) return;
    emit clearProps();
    fileAdd(file);
  }

  // update timeline and animation view with the currently selected body part
  // this helps to sync the visual selections
  partChoice();
}

// Menu action: File / Add New Animation ...
void QAvimatorWindow::fileAdd()
{
  fileAdd(QString::null);
}

void QAvimatorWindow::fileAdd(const QString& name)
{
  QString file=name;

  if(file.isEmpty())
    file=selectFileToOpen(tr("Select Animation File to Add"));

  if(!file.isEmpty())
  {
    // handling of non-existant file names
    if(!QFile::exists(file))
    {
      QMessageBox::warning(this,QObject::tr("Load Animation File"),QObject::tr("<qt>Animation file not found:<br />%1</qt>").arg(file));
      return;
    }
    addToOpenFiles(file);
    Animation* anim=new Animation(ui->animationView->getBVH(),file);
    animationIds.append(anim);
    calculateLongestRunningTime();

    setCurrentFile(file);

    scene->addAnimation(anim);
    timeline->setAnimation(anim);
    selectAnimation(anim);

//    qDebug("qavimator::fileAdd(): checking for loop points");
    // no loop in point? must be a BVH or an older avm. set a sane default
    if(anim->getLoopInPoint()==-1)
    {
//      qDebug("qavimator::fileAdd(): no loop points, adding new");
      // first set loop out point to avoid clamping of loop in point
      setLoopOutPoint(anim->getNumberOfFrames());

      if(UserSettings::protectFirstFrame())
      {
//        qDebug("qavimator::fileAdd(): adding loop points for protected frame 1 animation");
        anim->setFrame(1);
        setCurrentFrame(1);
        setLoopInPoint(2);
      }
      else
      {
//        qDebug("qavimator::fileAdd(): adding loop points for unprotected frame 1 animation");
        anim->setFrame(0);
        setCurrentFrame(0);
        setLoopInPoint(1);
      }
    }
    else
    {
//      qDebug("qavimator::fileAdd(): reading saved loop points");
      setLoopInPoint(anim->getLoopInPoint()+1);
      setLoopOutPoint(anim->getLoopOutPoint()+1);
    }

    // FIXME: code duplication
    connect(anim,SIGNAL(currentFrame(int)),this,SLOT(setCurrentFrame(int)));

    ui->animationView->selectPart(nodeMapping[ui->editPartCombo->currentIndex()]);
    updateInputs();
    updateFps();
    anim->setDirty(false);

// makeshift tool for new independant playback testing
// anim->setPlaystate(PLAYSTATE_LOOPING);

  }
}

// Menu Action: File / Save
void QAvimatorWindow::fileSave()
{
  if (currentFile == m_untitledName)
  {
    fileSaveAs();
  }
  else
  {
    scene->getAnimation()->saveBVH(currentFile);
  }
}

// Menu Action: File / Save As...
void QAvimatorWindow::fileSaveAs()
{
   //// For some unknown reason passing "this" locks up the OSX qavimator window. Possibly a QT4 bug, needs investigation
#ifdef __APPLE__
   QString file = QFileDialog::getSaveFileName(NULL, tr("Save Animation File"), currentFile, m_animFilter, 0, QFileDialog:: DontConfirmOverwrite);
#else
   QString file = QFileDialog::getSaveFileName(this, tr("Save Animation File"), currentFile, m_animFilter, 0, QFileDialog:: DontConfirmOverwrite);
#endif

  if(!file.isEmpty())
  {
    QFileInfo fileInfo(file);

    // make sure file has proper extension (either .bvh or .avm)
    QString extension=fileInfo.suffix().toLower();
    if(extension!="avm" && extension!="bvh")
      file+=".avm";

    // if the file didn't exist yet or the user accepted to overwrite it, save it.
    if(checkFileOverwrite(fileInfo))
    {
      setCurrentFile(file);
      UserSettings::setLastPath(fileInfo.path());
      scene->getAnimation()->saveBVH(file);
      // update animation selector combo box
      ui->selectAnimationCombo->setItemText(ui->selectAnimationCombo->currentIndex(),fileInfo.baseName());
      openFiles[ui->selectAnimationCombo->currentIndex()]=file;
      ui->fileExportForSecondLifeAction->setEnabled(true);
    }
  }
}

// Menu Action: File / Export For Second Life
void QAvimatorWindow::fileExportForSecondLife()
{
  // FIXME: think of a sensible thing to do when the animation has not been saved
  //        as .avm yet
  if (currentFile != m_untitledName)
  {
    QFileInfo fileInfo(currentFile);
    QString exportName=fileInfo.path()+"/"+fileInfo.completeBaseName()+".bvh";

    qDebug("qavimator::fileExportForSecondLife(): exporting animation as '%s'.",exportName.toLatin1().constData());
    scene->getAnimation()->saveBVH(exportName);
    QMessageBox::information(this,QObject::tr("Export for Second Life"),QObject::tr("Animation was exported for Second Life as:\n%1").arg(exportName));
  }
}

// Menu Action: File / Load Props...
void QAvimatorWindow::fileLoadProps()
{
   //// For some unknown reason passing "this" locks up the OSX qavimator window. Possibly a QT4 bug, needs investigation
#ifdef __APPLE__
   QString fileName = QFileDialog::getOpenFileName(nullptr, QString(), UserSettings::lastPath(), m_propFilter);
#else
   QString fileName = QFileDialog::getOpenFileName(this, QString(), UserSettings::lastPath(), m_propFilter);
#endif

  if(!fileName.isEmpty())
  {
    QFileInfo fileInfo(fileName);

    if(fileInfo.exists())
    {
      emit clearProps();
      QFile file(fileName);
      if(file.open(QIODevice::ReadOnly))
      {
        QString line;
        bool tooOldWarning=false;
        while(!file.atEnd())
        {
          qDebug("qavimator::fileLoadProps(): loading prop");

          line=file.readLine(2048);
          QStringList props=line.split(' ');

          // make sure we don't crash with an assert if the array is too short (e.g. attachment point missing)
          if(props.count()!=11)
          {
            // show this warning only once per .prp file
            if(!tooOldWarning)
            {
              qDebug("qavimator::fileLoadProps(): Prop file either too old or corrupted. Trying to recover.");
              tooOldWarning=true;
            }
            // append the necessary number of "0" to the array
            while(props.count()<11) props.append("0");
          }

          auto propType = static_cast<Prop::PropType>(props[0].toInt());
          auto xPosition = props[1].toDouble();
          auto yPosition = props[2].toDouble();
          auto zPosition = props[3].toDouble();
          auto xScale = props[4].toDouble();
          auto yScale = props[5].toDouble();
          auto zScale = props[6].toDouble();
          auto xRotation = props[7].toDouble();
          auto yRotation = props[8].toDouble();
          auto zRotation = props[9].toDouble();
          auto isAttached = props[10].toInt();

          // TODO Temporary hack for adding mesh as part of removing GLUT
          QSharedPointer<Mesh> mesh;

          switch (props[0].toInt())
          {
            case Prop::PropType::Box:
              mesh = cubeMesh;
              break;
            case Prop::PropType::Sphere:
              mesh = sphereMesh;
              break;
            case Prop::PropType::Cone:
              mesh = coneMesh;
              break;
            case Prop::PropType::Torus:
              mesh = torusMesh;
              break;
          }

          const Prop* prop = scene->addProp(propType, mesh,
                                            isAttached,
                                            xPosition, yPosition, zPosition,
                                            xScale, yScale, zScale,
                                            xRotation, yRotation, zRotation);
          if(prop)
          {
            ui->propNameCombo->addItem(prop->name());
            ui->propNameCombo->setCurrentIndex(ui->propNameCombo->count()-1);
            selectProp();
          }
        } // while
      }
    }
  }
}

// Menu Action: File / Save Props...
void QAvimatorWindow::fileSaveProps()
{
   //// For some unknown reason passing "this" locks up the OSX qavimator window. Possibly a QT4 bug, needs investigation
#ifdef __APPLE__
   QString fileName = QFileDialog::getSaveFileName(NULL, tr("Save Props"), currentFile, m_propFilter);
#else
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save Props"), currentFile, m_propFilter);
#endif

  if(!fileName.isEmpty())
  {
    QFileInfo fileInfo(fileName);
    // make sure file has proper extension (.prp)
    if(fileInfo.suffix().toLower()!="prp")
      fileName+=".prp";

    // check if file exists
    // Obsolete for Qt 4.x
    if(!checkFileOverwrite(fileInfo)) return;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
      for(int index=0;index<ui->propNameCombo->count();index++)
      {
        Prop* prop=scene->getPropByName(ui->propNameCombo->itemText(index));
        QStringList properties;
        properties.append(QString::number(prop->propType()));
        properties.append(QString::number(prop->xPosition()));
        properties.append(QString::number(prop->yPosition()));
        properties.append(QString::number(prop->zPosition()));
        properties.append(QString::number(prop->xScale()));
        properties.append(QString::number(prop->yScale()));
        properties.append(QString::number(prop->zScale()));
        properties.append(QString::number(prop->xRotation()));
        properties.append(QString::number(prop->yRotation()));
        properties.append(QString::number(prop->zRotation()));
        properties.append(QString::number(prop->isAttached()));
        QString line=properties.join(" ")+"\n";
        file.write(line.toLatin1().constData(),line.length());
      } // for
    }
  }
}

// Menu Action: File / Exit
void QAvimatorWindow::fileExit()
{
  if(!clearOpenFiles())
    return;

  UserSettings::setSkeleton(ui->optionsSkeletonAction->isChecked());
  UserSettings::setJointLimits(ui->optionsJointLimitsAction->isChecked());
  UserSettings::setProtectFirstFrame(ui->optionsProtectFirstFrameAction->isChecked());
  UserSettings::setShowTimelinePanel(ui->optionsShowTimelineAction->isChecked());

  UserSettings::setMainWindowWidth(size().width());
  UserSettings::setMainWindowHeight(size().height());

  UserSettings::writeSettings();

  // remove all widgets and close the main form
  qApp->exit(0);
}

// Menu Action: Edit / Cut
void QAvimatorWindow::editCut()
{
//  qDebug("qavimator::editCut()");
  scene->getAnimation()->cutFrame();
  frameDataValid=true;
  updateInputs();
}

// Menu Action: Edit / Copy
void QAvimatorWindow::editCopy()
{
  scene->getAnimation()->copyFrame();
  frameDataValid=true;
  updateInputs();
}

// Menu Action: Edit / Paste
void QAvimatorWindow::editPaste()
{
  if(frameDataValid)
  {
    scene->getAnimation()->pasteFrame();
    ui->animationView->repaint();
    updateInputs();
  }
}

// Menu Action: Edit / Paste
void QAvimatorWindow::toolsOptimizeBVH()
{
  scene->getAnimation()->optimize();
  updateInputs();
}

// Menu Action: Options / Skeleton
void QAvimatorWindow::setSkeleton(bool on)
{
  UserSettings::setSkeleton(on);
}

// Menu Action: Options / Loop
void QAvimatorWindow::setLoop(bool on)
{
  UserSettings::setLoop(on);

  // update play state
  if(playstate==PLAYSTATE_LOOPING)
    setPlaystate(PLAYSTATE_PLAYING);
  else
    // just update play button icon (in case we're stopped it changes from/to looping icon)
    setPlaystate(playstate);
}

// Menu Action: Options / Joint Limits
void QAvimatorWindow::setJointLimits(bool on)
{
  UserSettings::setJointLimits(on);
  Animation* anim=scene->getAnimation();
  if(anim)
  {
    ui->animationView->repaint();
    updateInputs();
  }
}

// Menu Action: Options / Protect First Frame
void QAvimatorWindow::setProtectFirstFrame(bool on)
{
  UserSettings::setProtectFirstFrame(on);
  if(on && ui->currentFrameSlider->value()==0) protect=true;
  else protect=false;

  emit protectFrame(protect);
  updateInputs();
}

// Menu Action: Options / Show Timeline
void QAvimatorWindow::showTimeline(bool on)
{
  if(on)
    ui->timelineView->show();
  else
    ui->timelineView->hide();

  // hack to get 3D view back in shape
  qApp->processEvents();
  QSize oldSize=ui->animationView->size();
  ui->animationView->resize(oldSize.width(),oldSize.height()-1);
  qApp->processEvents();
  ui->animationView->resize(oldSize);
}

// Menu Action: Options / Configure QAvimator
void QAvimatorWindow::configure()
{
  UserSettingsDialog* dialog=new UserSettingsDialog(this);
  connect(dialog,SIGNAL(configChanged()),this,SLOT(configChanged()));

  dialog->exec();

  delete dialog;
}

void QAvimatorWindow::configChanged()
{
  ui->animationView->repaint();
}

// Menu Action: Help / About ...
void QAvimatorWindow::helpAbout()
{
  QScopedPointer<AboutDialog> dialog(new AboutDialog(this));
  connect(dialog.data(), SIGNAL(checkUpdates()),
          &updateChecker, SLOT(checkUpdates()));
  connect(&updateChecker, SIGNAL(updateCheckFinished()),
          dialog.data(), SLOT(updateCheckFinished()));
  dialog->exec();
}

void QAvimatorWindow::helpWhatsNew()
{
  updateChecker.showNews();
}

// checks if a file already exists at the given path and displays a warning message
// returns true if it's ok to save/overwrite, else returns false
bool QAvimatorWindow::checkFileOverwrite(const QFileInfo& fileInfo)
{
  // get file info
  if(fileInfo.exists())
  {
    int answer=QMessageBox::question(this,tr("File Exists"),tr("A file with the name \"%1\" does already exist. Do you want to overwrite it?").arg(fileInfo.fileName()),QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton);
    if(answer==QMessageBox::No) return false;
  }
  return true;
}

void QAvimatorWindow::setX(float x)
{
  setSliderValue(ui->xRotationSlider,ui->xRotationEdit,x);
}

void QAvimatorWindow::setY(float y)
{
  setSliderValue(ui->yRotationSlider,ui->yRotationEdit,y);
}

void QAvimatorWindow::setZ(float z)
{
  setSliderValue(ui->zRotationSlider,ui->zRotationEdit,z);
}

float QAvimatorWindow::getX()
{
  return ui->xRotationSlider->value() / m_precision;
}

float QAvimatorWindow::getY()
{
  return ui->yRotationSlider->value() / m_precision;
}

float QAvimatorWindow::getZ()
{
  return ui->zRotationSlider->value() / m_precision;
}

void QAvimatorWindow::setXPos(float x)
{
  setSliderValue(ui->xPositionSlider,ui->xPositionEdit,x);
}

void QAvimatorWindow::setYPos(float y)
{
  setSliderValue(ui->yPositionSlider,ui->yPositionEdit,y);
}

void QAvimatorWindow::setZPos(float z)
{
  setSliderValue(ui->zPositionSlider,ui->zPositionEdit,z);
}

float QAvimatorWindow::getXPos()
{
  return ui->xPositionSlider->value() / m_precision;
}

float QAvimatorWindow::getYPos()
{
  return ui->yPositionSlider->value() / m_precision;
}

float QAvimatorWindow::getZPos()
{
  return ui->zPositionSlider->value() / m_precision;
}

void QAvimatorWindow::enableProps(bool state)
{
  ui->propPositionGroup->setEnabled(state);
  ui->propScaleGroup->setEnabled(state);
  ui->propRotationGroup->setEnabled(state);
  ui->attachToLabel->setEnabled(state);
  ui->attachToComboBox->setEnabled(state);
}

void QAvimatorWindow::enableAvatarRotation(bool state)
{
  ui->rotationGroupBox->setEnabled(state);
}

void QAvimatorWindow::enableAvatarPosition(bool state)
{
  ui->positionGroupBox->setEnabled(state);
}

void QAvimatorWindow::updatePropSpins(const QVector3D& position, const QVector3D& rotation, const QVector3D& scale)
{
  ui->propXPosSpin->blockSignals(true);
  ui->propYPosSpin->blockSignals(true);
  ui->propZPosSpin->blockSignals(true);

  ui->propXPosSpin->setValue(static_cast<int>(position.x()));
  ui->propYPosSpin->setValue(static_cast<int>(position.y()));
  ui->propZPosSpin->setValue(static_cast<int>(position.z()));

  ui->propXPosSpin->blockSignals(false);
  ui->propYPosSpin->blockSignals(false);
  ui->propZPosSpin->blockSignals(false);

  ui->propXRotSpin->blockSignals(true);
  ui->propYRotSpin->blockSignals(true);
  ui->propZRotSpin->blockSignals(true);

  ui->propXRotSpin->setValue((360 + static_cast<int>(rotation.x())) % 360);
  ui->propYRotSpin->setValue((360 + static_cast<int>(rotation.y())) % 360);
  ui->propZRotSpin->setValue((360 + static_cast<int>(rotation.z())) % 360);

  ui->propXRotSpin->blockSignals(false);
  ui->propYRotSpin->blockSignals(false);
  ui->propZRotSpin->blockSignals(false);

  ui->propXScaleSpin->blockSignals(true);
  ui->propYScaleSpin->blockSignals(true);
  ui->propZScaleSpin->blockSignals(true);

  ui->propXScaleSpin->setValue(static_cast<int>(scale.x()));
  ui->propYScaleSpin->setValue(static_cast<int>(scale.y()));
  ui->propZScaleSpin->setValue(static_cast<int>(scale.z()));

  ui->propXScaleSpin->blockSignals(false);
  ui->propYScaleSpin->blockSignals(false);
  ui->propZScaleSpin->blockSignals(false);
}

void QAvimatorWindow::onNewProp(const QString& propName)
{
  ui->propNameCombo->addItem(propName);
  ui->propNameCombo->setCurrentIndex(ui->propNameCombo->count() - 1);
  selectProp();
  ui->attachToComboBox->setCurrentIndex(0);
}

void QAvimatorWindow::onSelectProp(bool isSelected, int attachmentPoint)
{
  ui->propNameCombo->setEnabled(isSelected);
  ui->deletePropButton->setEnabled(isSelected);
  if (isSelected)
  {
    ui->attachToComboBox->setCurrentIndex(attachmentPoint);
  }
  ui->animationView->selectProp();
}

void QAvimatorWindow::onDeleteProp(const QString propName)
{
  for (int index = 0; index < ui->propNameCombo->count(); index++)
  {
    if (ui->propNameCombo->itemText(index) == propName)
    {
      ui->propNameCombo->removeItem(index);
      selectProp();
    }
  }
}

void QAvimatorWindow::onClearProps()
{
  ui->propNameCombo->clear();
}

void QAvimatorWindow::selectProp()
{
  emit selectProp(ui->propNameCombo->currentIndex());
}

// helper function to prevent feedback between the two widgets
void QAvimatorWindow::setSliderValue(QSlider* slider,QLineEdit* edit,float value)
{
  slider->blockSignals(true);
  edit->blockSignals(true);
  slider->setValue((int)(value * m_precision));
  edit->setText(QString::number(value));
  edit->blockSignals(false);
  slider->blockSignals(false);
}

void QAvimatorWindow::updateFps()
{
  int fps=scene->getAnimation()->fps();

  // guard against division by zero
  if(fps)
  {
    // don't send FPS change back to Animation object
    ui->framesSpin->blockSignals(true);
    ui->fpsSpin->setValue(fps);
    // re-enable FPS signal
    ui->framesSpin->blockSignals(false);
  }
}

// Adds a file to the open files list, and adds the entry in the combo box
void QAvimatorWindow::addToOpenFiles(const QString& fileName)
{
    openFiles.append(fileName);

    QString fixedName=fileName;
    QRegExp pattern(".*/");
    fixedName.remove(pattern);
    pattern.setPattern("(\\.bvh|\\.avm)");
    fixedName.remove(pattern);

    ui->selectAnimationCombo->addItem(fixedName);
}

void QAvimatorWindow::removeFromOpenFiles(unsigned int which)
{
  if(which>= (unsigned int) openFiles.count()) return;
  openFiles.removeAt(which);
  ui->selectAnimationCombo->removeItem(which);
}

// empty out the open files list
bool QAvimatorWindow::clearOpenFiles()
{
  for(unsigned int index=0;index< (unsigned int) animationIds.count();index++)
  {
    if(animationIds.at(index)->dirty())
    {
      int answer=QMessageBox::question(this,tr("Unsaved Changes"),tr("There are some unsaved changes. Are you sure you want to continue and lose all unsaved data?"),QMessageBox::Yes,QMessageBox::No,QMessageBox::NoButton);
      if(answer==QMessageBox::No)
        return false;
      else
        break;
    }
  }

  timeline->setAnimation(0);
  scene->clear();
  currentPart = NULL;
  openFiles.clear();
  ui->selectAnimationCombo->clear();
  animationIds.clear();
  setCurrentFile(m_untitledName);
  longestRunningTime=0.0;

  return true;
}

// convenience function to set window title in a defined way
void QAvimatorWindow::setCurrentFile(const QString& fileName)
{
  currentFile=fileName;
  setWindowTitle("qavimator ["+currentFile+"]");
}

// this slot gets called from Animation::setFrame(int)
void QAvimatorWindow::setCurrentFrame(int frame)
{
  // make sure current frame is only updated when no animation is playing (manual change,
  // program startup) or that only the currently selected animation updates the frame
  // position, so we don't have jumping back and forth while playing multiple animations
  if(playstate==PLAYSTATE_STOPPED || sender()==animationIds.at(ui->selectAnimationCombo->currentIndex()))
  {
    ui->currentFrameSlider->blockSignals(true);
    ui->currentFrameSlider->setValue(frame);
    ui->currentFrameSlider->blockSignals(false);
    ui->currentFrameLabel->setText(QString::number(frame+1));

    timeline->setCurrentFrame(frame);
//  animationView->setFrame(frame);
    // check if we are at the first frame and if it's protected
    if(frame==0 && UserSettings::protectFirstFrame()) protect=true;
    else protect=false;
    emit protectFrame(protect);
    updateInputs();
    updateKeyBtn();
  }
}

// gets called by selecting an animation from the animation combo box
void QAvimatorWindow::animationChanged(int which)
{
  qDebug("qavimator::animationChanged(%d)",which);

  // safety to check if "which" is out of bounds of loaded animations
  if(which>=openFiles.count()) return;

  // get animation pointer
  Animation* anim=animationIds.at(which);
  // select animation (will also update combo box, but better than duplicate code)
  selectAnimation(anim);
  ui->animationView->selectPart(0);
  currentPart = nullptr;
}

// gets called from AnimationView::animationSelected()
void QAvimatorWindow::selectAnimation(Animation* animation)
{
  // find animation index in list of open files
  for(unsigned int index=0;index< (unsigned int) animationIds.count();index++)
  {
    // index found
    if(animationIds.at(index)==animation)
    {
      // prevent signal looping
      ui->animationView->blockSignals(true);
      scene->blockSignals(true);
      // update animation combo box
      ui->selectAnimationCombo->setCurrentIndex(index);
      // update window title
      setCurrentFile(openFiles.at(index));
      // update animation view (might already be active, depending on how this function was called)
      scene->selectAnimation(index);
      // re-enable signals
      ui->animationView->blockSignals(false);
      scene->blockSignals(false);
    }
  } // for

  // update avatar figure combo box
  int figure=0;
  if(animation->getFigureType()==Animation::FIGURE_MALE) figure=1;
  ui->figureCombo->setCurrentIndex(figure);

  // update timeline
  timeline->setAnimation(animation);
  updateInputs();

  // enable export to second life if current file name is not the default untitled name
  ui->fileExportForSecondLifeAction->setEnabled(currentFile != m_untitledName);
}

// set loop in point (user view, so always +1)
void QAvimatorWindow::setLoopInPoint(int inFrame)
{
  Animation* anim=scene->getAnimation();

  // FIXME: Temporary fix for a seg fault caused
  //  by adding then deleting a prop immediately
  //  before closing the QAvimator window
  if (!anim) return;

  int numOfFrames=anim->getNumberOfFrames();
  int outFrame=anim->getLoopOutPoint();
  qDebug("qavimator::setLoopInPoint(%d) (%d frames)",inFrame,numOfFrames);

  if(inFrame>numOfFrames) inFrame=numOfFrames;
  if(inFrame>outFrame) inFrame=outFrame+1;
  if(inFrame<1) inFrame=1;

  anim->setLoopInPoint(inFrame-1);

  ui->loopInSpinBox->setValue(inFrame);

  ui->loopInPercentLabel->setText(QString("(%1%)").arg(inFrame*100/numOfFrames));
}

// set loop out point (user view, so always +1)
void QAvimatorWindow::setLoopOutPoint(int outFrame)
{
  Animation* anim=scene->getAnimation();
  int numOfFrames=anim->getNumberOfFrames();
  int inFrame=anim->getLoopInPoint();
  qDebug("qavimator::setLoopOutPoint(%d) (%d frames)",outFrame,numOfFrames);

  if(outFrame>numOfFrames) outFrame=numOfFrames;
  if((outFrame-1)<inFrame) outFrame=inFrame+1;
  if(outFrame<1) outFrame=1;

  anim->setLoopOutPoint(outFrame-1);

  ui->loopOutSpinBox->setValue(outFrame);

  ui->loopOutPercentLabel->setText(QString("(%1%)").arg(outFrame*100/numOfFrames));
}

void QAvimatorWindow::setPlaystate(PlayState state)
{
//  qDebug("qavimator::setPlaystate(): setting playstate %d",(int) state);
  // set state
  playstate=state;

  // set play button icons according to play state
  if(state==PLAYSTATE_STOPPED)
    ui->playButton->setIcon(UserSettings::loop() ? loopIcon : playIcon);
  else if(state==PLAYSTATE_LOOPING)
    ui->playButton->setIcon(playIcon);
  else if(state==PLAYSTATE_PLAYING)
    ui->playButton->setIcon(stopIcon);
  else
    qDebug("qavimator::setPlaystate(): unknown playstate %d",(int) state);
}

// prevent closing of main window if there are unsaved changes
void QAvimatorWindow::closeEvent(QCloseEvent* event)
{
  if(!clearOpenFiles())
    event->ignore();
  else
    event->accept();
}

// calculates the longest running time of all animations
double QAvimatorWindow::calculateLongestRunningTime()
{
  qDebug("qavimator::calculateLongestRunningTime()");
  longestRunningTime=0.0;
  for(unsigned int index=0;index< (unsigned int) animationIds.count();index++)
  {
    Animation* anim=animationIds.at(index);
    double time=anim->getNumberOfFrames()/((double) anim->fps());
    if(time>longestRunningTime) longestRunningTime=time;
  }
  qDebug("qavimator::calculateLongestRunningTime(): longestRunningTime now: %f seconds",longestRunningTime);
  return longestRunningTime;
}

// -------------------------------------------------------------------------
// autoconnection from designer UI

// ------- Menu Action Slots --------

void QAvimatorWindow::on_fileNewAction_triggered()
{
  fileNew();
}

void QAvimatorWindow::on_fileOpenAction_triggered()
{
  fileOpen();
}

void QAvimatorWindow::on_fileAddAction_triggered()
{
  fileAdd();
}

void QAvimatorWindow::on_fileSaveAction_triggered()
{
  fileSave();
}

void QAvimatorWindow::on_fileSaveAsAction_triggered()
{
  fileSaveAs();
}

void QAvimatorWindow::on_fileExportForSecondLifeAction_triggered()
{
  fileExportForSecondLife();
}

void QAvimatorWindow::on_fileLoadPropsAction_triggered()
{
  fileLoadProps();
}

void QAvimatorWindow::on_fileSavePropsAction_triggered()
{
  fileSaveProps();
}

void QAvimatorWindow::on_fileExitAction_triggered()
{
  fileExit();
}

void QAvimatorWindow::on_editCutAction_triggered()
{
  editCut();
}

void QAvimatorWindow::on_editCopyAction_triggered()
{
  editCopy();
}

void QAvimatorWindow::on_editPasteAction_triggered()
{
  editPaste();
}

void QAvimatorWindow::on_toolsOptimizeBVHAction_triggered()
{
  toolsOptimizeBVH();
}

void QAvimatorWindow::on_toolsMirrorAction_triggered()
{
  Animation* anim=scene->getAnimation();
  anim->mirror(currentPart);
  updateInputs();
}

void QAvimatorWindow::on_optionsSkeletonAction_toggled(bool on)
{
  setSkeleton(on);
}

void QAvimatorWindow::on_optionsJointLimitsAction_toggled(bool on)
{
  setJointLimits(on);
}

void QAvimatorWindow::on_optionsLoopAction_toggled(bool on)
{
  setLoop(on);
}

void QAvimatorWindow::on_optionsProtectFirstFrameAction_toggled(bool on)
{
  setProtectFirstFrame(on);
}

void QAvimatorWindow::on_optionsShowTimelineAction_toggled(bool on)
{
  showTimeline(on);
}

void QAvimatorWindow::on_optionsConfigureQAvimatorAction_triggered()
{
  configure();
}

void QAvimatorWindow::on_helpAboutAction_triggered()
{
  helpAbout();
}

void QAvimatorWindow::on_helpWhatsNewAction_triggered()
{
  helpWhatsNew();
}

// ------- Additional Toolbar Element Slots --------

void QAvimatorWindow::on_resetCameraAction_triggered()
{
  emit resetCamera();
}

// ------- UI Element Slots --------

void QAvimatorWindow::on_selectAnimationCombo_activated(int which)
{
  animationChanged(which);
}

void QAvimatorWindow::on_figureCombo_activated(int newShape)
{
  setAvatarShape(newShape);
}

void QAvimatorWindow::on_scaleSpin_valueChanged(int newValue)
{
  setAvatarScale(newValue);
}

void QAvimatorWindow::on_editPartCombo_activated(int)
{
  partChoice();
}

void QAvimatorWindow::on_xRotationEdit_editingFinished()
{
  rotationValue();
}

void QAvimatorWindow::on_xRotationSlider_valueChanged(int)
{
  rotationSlider(sender());
}

void QAvimatorWindow::on_yRotationEdit_editingFinished()
{
  rotationValue();
}

void QAvimatorWindow::on_yRotationSlider_valueChanged(int)
{
  rotationSlider(sender());
}

void QAvimatorWindow::on_zRotationEdit_editingFinished()
{
  rotationValue();
}

void QAvimatorWindow::on_zRotationSlider_valueChanged(int)
{
  rotationSlider(sender());
}

void QAvimatorWindow::on_xPositionEdit_editingFinished()
{
  positionValue();
}

void QAvimatorWindow::on_xPositionSlider_valueChanged(int)
{
  positionSlider(sender());
}

void QAvimatorWindow::on_yPositionEdit_editingFinished()
{
  positionValue();
}

void QAvimatorWindow::on_yPositionSlider_valueChanged(int)
{
  positionSlider(sender());
}

void QAvimatorWindow::on_zPositionEdit_editingFinished()
{
  positionValue();
}

void QAvimatorWindow::on_zPositionSlider_valueChanged(int)
{
  positionSlider(sender());
}

void QAvimatorWindow::on_easeInCheck_stateChanged(int newState)
{
  easeInChanged(newState);
}

void QAvimatorWindow::on_easeOutCheck_stateChanged(int newState)
{
  easeOutChanged(newState);
}

void QAvimatorWindow::on_newBoxPropButton_clicked()
{
  emit newProp(Prop::Box, cubeMesh);
}

void QAvimatorWindow::on_newSpherePropButton_clicked()
{
  emit newProp(Prop::Sphere, sphereMesh);
}

void QAvimatorWindow::on_newConePropButton_clicked()
{
  emit newProp(Prop::Cone, coneMesh);
}

void QAvimatorWindow::on_newTorusPropButton_clicked()
{
  emit newProp(Prop::Torus, torusMesh);
}

void QAvimatorWindow::on_propNameCombo_activated(int index)
{
  selectProp();
}

void QAvimatorWindow::on_deletePropButton_clicked()
{
  emit deleteProp();
}

void QAvimatorWindow::on_attachToComboBox_activated(int attachmentPoint)
{
  // FIXME: find better solution for filtering endpoint for joints
  if (ui->attachToComboBox->currentText() == "-")
  {
    attachmentPoint = 0;
  }
  emit attachProp(attachmentPoint);
}

void QAvimatorWindow::on_propXPosSpin_valueChanged(int)
{
  emit propPositionChanged(ui->propXPosSpin->value(), ui->propYPosSpin->value(), ui->propZPosSpin->value());
}

void QAvimatorWindow::on_propYPosSpin_valueChanged(int)
{
  emit propPositionChanged(ui->propXPosSpin->value(), ui->propYPosSpin->value(), ui->propZPosSpin->value());
}

void QAvimatorWindow::on_propZPosSpin_valueChanged(int)
{
  emit propPositionChanged(ui->propXPosSpin->value(), ui->propYPosSpin->value(), ui->propZPosSpin->value());
}

void QAvimatorWindow::on_propXScaleSpin_valueChanged(int)
{
  emit propScaleChanged(ui->propXScaleSpin->value(), ui->propYScaleSpin->value(), ui->propZScaleSpin->value());
}

void QAvimatorWindow::on_propYScaleSpin_valueChanged(int)
{
  emit propScaleChanged(ui->propXScaleSpin->value(), ui->propYScaleSpin->value(), ui->propZScaleSpin->value());
}

void QAvimatorWindow::on_propZScaleSpin_valueChanged(int)
{
  emit propScaleChanged(ui->propXScaleSpin->value(), ui->propYScaleSpin->value(), ui->propZScaleSpin->value());
}

void QAvimatorWindow::on_propXRotSpin_valueChanged(int)
{
  emit propRotationChanged(ui->propXRotSpin->value(), ui->propYRotSpin->value(), ui->propZRotSpin->value());
}

void QAvimatorWindow::on_propYRotSpin_valueChanged(int)
{
  emit propRotationChanged(ui->propXRotSpin->value(), ui->propYRotSpin->value(), ui->propZRotSpin->value());
}

void QAvimatorWindow::on_propZRotSpin_valueChanged(int)
{
  emit propRotationChanged(ui->propXRotSpin->value(), ui->propYRotSpin->value(), ui->propZRotSpin->value());
}

void QAvimatorWindow::on_currentFrameSlider_valueChanged(int newValue)
{
  frameSlider(newValue);
}

void QAvimatorWindow::on_playButton_clicked()
{
  nextPlaystate();
}

void QAvimatorWindow::on_keyframeButton_toggled(bool on)
{
  qDebug("on_keyframeButton_toggled(%d)",(int) on);
  scene->getAnimation()->toggleKeyFrame(currentPart); // (animationView->getSelectedPart());
  ui->animationView->repaint();
}

void QAvimatorWindow::on_loopInSpinBox_editingFinished()
{
  setLoopInPoint(ui->loopInSpinBox->value());
}

void QAvimatorWindow::on_loopOutSpinBox_editingFinished()
{
  setLoopOutPoint(ui->loopOutSpinBox->value());
}

void QAvimatorWindow::on_framesSpin_valueChanged(int newValue)
{
  numFramesChanged(newValue);
}

void QAvimatorWindow::on_fpsSpin_valueChanged(int newValue)
{
  setFPS(newValue);
}

// end autoconnection from designer UI
// -------------------------------------------------------------------------
