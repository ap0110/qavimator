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

#ifndef USERSETTINGS_H
#define USERSETTINGS_H

/**
	@author Zi Ree <Zi Ree @ Second Life>
*/

class QString;

class UserSettings
{
  public:
    UserSettings() = delete;

    static void setLoop(bool value);
    static bool loop();
    static void setSkeleton(bool value);
    static bool skeleton();
    static void setJointLimits(bool value);
    static bool jointLimits();
    static void setProtectFirstFrame(bool value);
    static bool protectFirstFrame();
    static void setShowTimelinePanel(bool value);
    static bool showTimelinePanel();

    static void setMainWindowWidth(int value);
    static int mainWindowWidth();
    static void setMainWindowHeight(int value);
    static int mainWindowHeight();

    static void setLastPath(QString value);
    static QString lastPath();

    static void setFog(bool on);
    static bool fog();
    static void setFloorTranslucency(int value);
    static int  floorTranslucency();

    static void setEaseIn(bool on);
    static bool easeIn();
    static void setEaseOut(bool on);
    static bool easeOut();

    static void readSettings();
    static void writeSettings();

private:
    static const QString m_userSettingsGroup;
    static const QString m_userSettingsVersionGroup;

    static bool m_loop;
    static const QString m_loopKey;
    static bool m_skeleton;
    static const QString m_skeletonKey;
    static bool m_jointLimits;
    static const QString m_jointLimitsKey;
    static bool m_protectFirstFrame;
    static const QString m_protectFirstFrameKey;
    static bool m_showTimelinePanel;
    static const QString m_showTimelinePanelKey;

    static int m_mainWindowWidth;
    static const QString m_mainWindowWidthKey;
    static int m_mainWindowHeight;
    static const QString m_mainWindowHeightKey;

    static QString m_lastPath;
    static const QString m_lastPathKey;

    static bool m_fog;
    static const QString m_fogKey;
    static int  m_floorTranslucency;
    static const QString  m_floorTranslucencyKey;

    static bool m_easeIn;
    static const QString m_easeInKey;
    static bool m_easeOut;
    static const QString m_easeOutKey;
};

#endif
