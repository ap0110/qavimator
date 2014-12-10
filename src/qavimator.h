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

#ifndef QAVIMATOR_H
#define QAVIMATOR_H

#include <QObject>

class Scene;

class QAvimator : public QObject
{
    Q_OBJECT

  public:
    explicit QAvimator(QObject* parent = nullptr);

    // TODO Temporary method while logic is moved around
    void setScene(Scene* scene);

  public slots:
    void selectProp(int index);

  signals:
    void enablePropsUi(bool state);
    void enableAvatarRotationUi(bool state);
    void enableAvatarPositionUi(bool state);
    void updatePropSpinsUi(const QVector3D& position, const QVector3D& rotation, const QVector3D& scale);
    void selectPropUi(bool isSelected, int attachmentPoint);

  private:
    Scene* m_scene;
};

#endif
