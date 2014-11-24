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

#include "qavimatorwindow.h"

int main( int argc, char ** argv )
{
    QApplication application(argc, argv);
    QAvimatorWindow* qavimatorWindow = new QAvimatorWindow();
    // Queue an event to show the main window
    qavimatorWindow->show();
    // Queue an event to execute a method after the main window shows
    qavimatorWindow->queueAfterShow();
    application.connect(&application,SIGNAL(lastWindowClosed()),&application,SLOT(quit()));
    // Start the event loop
    return application.exec();
}
