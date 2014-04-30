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

#include <QApplication>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include "qavimator.h"

int main( int argc, char ** argv )
{

#ifdef __APPLE__
    char firstArg[] = "-platformpluginpath";

    char buffer[1024];
    uint32_t maxLength = sizeof(buffer);
    int resultCode = _NSGetExecutablePath(buffer, &maxLength);
    if (resultCode != 0)
    {
	// Error getting path to executable
	return resultCode;
    }

    // Get length of string in buffer
    int actualLength = 0;
    bool foundNullTerminator = false;
    while (!foundNullTerminator && actualLength < maxLength)
    {
	if (buffer[actualLength] == '\0')
	{
	    foundNullTerminator = true;
	}
	else
	{
	    actualLength++;
	}
    }
    if (!foundNullTerminator)
    {
	// Error finding end of string
	return 1;
    }

    char relativePath[] = "/../Plugins";
    int relativePathLength = sizeof(relativePath);

    // Don't forget to create space for a null terminator
    char* secondArg = new char[actualLength + relativePathLength + 1];
    // Get the path to the executable
    for (int i = 0; i < actualLength; i++)
    {
	secondArg[i] = buffer[i];
    }
    // then append the relative path from the executable
    for (int i = actualLength; i < actualLength + relativePathLength; i++)
    {
	secondArg[i] = relativePath[i - actualLength];
    }
    // and finally append the null terminator
    secondArg[actualLength + relativePathLength] = '\0';

    int newArgc = argc + 2;
    char** newArgv = new char*[newArgc];
    // Preserve the arguments passed to the program
    for (int i = 0; i < argc; i++)
    {
	newArgv[i] = argv[i];
    }
    // and append the new arguments
    newArgv[argc] = firstArg;
    newArgv[argc + 1] = secondArg;
#else
    int newArgc = argc;
    char** newArgv = argv;
#endif

    QApplication application(newArgc, newArgv);
    QAvimator* qavimator = new QAvimator();
    qavimator->show();
    application.connect(&application,SIGNAL(lastWindowClosed()),&application,SLOT(quit()));
    int returnValue = application.exec();

#ifdef __APPLE__
    delete[] secondArg;
    delete[] newArgv;
#endif

    return returnValue;
}
