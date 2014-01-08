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

#include <QDir>

#include "animation.h"

#include "filemanager.h"

FileManager::FileManager(QObject* parent)
  : QObject(parent)
{
}

FileManager::~FileManager()
{
}

Animation* FileManager::loadAnimationFromFile(const QString& filePath)
{
  QFile file(filePath);
  if (!file.exists())
  {
    return NULL;
  }

  Animation* result = NULL;

  FileType type = determineFileType(file);
  switch (type)
  {
    case FT_BVH:
      result = readBvh(file);
      break;
    case FT_ANIM:
      result = readAnim(file);
      break;
    case FT_QAVM:
      result = readQavm(file);
      break;
    default:
      // TODO Alert user to unknown file type
      result = NULL;
      break;
  }

  return result;
}

Animation* FileManager::loadAnimationFromApplicationData(const QString& fileName)
{
  QDir dataDirectory = getDataDirectoryByOperatingSystem();

  if (!dataDirectory.exists(fileName))
  {
    return NULL;
  }

  return loadAnimationFromFile(dataDirectory.absoluteFilePath(fileName));
}

QDir FileManager::getDataDirectoryByOperatingSystem() const
{
#ifdef Q_OS_MAC
  return QDir(QApplication::applicationDirPath() + "/../Resources");
#else
  return QDir(QDir::currentPath() + QString("/data"));
#endif
}

const FileManager::FileType FileManager::determineFileType(const QFile& file) const
{
  // TODO Determine the file type by looking at the content, NOT by looking at the file extension
  return FT_UNKNOWN;
}

Animation* FileManager::readBvh(const QFile& file)
{
  // TODO Read BVH file
  return NULL;
}

Animation* FileManager::readAnim(const QFile& file)
{
  // TODO Read Second Life animation file
  return NULL;
}

Animation* FileManager::readQavm(const QFile& file)
{
  // TODO Read QAvimator file
  return NULL;
}
