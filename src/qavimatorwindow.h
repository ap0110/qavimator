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

/*
 * features added by Darkside Eldrich
 */

#ifndef QAVIMATORWINDOW_H
#define QAVIMATORWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "playstate.h"
#include "prop.h"
#include "updatechecker.h"

class QCloseEvent;
class QFileInfo;
class QLineEdit;
class QSlider;

class Animation;
class BVHNode;
class QAvimator;
struct RotationLimits;
class Scene;
class Timeline;

namespace Ui {
  class QAvimatorWindow;
}

class QAvimatorWindow : public QMainWindow
{
  Q_OBJECT

  public:
    QAvimatorWindow();
    ~QAvimatorWindow();

    void queueAfterShow();

  signals:
    void enableEaseInOut(bool state);
    void resetCamera();
    void protectFrame(bool state);
    void newProp(Prop::PropType, QSharedPointer<Mesh> mesh);
    void selectProp(int index);
    void attachProp(int attachmentPoint);
    void propPositionChanged(double xPosition, double yPosition, double zPosition);
    void propScaleChanged(double xScale, double yScale, double zScale);
    void propRotationChanged(double xRotation, double yRotation, double zRotation);
    void deleteProp();
    void clearProps();

  protected slots:
    void afterShow();

    void configChanged();

    void partClicked(BVHNode* node, QVector3D rotation, RotationLimits rotLimits, QVector3D position);
    void partDragged(BVHNode* node,double changeX,double changeY,double changeZ);
    void propClicked(Prop* prop);
    void propDragged(Prop* prop,double x,double y,double z);
    void propScaled(Prop* prop,double x,double y,double z);
    void propRotated(Prop* prop,double x,double y,double z);
    void backgroundClicked();

    void frameTimeout();

    void setCurrentFrame(int frame);

    void selectAnimation(Animation* animation);

    // autoconnection from designer UI

    // ------- Menu Action Slots --------
    void on_fileNewAction_triggered();
    void on_fileOpenAction_triggered();
    void on_fileAddAction_triggered();
    void on_fileSaveAction_triggered();
    void on_fileSaveAsAction_triggered();
    void on_fileExportForSecondLifeAction_triggered();
    void on_fileLoadPropsAction_triggered();
    void on_fileSavePropsAction_triggered();
    void on_fileExitAction_triggered();

    void on_editCutAction_triggered();
    void on_editCopyAction_triggered();
    void on_editPasteAction_triggered();

    void on_toolsOptimizeBVHAction_triggered();
    void on_toolsMirrorAction_triggered();

    void on_optionsSkeletonAction_toggled(bool on);
    void on_optionsJointLimitsAction_toggled(bool on);
    void on_optionsLoopAction_toggled(bool on);
    void on_optionsProtectFirstFrameAction_toggled(bool on);
    void on_optionsShowTimelineAction_toggled(bool on);
    void on_optionsConfigureQAvimatorAction_triggered();

    void on_helpAboutAction_triggered();
    void on_helpWhatsNewAction_triggered();

    // ------- Additional Toolbar Element Slots -------

    void on_resetCameraAction_triggered();

    // ------- UI Element Slots --------
    void on_selectAnimationCombo_activated(int);
    void on_figureCombo_activated(int);
    void on_scaleSpin_valueChanged(int newValue);
    void on_editPartCombo_activated(int);
    void on_xRotationEdit_editingFinished();
    void on_xRotationSlider_valueChanged(int);
    void on_yRotationEdit_editingFinished();
    void on_yRotationSlider_valueChanged(int);
    void on_zRotationEdit_editingFinished();
    void on_zRotationSlider_valueChanged(int);
    void on_xPositionEdit_editingFinished();
    void on_xPositionSlider_valueChanged(int);
    void on_yPositionEdit_editingFinished();
    void on_yPositionSlider_valueChanged(int);
    void on_zPositionEdit_editingFinished();
    void on_zPositionSlider_valueChanged(int);
    void on_easeInCheck_stateChanged(int newState);
    void on_easeOutCheck_stateChanged(int newState);

    void on_newBoxPropButton_clicked();
    void on_newSpherePropButton_clicked();
    void on_newConePropButton_clicked();
    void on_newTorusPropButton_clicked();
    void on_propNameCombo_activated(int index);
    void on_deletePropButton_clicked();
    void on_attachToComboBox_activated(int attachmentPoint);
    void on_propXPosSpin_valueChanged(int);
    void on_propYPosSpin_valueChanged(int);
    void on_propZPosSpin_valueChanged(int);
    void on_propXScaleSpin_valueChanged(int);
    void on_propYScaleSpin_valueChanged(int);
    void on_propZScaleSpin_valueChanged(int);
    void on_propXRotSpin_valueChanged(int);
    void on_propYRotSpin_valueChanged(int);
    void on_propZRotSpin_valueChanged(int);

