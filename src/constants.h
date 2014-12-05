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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QStandardItem>

class QString;

class VersionNumber;

class Constants
{
  public:
    enum class ItemType
    {
      Prop = 1 + QStandardItem::UserType
    };

    Constants() = delete;

    static const QString buildDateTime();
    static const VersionNumber& versionNumber();
    static const QString versionNumberString();
    static const QString buildNumber();
    static const QString updateChannel();
    static const QString applicationName();
    static const QString organizationName();
    static const QString organizationDomain();

    static const QString qavimatorDataPath();
    static const QString defaultPosePath();
    static const QString limitsFilePath();
    static const QString maleBvhPath();
    static const QString femaleBvhPath();
    static const QString cubeMeshFilePath();
    static const QString sphereMeshFilePath();
    static const QString coneMeshFilePath();
    static const QString torusMeshFilePath();

    static const int keyWidth();
    static const int keyHeight();
    static const int lineHeight();
    static const int leftStrut();
    // FIXME: find out at runtime
    static const int numParts();

  private:
    enum class UpdateChannel
    {
      Development = 0,
      Beta = 1,
      Release = 2
    };

    static const QString m_buildDateTime;
    static const VersionNumber m_versionNumber;
    static const QString m_buildNumber;
    static const UpdateChannel m_updateChannel;
    static const QString m_applicationName;
    static const QString m_qavimatorDataPath;
};

#endif
