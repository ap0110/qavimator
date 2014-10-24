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

#define OBJECT_START      8000

class PropManager : public QObject
{
  Q_OBJECT

  public:
    PropManager(QObject* parent = nullptr);
    ~PropManager();

    unsigned int getSelectedPropId();
    Prop* getPropByName(const QString& lookName);
    Prop* getPropById(unsigned int id);

    Prop* at(int i) const;
    int count() const;

    Prop* addProp(Prop::PropType type,
                        double x, double y, double z,
                        double xs, double ys, double zs,
                        double xr, double yr, double zr,
                        int attach, QSharedPointer<Mesh> mesh);
    void deleteProp(Prop* prop);
    void clearProps();

    void selectProp(unsigned int id = 0);

  private:
    unsigned int nextPropId;

    QList<Prop*> propList;

    unsigned int propSelected;
};

#endif
