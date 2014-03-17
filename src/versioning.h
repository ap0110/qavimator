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

#ifndef VERSIONING_H
#define VERSIONING_H

class QString;

class Versioning
{
  public:
    Versioning() = delete;

    static QString buildTimestamp();
    static QString versionNumber();
    static QString updateChannel();
    static QString updateChannelSuffix();

  private:
    typedef enum class
    {
      Development = 0,
      Beta = 1,
      Release = 2
    } UpdateChannel;

    static QString m_buildTimestamp;
    static QString m_versionNumber;
    static UpdateChannel m_updateChannel;
};

#endif
