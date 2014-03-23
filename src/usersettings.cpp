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

#include "settings.h"

#include "usersettings.h"

QString UserSettings::m_userSettingsGroup = "user_settings";
QString UserSettings::m_versionGroup = "1.0";

bool UserSettings::m_loop = true;
QString UserSettings::m_loopKey = "loop";
bool UserSettings::m_skeleton = false;
QString UserSettings::m_skeletonKey = "skeleton";
bool UserSettings::m_jointLimits = true;
QString UserSettings::m_jointLimitsKey = "joint_limits";
bool UserSettings::m_protectFirstFrame = true;
QString UserSettings::m_protectFirstFrameKey = "protect_first_frame";
bool UserSettings::m_showTimelinePanel = true;
QString UserSettings::m_showTimelinePanelKey = "show_timeline_panel";

int UserSettings::m_mainWindowWidth = 850;
QString UserSettings::m_mainWindowWidthKey = "main_window_width";
int UserSettings::m_mainWindowHeight = 600;
QString UserSettings::m_mainWindowHeightKey = "main_window_height";

QString UserSettings::m_lastPath = QString::null;
QString UserSettings::m_lastPathKey = "last_path";

bool UserSettings::m_fog = true;
QString UserSettings::m_fogKey = "fog";
int  UserSettings::m_floorTranslucency = 33;
QString  UserSettings::m_floorTranslucencyKey = "floor_translucency";

bool UserSettings::m_easeIn = false;
QString UserSettings::m_easeInKey = "ease_in";
bool UserSettings::m_easeOut = false;
QString UserSettings::m_easeOutKey = "ease_out";


void UserSettings::setLoop(bool on)               { m_loop = on; }
bool UserSettings::loop()                            { return m_loop; }
void UserSettings::setSkeleton(bool on)           { m_skeleton = on; }
bool UserSettings::skeleton()                        { return m_skeleton; }
void UserSettings::setJointLimits(bool on)        { m_jointLimits = on; }
bool UserSettings::jointLimits()                     { return m_jointLimits; }
void UserSettings::setProtectFirstFrame(bool on)  { m_protectFirstFrame = on; }
bool UserSettings::protectFirstFrame()               { return m_protectFirstFrame; }
void UserSettings::setShowTimelinePanel(bool on)  { m_showTimelinePanel = on; }
bool UserSettings::showTimelinePanel()               { return m_showTimelinePanel; }

void UserSettings::setMainWindowWidth(int value)     { m_mainWindowWidth = value; }
int UserSettings::mainWindowWidth()                  { return m_mainWindowWidth; }
void UserSettings::setMainWindowHeight(int value)    { m_mainWindowHeight = value; }
int UserSettings::mainWindowHeight()                 { return m_mainWindowHeight; }

void UserSettings::setLastPath(QString value)        { m_lastPath = value; }
QString UserSettings::lastPath()                     { return m_lastPath; }

void UserSettings::setFog(bool on)                { m_fog = on; }
bool UserSettings::fog()                             { return m_fog; }
void UserSettings::setFloorTranslucency(int on)   { m_floorTranslucency = on; }
int  UserSettings::floorTranslucency()               { return m_floorTranslucency; }

void UserSettings::setEaseIn(bool on)             { m_easeIn = on; }
bool UserSettings::easeIn()                          { return m_easeIn; }
void UserSettings::setEaseOut(bool on)            { m_easeOut = on; }
bool UserSettings::easeOut()                         { return m_easeOut; }

void UserSettings::readSettings()
{
  Settings settings;
  settings.beginGroup(userSettingsGroup());

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

void UserSettings::writeSettings()
{
  Settings settings;
  settings.beginGroup(userSettingsGroup());
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

QString UserSettings::userSettingsGroup()
{
  return m_userSettingsGroup;
}

QString UserSettings::versionGroup()
{
  return m_versionGroup;
}

QString UserSettings::loopKey()
{
  return m_loopKey;
}

QString UserSettings::skeletonKey()
{
  return m_skeletonKey;
}

QString UserSettings::jointLimitsKey()
{
  return m_jointLimitsKey;
}

QString UserSettings::protectFirstFrameKey()
{
  return m_protectFirstFrameKey;
}

QString UserSettings::showTimelinePanelKey()
{
  return m_showTimelinePanelKey;
}

QString UserSettings::mainWindowWidthKey()
{
  return m_mainWindowWidthKey;
}

QString UserSettings::mainWindowHeightKey()
{
  return m_mainWindowHeightKey;
}

QString UserSettings::lastPathKey()
{
  return m_lastPathKey;
}

QString UserSettings::fogKey()
{
  return m_fogKey;
}

QString UserSettings::floorTranslucencyKey()
{
  return m_floorTranslucencyKey;
}

QString UserSettings::easeInKey()
{
  return m_easeInKey;
}

QString UserSettings::easeOutKey()
{
  return m_easeOutKey;
}
