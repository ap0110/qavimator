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

#include "constants.h"

#include "settings.h"

Settings::Settings(QObject* parent) :
  QObject(parent),
  m_settings(QSettings::IniFormat, QSettings::UserScope,
             Constants::organizationName(), Constants::applicationName())
{
}

Settings::~Settings()
{
}

QStringList Settings::childGroups() const
{
  return m_settings.childGroups();
}

void Settings::beginGroup(const QString& prefix)
{
  m_settings.beginGroup(prefix);
}

void Settings::endGroup()
{
  m_settings.endGroup();
}

QVariant Settings::value(const QString& key, const QVariant& defaultValue)
{
  return m_settings.value(key, defaultValue);
}

void Settings::setValue(const QString& key, const QVariant& value)
{
  m_settings.setValue(key, value);
}
