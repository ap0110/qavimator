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

#include "updatersettings.h"

#include "updatenotificationdialog.h"
#include "ui_updatenotificationdialog.h"

UpdateNotificationDialog::UpdateNotificationDialog(QWidget *parent) :
  QDialog(parent,
          Qt::Window
          | Qt::CustomizeWindowHint
          | Qt::WindowTitleHint
          | Qt::WindowSystemMenuHint
          | Qt::WindowCloseButtonHint
          | Qt::MSWindowsFixedSizeDialogHint),
  ui(new Ui::UpdateNotificationDialog)
{
  ui->setupUi(this);

  ui->autoUpdatesCheckBox->setChecked(UpdaterSettings::hasAutomaticUpdates());
}

UpdateNotificationDialog::~UpdateNotificationDialog()
{
}

void UpdateNotificationDialog::on_autoUpdatesCheckBox_stateChanged(int state)
{
  UpdaterSettings::setHasAutomaticUpdates(state == Qt::Checked);
}
