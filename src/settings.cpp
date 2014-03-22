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

#include "metadata.h"

#include "settings.h"

QString Settings::m_organizationName = "QAvimator Team";
QString Settings::m_organizationDomain = "qavimator.org";
QString Settings::m_applicationName = QString("QAvimator%1").arg(Metadata::updateChannelSuffix());

QString Settings::m_settingsGroup = "settings";
QString Settings::m_versionGroup = "1.0";

bool Settings::m_loop = true;
QString Settings::m_loopKey = "loop";
bool Settings::m_skeleton = false;
QString Settings::m_skeletonKey = "skeleton";
bool Settings::m_jointLimits = true;
QString Settings::m_jointLimitsKey = "joint_limits";
bool Settings::m_protectFirstFrame = true;
QString Settings::m_protectFirstFrameKey = "protect_first_frame";
bool Settings::m_showTimelinePanel = true;
QString Settings::m_showTimelinePanelKey = "show_timeline_panel";

int Settings::m_mainWindowWidth = 850;
QString Settings::m_mainWindowWidthKey = "main_window_width";
int Settings::m_mainWindowHeight = 600;
QString Settings::m_mainWindowHeightKey = "main_window_height";

QString Settings::m_lastPath = QString::null;
QString Settings::m_lastPathKey = "last_path";

bool Settings::m_fog = true;
QString Settings::m_fogKey = "fog";
int  Settings::m_floorTranslucency = 33;
QString  Settings::m_floorTranslucencyKey = "floor_translucency";

bool Settings::m_easeIn = false;
QString Settings::m_easeInKey = "ease_in";
bool Settings::m_easeOut = false;
QString Settings::m_easeOutKey = "ease_out";


QString Settings::organizationName()
{
  return m_organizationName;
}

QString Settings::organizationDomain()
{
  return m_organizationDomain;
}

QString Settings::applicationName()
{
  return m_applicationName;
}

void Settings::setLoop(bool on)               { m_loop = on; }
bool Settings::loop()                            { return m_loop; }
void Settings::setSkeleton(bool on)           { m_skeleton = on; }
bool Settings::skeleton()                        { return m_skeleton; }
void Settings::setJointLimits(bool on)        { m_jointLimits = on; }
bool Settings::jointLimits()                     { return m_jointLimits; }
void Settings::setProtectFirstFrame(bool on)  { m_protectFirstFrame = on; }
bool Settings::protectFirstFrame()               { return m_protectFirstFrame; }
void Settings::setShowTimelinePanel(bool on)  { m_showTimelinePanel = on; }
bool Settings::showTimelinePanel()               { return m_showTimelinePanel; }

void Settings::setMainWindowWidth(int value)     { m_mainWindowWidth = value; }
int Settings::mainWindowWidth()                  { return m_mainWindowWidth; }
void Settings::setMainWindowHeight(int value)    { m_mainWindowHeight = value; }
int Settings::mainWindowHeight()                 { return m_mainWindowHeight; }

void Settings::setLastPath(QString value)        { m_lastPath = value; }
QString Settings::lastPath()                     { return m_lastPath; }

void Settings::setFog(bool on)                { m_fog = on; }
bool Settings::fog()                             { return m_fog; }
void Settings::setFloorTranslucency(int on)   { m_floorTranslucency = on; }
int  Settings::floorTranslucency()               { return m_floorTranslucency; }

void Settings::setEaseIn(bool on)             { m_easeIn = on; }
bool Settings::easeIn()                          { return m_easeIn; }
void Settings::setEaseOut(bool on)            { m_easeOut = on; }
bool Settings::easeOut()                         { return m_easeOut; }

void Settings::readSettings()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     organizationName(), applicationName());
  settings.beginGroup(settingsGroup());

  if (settings.childGroups().contains(versionGroup(), Qt::CaseInsensitive))
  {
    settings.beginGroup(versionGroup());

    setLoop(settings.value(loopKey()).toBool());
    setSkeleton(settings.value(skeletonKey()).toBool());
    setJointLimits(settings.value(jointLimitsKey()).toBool());
    setProtectFirstFrame(settings.value(protectFirstFrameKey()).toBool());
    setShowTimelinePanel(settings.value(showTimelinePanelKey()).toBool());

    setMainWindowWidth(settings.value(mainWindowWidthKey()).toInt());
    setMainWindowHeight(settings.value(mainWindowHeightKey()).toInt());

    setLastPath(settings.value(lastPathKey()).toString());

    // OpenGL settings
    setFog(settings.value(fogKey()).toBool());
    setFloorTranslucency(settings.value(floorTranslucencyKey()).toInt());

    // settings for ease in/ease outFrame
    setEaseIn(settings.value(easeInKey()).toBool());
    setEaseOut(settings.value(easeOutKey()).toBool());

    settings.endGroup();
  }

  settings.endGroup();
}

void Settings::writeSettings()
{
  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     organizationName(), applicationName());
  settings.beginGroup(settingsGroup());
  settings.beginGroup(versionGroup());

  settings.setValue(loopKey(), loop());
  settings.setValue(skeletonKey(), skeleton());
  settings.setValue(jointLimitsKey(), jointLimits());
  settings.setValue(protectFirstFrameKey(), protectFirstFrame());
  settings.setValue(showTimelinePanelKey(), showTimelinePanel());

  settings.setValue(mainWindowWidthKey(), mainWindowWidth());
  settings.setValue(mainWindowHeightKey(), mainWindowHeight());

  settings.setValue(lastPathKey(), lastPath());

  // OpenGL settings
  settings.setValue(fogKey(), fog());
  settings.setValue(floorTranslucencyKey(), floorTranslucency());

  // settings for ease in/ease outFrame
  settings.setValue(easeInKey(), easeIn());
  settings.setValue(easeOutKey(), easeOut());

  settings.endGroup();
  settings.endGroup();
}

QString Settings::settingsGroup()
{
  return m_settingsGroup;
}

QString Settings::versionGroup()
{
  return m_versionGroup;
}

QString Settings::loopKey()
{
  return m_loopKey;
}

QString Settings::skeletonKey()
{
  return m_skeletonKey;
}

QString Settings::jointLimitsKey()
{
  return m_jointLimitsKey;
}

QString Settings::protectFirstFrameKey()
{
  return m_protectFirstFrameKey;
}

QString Settings::showTimelinePanelKey()
{
  return m_showTimelinePanelKey;
}

QString Settings::mainWindowWidthKey()
{
  return m_mainWindowWidthKey;
}

QString Settings::mainWindowHeightKey()
{
  return m_mainWindowHeightKey;
}

QString Settings::lastPathKey()
{
  return m_lastPathKey;
}

QString Settings::fogKey()
{
  return m_fogKey;
}

QString Settings::floorTranslucencyKey()
{
  return m_floorTranslucencyKey;
}

QString Settings::easeInKey()
{
  return m_easeInKey;
}

QString Settings::easeOutKey()
{
  return m_easeOutKey;
}
