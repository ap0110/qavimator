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

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <QFile>
#include <QStringList>
#include <QVector>
#include <QVector3D>

#include "mesh.h"

Mesh::Mesh(const QString& fileName) :
  QObject(nullptr),
  m_vertices(),
  m_normals(),
  m_triangleVertexIndices(),
  m_triangleNormalIndices()
{
  readFromFile(fileName);
}

void Mesh::draw() const
{
  if (!m_triangleVertexIndices.isEmpty())
  {
    int vertexSize = 3;
    QVector<float> vertices(m_triangleVertexIndices.size() * vertexSize);
    QVector<float> normals(m_triangleVertexIndices.size() * vertexSize);
    for (int i = 0; i < m_triangleVertexIndices.size(); ++i)
    {
      for (int j = 0; j < vertexSize; ++j)
      {
        vertices[(i * vertexSize) + j] = m_vertices[(m_triangleVertexIndices[i] * vertexSize) + j];
        normals[(i * vertexSize) + j] = m_normals[(m_triangleNormalIndices[i] * vertexSize) + j];
      }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glNormalPointer(GL_FLOAT, 0, normals.constData());
    glDrawArrays(GL_TRIANGLES, 0, m_triangleVertexIndices.size());
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

bool Mesh::readFromFile(const QString& fileName)
{
  QFile file(fileName);
  if (!file.exists())
  {
    // TODO Error: File does not exist
    return false;
  }
  if (!file.open(QIODevice::ReadOnly
                 | QIODevice::Text))
  {
    // TODO Error: Unable to open file for reading
    return false;
  }

  QString fileText = (QString)(file.readAll());

  QStringList lines = fileText.split("\n", QString::SkipEmptyParts);

  for (int i = 0; i < lines.size(); ++i)
  {
    auto tokens = lines[i].split(" ", QString::SkipEmptyParts);

    if (tokens[0] == "v")
    {
      if (tokens.size() == 4)
      {
        // TODO Handle errors converting to float
        m_vertices << tokens[1].toFloat() << tokens[2].toFloat() << tokens[3].toFloat();
      }
      else
      {
        // TODO Handle unexpected number of tokens
        return false;
      }
    }
    else if (tokens[0] == "vn")
    {
      if (tokens.size() == 4)
      {
        // TODO Handle errors converting to float
        m_normals << tokens[1].toFloat() << tokens[2].toFloat() << tokens[3].toFloat();
      }
      else
      {
        // TODO Handle unexpected number of tokens
        return false;
      }
    }
    else if (tokens[0] == "f")
    {
      // Separates the vertex index on the left from the normal index on the right
      QString separator = "//";
      if (tokens.size() == 4)
      {
        // TODO Handle errors converting to float
        for (int j = 1; j < 4; ++j)
        {
          m_triangleVertexIndices.append(tokens[j].section(separator, 0, 0).toFloat() - 1.0f);
          m_triangleNormalIndices.append(tokens[j].section(separator, 1, 1).toFloat() - 1.0f);
        }
      }
      else
      {
        // TODO Handle unexpected number of tokens
        return false;
      }
    }
    else
    {
      // TODO Handle unexpected tokens; ignore for now
    }
  }

  return true;
}

void Mesh::writeToFile(const QString& fileName)
{

}
