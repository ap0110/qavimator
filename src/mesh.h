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

#ifndef MESH_H
#define MESH_H

#include <QVector3D>
#include <QVector4D>

#include <QObject>

class GLUquadric;

class Mesh : public QObject
{
  Q_OBJECT

  public:
    enum class Shape
    {
        NONE = 0,
        CUBE = 1,
        SPHERE = 2,
        CONE = 3,
        TORUS = 4,
        CUSTOM = 5
    };

    explicit Mesh(const QString& fileName, const Shape shape);
    ~Mesh();

    void draw() const;

    bool readFromFile(const QString& fileName);
    void writeToFile(const QString& fileName);

  private:
    Shape m_shape;

    GLUquadric* m_quadric;

    QList<float> m_vertices;
    QList<float> m_normals;

    QList<int> m_triangleVertexIndices;
    QList<int> m_triangleNormalIndices;

};

#endif
