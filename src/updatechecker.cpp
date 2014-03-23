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

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScopedPointer>
#include <QXmlStreamReader>

#include "metadata.h"
#include "settings.h"
#include "versionnumber.h"

#include "updatechecker.h"

const QString UpdateChecker::m_url =
    QString(
      "http://qavimator.bitbucket.org/applications/qavimator/%1/updates.xml"
      ).arg(Metadata::updateChannel().toLower());
const QString UpdateChecker::m_updatesVersion = "1.0";

const QString UpdateChecker::m_updateSettingsGroup = "update_settings";
const QString UpdateChecker::m_updateSettingsVersionGroup = "1.0";

QDateTime UpdateChecker::m_lastSuccessfulCheck = Metadata::buildDateTime();
const QString UpdateChecker::m_lastSuccessfulCheckKey = "last_successful_check";
bool UpdateChecker::m_hasAutomaticUpdates = true;
const QString UpdateChecker::m_hasAutomaticUpdatesKey = "has_automatic_updates";

UpdateChecker::UpdateChecker(QObject* parent) :
  QObject(parent),
  m_networkAccessManager(new QNetworkAccessManager)
{
  readSettings();

  connect(m_networkAccessManager.data(), SIGNAL(finished(QNetworkReply*)),
          this, SLOT(replyFinished(QNetworkReply*)));
}

UpdateChecker::~UpdateChecker()
{
  writeSettings();
}

void UpdateChecker::checkUpdates()
{
  m_networkAccessManager->get(QNetworkRequest(m_url));
}

void UpdateChecker::replyFinished(QNetworkReply* reply)
{
  bool hasUpdated = processUpdates(reply->readAll());
  if (hasUpdated)
  {
    qDebug("Update available!");
  }
  reply->deleteLater();
}

void UpdateChecker::readSettings()
{
  Settings settings;
  settings.beginGroup(m_updateSettingsGroup);

  if (settings.childGroups().contains(m_updateSettingsVersionGroup))
  {
    settings.beginGroup(m_updateSettingsVersionGroup);

    m_lastSuccessfulCheck = settings.value(m_lastSuccessfulCheckKey).toDateTime();
    m_hasAutomaticUpdates = settings.value(m_hasAutomaticUpdatesKey).toBool();

    settings.endGroup();
  }
  settings.endGroup();
}

void UpdateChecker::writeSettings()
{
  Settings settings;
  settings.beginGroup(m_updateSettingsGroup);
  settings.beginGroup(m_updateSettingsVersionGroup);

  settings.setValue(m_lastSuccessfulCheckKey, m_lastSuccessfulCheck);
  settings.setValue(m_hasAutomaticUpdatesKey, m_hasAutomaticUpdates);

  settings.endGroup();
  settings.endGroup();
}

bool UpdateChecker::processUpdates(const QByteArray& updates)
{
  QXmlStreamReader xmlStreamReader(updates);

  bool hasUpdate = false;
  while (!xmlStreamReader.atEnd())
  {
    if (!xmlStreamReader.readNextStartElement()) break;
    if (xmlStreamReader.name() == "updates")
    {
      hasUpdate = readUpdates(xmlStreamReader);
    }
  }

  if (xmlStreamReader.hasError())
  {
    qDebug(QString("Error processing updates.xml: %1").arg(xmlStreamReader.errorString()).toStdString().c_str());
    hasUpdate = false;
  }
  else
  {
    qDebug("Error processing updates.xml: End of document reached without finding version number");
    hasUpdate = false;
  }

  return hasUpdate;
}

bool UpdateChecker::readUpdates(QXmlStreamReader& xmlStreamReader)
{
  QStringRef updatesVersion = xmlStreamReader.attributes().value("version");
  if (updatesVersion.isEmpty())
  {
    qDebug("Updates version attribute is empty or not defined");
  }
  else
  {
    VersionNumber versionNumber(updatesVersion.toString());
    if (versionNumber.compare(m_updatesVersion) > 0)
    {
      return true;
    }
  }

  while (!xmlStreamReader.atEnd())
  {
    if (!xmlStreamReader.readNextStartElement()) return false;
    if (xmlStreamReader.name() == "version")
    {
      return readVersion(xmlStreamReader);
    }
  }

  return false;
}

bool UpdateChecker::readVersion(QXmlStreamReader& xmlStreamReader)
{
  VersionNumber versionNumber(xmlStreamReader.readElementText());

  if (versionNumber.isValid()
      && versionNumber.compare(Metadata::versionNumber()) > 0)
  {
    return true;
  }

  return false;
}
