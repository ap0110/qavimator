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

#include <QString>

#include "metadata.h"
#include "settings.h"

#include "updatersettings.h"

const QString UpdaterSettings::m_lastSuccessfulCheckFormat = "MMM d, yyyy 'at' h:mm:ss";

const QString UpdaterSettings::m_updaterSettingsGroup = "updater_settings";
const QString UpdaterSettings::m_updaterSettingsVersionGroup = "1.0";

QString UpdaterSettings::m_lastSuccessfulCheck = "Never";
const QString UpdaterSettings::m_lastSuccessfulCheckKey = "last_successful_check";
bool UpdaterSettings::m_hasAutomaticUpdates = true;
const QString UpdaterSettings::m_hasAutomaticUpdatesKey = "has_automatic_updates";
QString UpdaterSettings::m_greatestInstalledVersion = "";
const QString UpdaterSettings::m_greatestInstalledVersionKey = "greatest_installed_version";

const QString UpdaterSettings::lastSuccessfulCheck()
{
  return m_lastSuccessfulCheck;
}

void UpdaterSettings::setLastSuccessfulCheck(const QString& checkDateTime)
{
  m_lastSuccessfulCheck = checkDateTime;
}

const bool UpdaterSettings::hasAutomaticUpdates()
{
  return m_hasAutomaticUpdates;
}

void UpdaterSettings::setHasAutomaticUpdates(bool on)
{
  m_hasAutomaticUpdates = on;
}

const QString UpdaterSettings::greatestInstalledVersion()
{
  return m_greatestInstalledVersion;
}

void UpdaterSettings::setGreatestInstalledVersion(const QString& version)
{
  m_greatestInstalledVersion = version;
}

void UpdaterSettings::readSettings()
{
  Settings settings;
  settings.beginGroup(m_updaterSettingsGroup);

  if (settings.childGroups().contains(m_updaterSettingsVersionGroup))
  {
    settings.beginGroup(m_updaterSettingsVersionGroup);

    m_lastSuccessfulCheck = settings.value(m_lastSuccessfulCheckKey).toString();
    m_hasAutomaticUpdates = settings.value(m_hasAutomaticUpdatesKey).toBool();
    m_greatestInstalledVersion = settings.value(m_greatestInstalledVersionKey).toString();

    settings.endGroup();
  }
  settings.endGroup();
}

void UpdaterSettings::writeSettings()
{
  Settings settings;
  settings.beginGroup(m_updaterSettingsGroup);
  settings.beginGroup(m_updaterSettingsVersionGroup);

  settings.setValue(m_lastSuccessfulCheckKey, lastSuccessfulCheck());
  settings.setValue(m_hasAutomaticUpdatesKey, hasAutomaticUpdates());
  settings.setValue(m_greatestInstalledVersionKey, greatestInstalledVersion());

  settings.endGroup();
  settings.endGroup();
}
