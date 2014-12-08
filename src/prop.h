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

#ifndef PROP_H
#define PROP_H

/**
  @author Zi Ree
*/

#include <QStandardItem>

#include "meshmodel.h"

class Prop : public QStandardItem
{
  public:
    typedef enum
    {
      Box=0,
      Sphere=1,
      Cone=2,
      Torus=3
    } PropType;

    typedef enum
    {
      Normal=0,
      Highlighted=1,
      Selected=2
    } State;

    Prop(unsigned int propId,PropType propType,const QString& name, QSharedPointer<Mesh> mesh);
    ~Prop();

    int type() const;

    PropType propType() const;
    void setPropType(PropType propType);

    unsigned int id() const;
    const QString& name() const;
    unsigned int isAttached() const;

    float xPosition() const;
    float yPosition() const;
    float zPosition() const;

    float xScale() const;
    float yScale() const;
    float zScale() const;

    float xRotation() const;
    float yRotation() const;
    float zRotation() const;

    void setPosition(double xp,double yp,double zp);
    void setScale(double scx,double scy,double scz);
    void setRotation(double rx,double ry,double rz);

    void draw(State state) const;

    void attach(unsigned int where);

  private:
    PropType m_propType;
    unsigned int m_id;

    QString m_name;

    QScopedPointer<QVector3D> m_position;
    QScopedPointer<QVector3D> m_scale;
    QScopedPointer<QVector3D> m_rotation;

    unsigned int m_attachmentPoint;

    MeshModel m_meshModel;
};

#endif
