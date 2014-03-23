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

const QString UserSettings::m_userSettingsGroup = "user_settings";
const QString UserSettings::m_userSettingsVersionGroup = "1.0";

bool UserSettings::m_loop = true;
const QString UserSettings::m_loopKey = "loop";
bool UserSettings::m_skeleton = false;
const QString UserSettings::m_skeletonKey = "skeleton";
bool UserSettings::m_jointLimits = true;
const QString UserSettings::m_jointLimitsKey = "joint_limits";
bool UserSettings::m_protectFirstFrame = true;
const QString UserSettings::m_protectFirstFrameKey = "protect_first_frame";
bool UserSettings::m_showTimelinePanel = true;
const QString UserSettings::m_showTimelinePanelKey = "show_timeline_panel";

int UserSettings::m_mainWindowWidth = 850;
const QString UserSettings::m_mainWindowWidthKey = "main_window_width";
int UserSettings::m_mainWindowHeight = 600;
const QString UserSettings::m_mainWindowHeightKey = "main_window_height";

QString UserSettings::m_lastPath = QString::null;
const QString UserSettings::m_lastPathKey = "last_path";

bool UserSettings::m_fog = true;
const QString UserSettings::m_fogKey = "fog";
int  UserSettings::m_floorTranslucency = 33;
const QString UserSettings::m_floorTranslucencyKey = "floor_translucency";

bool UserSettings::m_easeIn = false;
const QString UserSettings::m_easeInKey = "ease_in";
bool UserSettings::m_easeOut = false;
const QString UserSettings::m_easeOutKey = "ease_out";


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
int UserSettings::floorTranslucency()               { return m_floorTranslucency; }

void UserSettings::setEaseIn(bool on)             { m_easeIn = on; }
bool UserSettings::easeIn()                          { return m_easeIn; }
void UserSettings::setEaseOut(bool on)            { m_easeOut = on; }
bool UserSettings::easeOut()                         { return m_easeOut; }

void UserSettings::readSettings()
{
  Settings settings;
  settings.beginGroup(m_userSettingsGroup);

  if (settings.childGroups().contains(m_userSettingsVersionGroup, Qt::CaseInsensitive))
  {
    settings.beginGroup(m_userSettingsVersionGroup);

    setLoop(settings.value(m_loopKey).toBool());
    setSkeleton(settings.value(m_skeletonKey).toBool());
    setJointLimits(settings.value(m_jointLimitsKey).toBool());
    setProtectFirstFrame(settings.value(m_protectFirstFrameKey).toBool());
    setShowTimelinePanel(settings.value(m_showTimelinePanelKey).toBool());

    setMainWindowWidth(settings.value(m_mainWindowWidthKey).toInt());
    setMainWindowHeight(settings.value(m_mainWindowHeightKey).toInt());

    setLastPath(settings.value(m_lastPathKey).toString());

    // OpenGL settings
    setFog(settings.value(m_fogKey).toBool());
    setFloorTranslucency(settings.value(m_floorTranslucencyKey).toInt());

    // settings for ease in/ease outFrame
    setEaseIn(settings.value(m_easeInKey).toBool());
    setEaseOut(settings.value(m_easeOutKey).toBool());

    settings.endGroup();
  }

  settings.endGroup();
}

void UserSettings::writeSettings()
{
  Settings settings;
  settings.beginGroup(m_userSettingsGroup);
  settings.beginGroup(m_userSettingsVersionGroup);

  settings.setValue(m_loopKey, loop());
  settings.setValue(m_skeletonKey, skeleton());
  settings.setValue(m_jointLimitsKey, jointLimits());
  settings.setValue(m_protectFirstFrameKey, protectFirstFrame());
  settings.setValue(m_showTimelinePanelKey, showTimelinePanel());

  settings.setValue(m_mainWindowWidthKey, mainWindowWidth());
  settings.setValue(m_mainWindowHeightKey, mainWindowHeight());

  settings.setValue(m_lastPathKey, lastPath());

  // OpenGL settings
  settings.setValue(m_fogKey, fog());
  settings.setValue(m_floorTranslucencyKey, floorTranslucency());

  // settings for ease in/ease outFrame
  settings.setValue(m_easeInKey, easeIn());
  settings.setValue(m_easeOutKey, easeOut());

  settings.endGroup();
  settings.endGroup();
}
