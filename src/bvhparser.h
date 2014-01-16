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

#include <QStringListIterator>

class Animation;
class Joint;

class BvhParser
{
  public:
    BvhParser(const QString& bvhData);
    ~BvhParser();

    Animation* parseBvhData();

  private:
    Joint* parseHierarchy();
    void parseJoint(Joint* joint);
    QVector3D* parseOffset();
    void parseChannels(Joint* joint);

    bool hasNext();
    void next();
    bool hasPrevious();
    void previous();

    const QString& token();
    const QString& nextToken();
    void expectToken(const QString& expected);
    void expectNextToken(const QString& expected);
    bool isEqual(const QString& lhs, const QString& rhs) const;

    QScopedPointer<QStringListIterator> m_parser;

    QStringList m_tokenList;
    QString m_token;
};
