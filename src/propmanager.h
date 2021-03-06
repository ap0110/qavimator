/***************************************************************************
 *   Copyright (C) 2006 by Zi Ree   *
 *   Zi Ree @ SecondLife   *
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

#ifndef PROPMANAGER_H
#define PROPMANAGER_H

#include <QObject>

#include "prop.h"

class PropManager : public QObject
{
  Q_OBJECT

  public:
    PropManager(int nextPropId, QObject* parent = nullptr);
    ~PropManager();

    unsigned int getSelectedPropId();
    Prop* getPropByName(const QString& lookName);
    Prop* getPropById(unsigned int id);

    Prop* at(int i) const;
    int count() const;

    Prop* addProp(Prop::PropType type, QSharedPointer<Mesh> mesh,
                  int attach,
                  double xPosition, double yPosition, double zPosition,
                  double xScale, double yScale, double zScale,
                  double xRotation, double yRotation, double zRotation);
    void deleteProp(Prop* prop);
    void clearProps();

    void selectProp(unsigned int id = 0);

    QAbstractItemModel* addressOfPropModel();

  private:
    unsigned int m_nextPropId;

    QStandardItemModel propModel;

    unsigned int propSelected;
};

#endif
