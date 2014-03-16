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

#include "versioning.h"

#ifdef VERSION_NUMBER
  QString Versioning::m_versionNumber = VERSION_NUMBER;
#else
  QString Versioning::m_versionNumber = "0";
#endif

#if UPDATE_CHANNEL == 2
  Versioning::UpdateChannel Versioning::m_updateChannel = UpdateChannel::Release;
#elif UPDATE_CHANNEL == 1
  Versioning::UpdateChannel Versioning::m_updateChannel = UpdateChannel::Beta;
#else
  Versioning::UpdateChannel Versioning::m_updateChannel = UpdateChannel::Development;
#endif

Versioning::Versioning()
{
  // should never be accessed
  qWarning("Constructor of static class \"Versioning\" was called");
}

Versioning::~Versioning()
{
  // should never be accessed
  qWarning("Deconstructor of static class \"Versioning\" was called");
}

QString Versioning::versionNumber()
{
  return m_versionNumber;
}

QString Versioning::updateChannel()
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

QString Versioning::updateChannelSuffix()
{
  switch (m_updateChannel)
  {
    case UpdateChannel::Release:
      return "";
      break;
    case UpdateChannel::Beta:
      return "-beta";
      break;
    default:
      return "-development";
      break;
  }
}
