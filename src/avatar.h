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

#ifndef AVATAR_H
#define AVATAR_H

#include <QObject>

typedef enum class
{
  MALE, FEMALE
} FigureType;

class Avatar : public QObject
{
  Q_OBJECT

  public:
    Avatar(QObject* parent = nullptr,
           float scale = 1.0,
           FigureType figureType = FigureType::FEMALE);
    ~Avatar();

    float scale() const;
    void setScale(float scale);

    FigureType figureType() const;
    void setFigureType(FigureType figureType);

  private:
    float m_scale;
    FigureType m_figureType;
};

#endif
