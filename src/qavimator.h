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


#ifndef QAVIMATOR_H
#define QAVIMATOR_H

#include <qtimer.h>

#define UNTITLED_NAME "Untitled.avm"
#define PLAY_IMAGE "data/play.png"
#define PAUSE_IMAGE "data/pause.png"
#define KEY_IMAGE "data/key.png"
#define NOKEY_IMAGE "data/nokey.png"

#include "mainapplicationform.h"

class qavimator : public MainApplicationForm
{
  Q_OBJECT

  public:
    qavimator();
    ~qavimator();

    double frameData[512];

  signals:
    void enableRotation(bool state);
    void enablePosition(bool state);

  protected:
    void setSliderValue(QSlider* slider,QLineEdit* edit,float value);

  protected slots:
    void cb_AnimationView();

    void cb_PartChoice();
    void cb_RotRoller(int dummy);
    void cb_RotValue();
    void cb_PosRoller(int dummy);
    void cb_PosValue();

    void cb_timeout();
    void cb_PlayBtn();
    void cb_fpsValue(int fps);
    void cb_FrameSlider(int position);

    void figureChanged(int shape);
    void numFramesChanged(int num);
    void keyframeButtonToggled(bool on);

    // ------- Menu Action Slots (Callbacks) --------
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExit();

    void editCopy();
    void editPaste();

    void optionsSkeleton(bool on);
    void optionsJointLimits(bool on);
    void optionsLoop(bool on);

  protected:
    void setCurrentFile(const QString& fileName);
    void enableInputs(bool state);
    void updateFps();

    void updateKeyBtn();
    void updateInputs();

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
    QTimer timer;

    bool playing;
    bool loop;
    bool frameDataValid;
};

#endif


/*

#include <string.h>
#include <FL/Fl_PNG_Image.h>
#include "AnimationView.h"
#include "Main.h"

*/
