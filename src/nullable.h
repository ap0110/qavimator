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

#ifndef NULLABLE_H
#define NULLABLE_H

#include <cstddef>

template<typename T> class Nullable
{
  public:
    Nullable() : m_hasValue(false), m_value() {}
    Nullable(std::nullptr_t) : m_hasValue(false), m_value() {}
    Nullable(const T& value) : m_hasValue(true), m_value(value) {}
    Nullable(const Nullable<T>& nullable) : m_hasValue(nullable.hasValue()), m_value(nullable.value()) {}
    ~Nullable() {}

    bool hasValue() const { return m_hasValue; }
    T value() const { return m_value; }

    Nullable<T>& operator=(const Nullable<T>& other)
    {
      if (other.hasValue())
      {
        m_value = other.value();
        m_hasValue = true;
      }
      else
      {
        m_hasValue = false;
      }

      return *this;
    }

    Nullable<T>& operator=(const T& other)
    {
      m_value = other;
      m_hasValue = true;

      return *this;
    }

    Nullable<T>& operator=(std::nullptr_t)
    {
      m_hasValue = false;

      return *this;
    }

  private:
    bool m_hasValue;
    T m_value;
};

#endif
