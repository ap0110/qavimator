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

class UpdateChecker : public QObject
{
  Q_OBJECT

  public:
    UpdateChecker(QObject* parent = nullptr);
    ~UpdateChecker();

  public slots:
    void checkUpdates();

  private slots:
    void replyFinished(QNetworkReply* reply);

  private:
    void readSettings();
    void writeSettings();

    bool processUpdates(const QByteArray& updates);
    bool readUpdates(QXmlStreamReader& xmlStreamReader);
    bool readVersion(QXmlStreamReader& xmlStreamReader);

    QScopedPointer<QNetworkAccessManager> m_networkAccessManager;

    static const QString m_url;
    static const QString m_updatesVersion;

    static const QString m_updateSettingsGroup;
    static const QString m_updateSettingsVersionGroup;

    static QDateTime m_lastSuccessfulCheck;
    static const QString m_lastSuccessfulCheckKey;
    static bool m_hasAutomaticUpdates;
    static const QString m_hasAutomaticUpdatesKey;
};

#endif
