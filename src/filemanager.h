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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

class QDir;
class QFile;

class NewAnimation;

class FileManager : public QObject
{
  Q_OBJECT

  public:
    FileManager(QObject* parent = nullptr);
    ~FileManager();

    QSharedPointer<NewAnimation> loadAnimationFromFile(const QString& fileName);

    QSharedPointer<NewAnimation> loadAnimationFromApplicationData(const QString& fileName);

  private:
    typedef enum class
    {
      UNKNOWN,
      BVH,
      ANIM,
      QAVM
    } FileType;

    QDir getDataDirectoryByOperatingSystem() const;
    const FileType determineFileType(QFile& openedFile) const;

    QSharedPointer<NewAnimation> readBvh(QFile& openedFile);
    QSharedPointer<NewAnimation> readAnim(const QFile& file);
    QSharedPointer<NewAnimation> readQavm(const QFile& file);
};

#endif
