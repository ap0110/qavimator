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

#include <QDesktopServices>
#include <QFile>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QScopedPointer>
#include <QXmlStreamReader>

#include "metadata.h"
#include "settings.h"
#include "updatenotificationdialog.h"
#include "updatersettings.h"
#include "versionnumber.h"

#include "updatechecker.h"
#include "whatsnewdialog.h"

UpdateCheckResults::UpdateCheckResults() :
  m_hasUpdates(false),
  m_hasSuccessfulUpdateCheck(false)
{
}

UpdateCheckResults::UpdateCheckResults(bool hasUpdates, bool hasSuccessfulUpdateCheck) :
  m_hasUpdates(hasUpdates),
  m_hasSuccessfulUpdateCheck(hasSuccessfulUpdateCheck)
{
}

UpdateCheckResults::~UpdateCheckResults()
{
}

bool UpdateCheckResults::hasUpdates()
{
  return m_hasUpdates;
}

void UpdateCheckResults::setHasUpdates(bool hasUpdates)
{
  m_hasUpdates = hasUpdates;
}

bool UpdateCheckResults::hasSuccessfulUpdateCheck()
{
  return m_hasSuccessfulUpdateCheck;
}

void UpdateCheckResults::setHasSuccessfulUpdateCheck(bool hasSuccessfulUpdateCheck)
{
  m_hasSuccessfulUpdateCheck = hasSuccessfulUpdateCheck;
}


UpdateChecker::UpdateChecker(QObject* parent, QWidget* parentWidget) :
  QObject(parent),
  m_networkAccessManager(new QNetworkAccessManager),
  m_parentWidget(parentWidget),
  m_news(""),
  m_url(QString(
          "http://qavimator.bitbucket.org/applications/qavimator/%1/updates.xml"
          ).arg(Metadata::updateChannel().toLower())),
  m_updatesVersion("1.0"),
  m_newsFilePath("NEWS")
{
  UpdaterSettings::readSettings();
  readNews();

  connect(m_networkAccessManager.data(), SIGNAL(finished(QNetworkReply*)),
          this, SLOT(replyFinished(QNetworkReply*)));
}

UpdateChecker::~UpdateChecker()
{
  UpdaterSettings::writeSettings();
}

void UpdateChecker::onStartup()
{
  VersionNumber greatestInstalledVersion(UpdaterSettings::greatestInstalledVersion());

  bool wasNewsShown = false;
  // Check validity
  if (greatestInstalledVersion.isValid())
  {
    // If valid, compare with current version
    if (greatestInstalledVersion.compare(Metadata::versionNumber()) < 0)
    {
      // If less, try to write the current version
      // then show news if successful
      if (tryWriteCurrentVersion())
      {
        wasNewsShown = showNews();
      }
    }
  }
  else
  {
    // If invalid, try to write the current version
    // then show news if successful
    if (tryWriteCurrentVersion())
    {
      wasNewsShown = showNews();
    }
  }

  if (!wasNewsShown && UpdaterSettings::hasAutomaticUpdates())
  {
    if (UpdaterSettings::lastSuccessfulCheck() == "Never"
        || QDateTime::fromString(
          UpdaterSettings::lastSuccessfulCheck()
          ).addDays(1) < QDateTime::currentDateTime())
    {
      checkUpdates();
    }
  }
}

bool UpdateChecker::hasNews()
{
  return !m_news.isEmpty();
}

void UpdateChecker::checkUpdates()
{
  m_networkAccessManager->get(QNetworkRequest(m_url));
}

bool UpdateChecker::showNews()
{
  bool canShowNews = !m_news.isEmpty();

  if (canShowNews)
  {
    execDialog(DialogType::WhatsNew);
  }

  return canShowNews;
}

void UpdateChecker::replyFinished(QNetworkReply* reply)
{
  UpdateCheckResults updateCheckResults = processUpdates(reply->readAll());
  if (updateCheckResults.hasUpdates())
  {
    execDialog(DialogType::UpdateNotification);
  }
  if (updateCheckResults.hasSuccessfulUpdateCheck())
  {
    UpdaterSettings::setLastSuccessfulCheck(QDateTime::currentDateTime().toString());
  }
  emit updateCheckFinished();
  reply->deleteLater();
}

UpdateCheckResults UpdateChecker::processUpdates(const QByteArray& updates)
{
  QXmlStreamReader xmlStreamReader(updates);

  while (!xmlStreamReader.atEnd())
  {
    if (!xmlStreamReader.readNextStartElement()) break;
    if (xmlStreamReader.name() == "updates")
    {
      return readUpdates(xmlStreamReader);
    }
  }

  if (xmlStreamReader.hasError())
  {
    qDebug(QString("Error processing updates.xml: %1").arg(xmlStreamReader.errorString()).toStdString().c_str());
  }
  else
  {
    qDebug("Error processing updates.xml: End of document reached without finding version number");
  }

  return UpdateCheckResults();
}

UpdateCheckResults UpdateChecker::readUpdates(QXmlStreamReader& xmlStreamReader)
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
      return UpdateCheckResults(true, true);
    }
  }

  while (!xmlStreamReader.atEnd())
  {
    if (!xmlStreamReader.readNextStartElement()) break;
    if (xmlStreamReader.name() == "version")
    {
      return readVersion(xmlStreamReader);
    }
  }

  return UpdateCheckResults();
}

UpdateCheckResults UpdateChecker::readVersion(QXmlStreamReader& xmlStreamReader)
{
  VersionNumber versionNumber(xmlStreamReader.readElementText());

  UpdateCheckResults results;
  if (versionNumber.isValid())
  {
    results.setHasSuccessfulUpdateCheck(true);
    if (versionNumber.compare(Metadata::versionNumber()) > 0)
    {
      results.setHasUpdates(true);
    }
  }
  return results;
}

void UpdateChecker::readNews()
{
  QFile file(m_newsFilePath);
  if (file.exists()
      && file.open(QIODevice::ReadOnly
                 | QIODevice::Text))
  {
    m_news = static_cast<QString>(file.readAll());
  }
}

bool UpdateChecker::tryWriteCurrentVersion()
{
  bool isSuccessful = false;

  QString oldValue = UpdaterSettings::greatestInstalledVersion();

  UpdaterSettings::setGreatestInstalledVersion(Metadata::versionNumberString());

  UpdaterSettings::writeSettings();
  UpdaterSettings::setGreatestInstalledVersion(oldValue);
  UpdaterSettings::readSettings();

  VersionNumber greatestInstalledVersion(UpdaterSettings::greatestInstalledVersion());
  if (greatestInstalledVersion.isValid()
      && greatestInstalledVersion.compare(Metadata::versionNumber()) == 0)
  {
    isSuccessful = true;
  }

  return isSuccessful;
}

bool UpdateChecker::execDialog(DialogType dialogType)
{
  QScopedPointer<QDialog> dialog;

  switch (dialogType)
  {
    case DialogType::UpdateNotification:
      dialog.reset(new UpdateNotificationDialog(m_parentWidget));
      break;
    case DialogType::WhatsNew:
      dialog.reset(new WhatsNewDialog(m_news, m_parentWidget));
      break;
  }

  return dialog->exec();
}
