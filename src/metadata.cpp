/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree                                          *
 *   Zi Ree @ SecondLife                                                   *
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
#include <QStringList>

#include "versionnumber.h"

#include "metadata.h"

#ifdef BUILD_TIMESTAMP_Z
  const QString Metadata::m_buildTimestamp = BUILD_TIMESTAMP_Z;
#else
  const QString Metadata::m_buildTimestamp = "";
#endif

#ifdef VERSION_NUMBER
  const VersionNumber Metadata::m_versionNumber = VersionNumber(VERSION_NUMBER);
#else
  const QString Metadata::m_versionNumber = "0";
#endif

#if UPDATE_CHANNEL == 2
  const Metadata::UpdateChannel Metadata::m_updateChannel = UpdateChannel::Release;
#elif UPDATE_CHANNEL == 1
  const Metadata::UpdateChannel Metadata::m_updateChannel = UpdateChannel::Beta;
#else
  const Metadata::UpdateChannel Metadata::m_updateChannel = UpdateChannel::Development;
#endif


const QString& Metadata::buildTimestamp()
{
  return m_buildTimestamp;
}

const VersionNumber& Metadata::versionNumber()
{
  return m_versionNumber;
}

const QString Metadata::versionNumberString()
{
  return m_versionNumber.toString();
}

const QString Metadata::updateChannel()
{
  switch (m_updateChannel)
  {
    case UpdateChannel::Release:
      return "Release";
      break;
    case UpdateChannel::Beta:
      return "Beta";
      break;
    default:
      return "Development";
      break;
  }
}

const QString Metadata::updateChannelSuffix()
{
  switch (m_updateChannel)
  {
    case UpdateChannel::Release:
      return "";
      break;
    case UpdateChannel::Beta:
      return "-Beta";
      break;
    default:
      return "-Development";
      break;
  }
}
