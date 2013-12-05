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

#include <iostream>

#include "settings.h"

bool Settings::m_loop=true;
bool Settings::m_skeleton=false;
bool Settings::m_jointLimits=true;
bool Settings::m_protectFirstFrame=true;
bool Settings::m_showTimelinePanel=true;

int Settings::m_mainWindowWidth=850;
int Settings::m_mainWindowHeight=600;

QString Settings::m_lastPath=QString::null;

bool Settings::m_fog=true;
int  Settings::m_floorTranslucency=33;

bool Settings::m_easeIn=false;
bool Settings::m_easeOut=false;

int Settings::m_figure=0;

Settings::Settings()
{
  // should never be accessed
  qWarning("Constructor of static class \"Settings\" was called");
}

Settings::~Settings()
{
  // should never be accessed
  qWarning("Deconstructor of static class \"Settings\" was called");
}

void Settings::setLoop(bool value)               { m_loop=value; }
bool Settings::loop()                            { return m_loop; }
void Settings::setSkeleton(bool value)           { m_skeleton=value; }
bool Settings::skeleton()                        { return m_skeleton; }
void Settings::setJointLimits(bool value)        { m_jointLimits=value; }
bool Settings::jointLimits()                     { return m_jointLimits; }
void Settings::setProtectFirstFrame(bool value)  { m_protectFirstFrame=value; }
bool Settings::protectFirstFrame()               { return m_protectFirstFrame; }
void Settings::setShowTimelinePanel(bool value)  { m_showTimelinePanel=value; }
bool Settings::showTimelinePanel()               { return m_showTimelinePanel; }

void Settings::setMainWindowWidth(int value)     { m_mainWindowWidth=value; }
int Settings::mainWindowWidth()                  { return m_mainWindowWidth; }
void Settings::setMainWindowHeight(int value)    { m_mainWindowHeight=value; }
int Settings::mainWindowHeight()                 { return m_mainWindowHeight; }

void Settings::setLastPath(QString value)        { m_lastPath=value; }
QString Settings::lastPath()                     { return m_lastPath; }

void Settings::setFog(bool value)                { m_fog=value; }
bool Settings::fog()                             { return m_fog; }
void Settings::setFloorTranslucency(int value)   { m_floorTranslucency=value; }
int  Settings::floorTranslucency()               { return m_floorTranslucency; }

void Settings::setEaseIn(bool value)             { m_easeIn=value; }
bool Settings::easeIn()                          { return m_easeIn; }
void Settings::setEaseOut(bool value)            { m_easeOut=value; }
bool Settings::easeOut()                         { return m_easeOut; }

void Settings::setFigure(int value)              { m_figure=value; }
int Settings::figure()                           { return m_figure; }

void Settings::readSettings()
{
  QSettings settings;
  settings.beginGroup("/qavimator");

  bool settingsFound=settings.value("/settings").toBool();
  if(settingsFound)
  {
    Settings::setLoop(settings.value("/loop").toBool());
    Settings::setSkeleton(settings.value("/skeleton").toBool());
    Settings::setJointLimits(settings.value("/joint_limits").toBool());
    Settings::setProtectFirstFrame(settings.value("/protect_first_frame").toBool());
    Settings::setShowTimelinePanel(settings.value("/show_timeline").toBool());

    Settings::setMainWindowWidth(settings.value("/mainwindow_width").toInt());
    Settings::setMainWindowHeight(settings.value("/mainwindow_height").toInt());

    Settings::setLastPath(settings.value("/last_path").toString());

    // OpenGL settings
    Settings::setFog(settings.value("/fog").toBool());
    Settings::setFloorTranslucency(settings.value("/floor_translucency").toInt());

    // settings for ease in/ease outFrame
    Settings::setEaseIn(settings.value("/ease_in").toBool());
    Settings::setEaseOut(settings.value("/ease_out").toBool());

    Settings::setFigure(settings.value("/figure").toInt());
  }
  settings.endGroup();
}

void Settings::writeSettings()
{
  QSettings settings;
  settings.beginGroup("/qavimator");

  // make sure we know next time, that there actually was a settings file
  settings.setValue("/settings",true);

  settings.setValue("/loop",Settings::loop());
  settings.setValue("/skeleton",Settings::skeleton());
  settings.setValue("/joint_limits",Settings::jointLimits());
  settings.setValue("/protect_first_frame",Settings::protectFirstFrame());
  settings.setValue("/show_timeline",Settings::showTimelinePanel());

  settings.setValue("/figure",Settings::figure());
  settings.setValue("/mainwindow_width",Settings::mainWindowWidth());
  settings.setValue("/mainwindow_height",Settings::mainWindowHeight());

  settings.setValue("/last_path",Settings::lastPath());

  // OpenGL settings
  settings.setValue("/fog",Settings::fog());
  settings.setValue("/floor_translucency",Settings::floorTranslucency());

  // settings for ease in/ease outFrame
  settings.setValue("/ease_in",Settings::easeIn());
  settings.setValue("/ease_out",Settings::easeOut());

  settings.endGroup();
}