    void on_currentFrameSlider_valueChanged(int newValue);
    void on_playButton_clicked();
    void on_keyframeButton_toggled(bool on);
    void on_loopInSpinBox_editingFinished();
    void on_loopOutSpinBox_editingFinished();
    void on_framesSpin_valueChanged(int num);
    void on_fpsSpin_valueChanged(int num);
    // end autoconnection from designer UI

  protected:
    // prevent closing of main window if there are unsaved changes
    virtual void closeEvent(QCloseEvent* event);

    void fileNew();
    void fileOpen();
    void fileOpen(const QString& fileName);
    // "add" a new file without clearing the old one(s)
    void fileAdd();
    void fileAdd(const QString& fileName);
    void fileSave();
    void fileSaveAs();
    void fileExportForSecondLife();
    void fileLoadProps();
    void fileSaveProps();
    void fileExit();

    void editCut();
    void editCopy();
    void editPaste();

    void toolsOptimizeBVH();
    void toolsMirror();

    void setSkeleton(bool on);
    void setJointLimits(bool on);
    void setLoop(bool on);
    void setProtectFirstFrame(bool on);
    void showTimeline(bool state);
    void configure();

    void helpAbout();
    void helpWhatsNew();

    void animationChanged(int which);
    void setAvatarShape(int shape);
    void setAvatarScale(int percent);
    void partChoice();
    void rotationValue();
    void rotationSlider(const QObject* slider);
    void positionValue();
    void positionSlider(const QObject* slider);

    void easeInChanged(int change);
    void easeOutChanged(int change);

    void frameSlider(int position);
    void nextPlaystate();
    void setLoopInPoint(int inFrame);
    void setLoopOutPoint(int outFrame);
    void numFramesChanged(int num);
    void setFPS(int fps);

    void setSliderValue(QSlider* slider,QLineEdit* edit,float value);

    QString selectFileToOpen(const QString& caption);
    void addToOpenFiles(const QString& fileName);
    void removeFromOpenFiles(unsigned int which);
    bool clearOpenFiles();

    void setPlaystate(PlayState state);

    bool checkFileOverwrite(const QFileInfo& fileInfo);
    void setCurrentFile(const QString& fileName);
    void enableInputs(bool state);

    void updateFps();
    void updateKeyBtn();
    void updateInputs();

    // calculates the longest running time of all loaded animations, returns it
    // and stores it in longestRunningTime member variable
    double calculateLongestRunningTime();

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    float getX();
    float getY();
    float getZ();

    void setXPos(float x);
    void setYPos(float y);
    void setZPos(float z);

    float getXPos();
    float getYPos();
    float getZPos();

    QString currentFile;
    QStringList openFiles;
    QTimer timer;
    // list of animation ids mapped to combo box indexes
    QList<Animation*> animationIds;

    // mapping of combo box indexes to node ids
    QList<int> nodeMapping;

    BVHNode* currentPart;

    Timeline* timeline;
    // icons for play button
    QIcon playIcon;
    QIcon loopIcon;
    QIcon stopIcon;

    // holds the current playing status
    PlayState playstate;

    bool frameDataValid;
    // will be true if a frame is protected
    bool protect;

    // holds the longest running time of all currently opened animations
    double longestRunningTime;

  private slots:
    void enableProps(bool state);
    void enableAvatarRotation(bool state);
    void enableAvatarPosition(bool state);
    void updatePropSpins(const QVector3D& position, const QVector3D& rotation, const QVector3D& scale);
    void onNewProp(const QString& propName);
    void onSelectProp(bool isSelected, int attachmentPoint);
    void onDeleteProp(const QString propName);
    void onClearProps();


  private:
    void selectProp();

    QScopedPointer<Ui::QAvimatorWindow> ui;

    // TODO Temporary pointer while logic is moved around.
    //  Eventually, QAvimator should own QAvimatorWindow
    QScopedPointer<QAvimator> m_qavimator;

    Scene* scene;

    UpdateChecker updateChecker;

    QSharedPointer<Mesh> cubeMesh;
    QSharedPointer<Mesh> sphereMesh;
    QSharedPointer<Mesh> coneMesh;
    QSharedPointer<Mesh> torusMesh;

    const QString m_animFilter;
    const QString m_propFilter;
    const QString m_untitledName;
    const int m_precision;
};

#endif
