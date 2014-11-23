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

#ifndef UPDATERSETTINGS_H
#define UPDATERSETTINGS_H

class QString;

class UpdaterSettings
{
  public:
    UpdaterSettings() = delete;

    static const QString lastSuccessfulCheck();
    static void setLastSuccessfulCheck(const QString& checkDateTime);
    static const bool hasAutomaticUpdates();
    static void setHasAutomaticUpdates(bool on);
    static const QString greatestInstalledVersion();
    static void setGreatestInstalledVersion(const QString& version);

    static void readSettings();
    static void writeSettings();

  private:
    static const QString m_lastSuccessfulCheckFormat;

    static const QString m_updaterSettingsGroup;
    static const QString m_updaterSettingsVersionGroup;

    static QString m_lastSuccessfulCheck;
    static const QString m_lastSuccessfulCheckKey;
    static bool m_hasAutomaticUpdates;
    static const QString m_hasAutomaticUpdatesKey;
    static QString m_greatestInstalledVersion;
    static const QString m_greatestInstalledVersionKey;
};

#endif
