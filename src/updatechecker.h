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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QNetworkAccessManager>
#include <QObject>

class QXmlStreamReader;

class UpdateCheckResults
{
  public:
    UpdateCheckResults();
    UpdateCheckResults(bool hasUpdates, bool hasSuccessfulUpdateCheck);
    ~UpdateCheckResults();

    bool hasUpdates();
    void setHasUpdates(bool hasUpdates);
    bool hasSuccessfulUpdateCheck();
    void setHasSuccessfulUpdateCheck(bool hasSuccessfulUpdateCheck);

  private:
    bool m_hasUpdates;
    bool m_hasSuccessfulUpdateCheck;
};

class UpdateChecker : public QObject
{
  Q_OBJECT

  public:
    UpdateChecker(QObject* parent = nullptr);
    ~UpdateChecker();

  signals:
    void updateCheckFinished();

  public slots:
    void checkUpdates();

  private slots:
    void replyFinished(QNetworkReply* reply);

  private:
    UpdateCheckResults processUpdates(const QByteArray& updates);
    UpdateCheckResults readUpdates(QXmlStreamReader& xmlStreamReader);
    UpdateCheckResults readVersion(QXmlStreamReader& xmlStreamReader);

    QScopedPointer<QNetworkAccessManager> m_networkAccessManager;

    const QString m_url;
    const QString m_updatesVersion;
};

#endif
