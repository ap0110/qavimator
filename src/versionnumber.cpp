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

#include <QRegularExpression>
#include <QString>
#include <QStringList>

#include "versionnumber.h"

VersionNumber::VersionNumber(const QString& versionNumber) :
  m_versionNumber(versionNumber),
  m_isValid(true)
{
  QRegularExpression regularExpression("^(0|([1-9][0-9]*)).(0|([1-9][0-9]*))(.(0|([1-9][0-9]*)))*$");
  auto matchResult = regularExpression.match(m_versionNumber);
  if (!matchResult.hasMatch())
  {
    m_versionNumber = "0.0";
    m_isValid = false;
  }
}

const QString& VersionNumber::toString() const
{
  return m_versionNumber;
}

bool VersionNumber::isValid() const
{
  return m_isValid;
}

int VersionNumber::compare(const VersionNumber& rhs) const
{
  QStringList lhsTokens = m_versionNumber.split(".");
  QStringList rhsTokens = rhs.toString().split(".");
  auto lhsIter = lhsTokens.constBegin();
  auto rhsIter = rhsTokens.constBegin();

  while (lhsIter != lhsTokens.constEnd()
         && rhsIter != rhsTokens.constEnd())
  {
    ushort lhsNumber = lhsIter->toUShort();
    ushort rhsNumber = rhsIter->toUShort();
    if (lhsNumber != rhsNumber)
    {
      return lhsNumber - rhsNumber;
    }
    lhsIter++;
    rhsIter++;
  }
  while (lhsIter != lhsTokens.constEnd())
  {
    if (lhsIter->toUShort() != 0)
    {
      return 1;
    }
    lhsIter++;
  }
  while (rhsIter != rhsTokens.constEnd())
  {
    if (rhsIter->toUShort() != 0)
    {
      return -1;
    }
    rhsIter++;
  }
  return 0;
}
