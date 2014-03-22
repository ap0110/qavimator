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

#include "usersettings.h"

#include "usersettingsdialog.h"
#include "ui_usersettingsdialog.h"

UserSettingsDialog::UserSettingsDialog(QWidget* parent) :
  QDialog(parent,
          Qt::WindowTitleHint
          | Qt::WindowSystemMenuHint
          | Qt::WindowCloseButtonHint
          | Qt::MSWindowsFixedSizeDialogHint),
  ui(new Ui::UserSettingsDialog)
{
  qDebug("UserSettingsDialog::UserSettingsDialog()");

  ui->setupUi(this);

  ui->useFogCheckbox->setChecked(UserSettings::fog());
  ui->floorTranslucencySpin->setValue(UserSettings::floorTranslucency());
  ui->easeInCheckbox->setChecked(UserSettings::easeIn());
  ui->easeOutCheckbox->setChecked(UserSettings::easeOut());
}

UserSettingsDialog::~UserSettingsDialog()
{
  qDebug("UserSettingsDialog::~UserSettingsDialog()");
}

void UserSettingsDialog::on_applyButton_clicked()
{
  qDebug("accept()");
  apply();
}

void UserSettingsDialog::on_okButton_clicked()
{
  qDebug("acceptOk()");
  apply();
  accept();
}

void UserSettingsDialog::on_cancelButton_clicked()
{
  qDebug("reject()");
  reject();
}

void UserSettingsDialog::on_useFogCheckbox_toggled(bool state)
{
  qDebug("useFogToggled(%d)",state);
}

void UserSettingsDialog::on_floorTranslucencySpin_valueChanged(int value)
{
  qDebug("floorTranslucencyChanged(%d)",value);
}

void UserSettingsDialog::on_easeInCheckbox_toggled(bool state)
{
  qDebug("easeInToggled(%d)",state);
}

void UserSettingsDialog::on_easeOutCheckbox_toggled(bool state)
{
  qDebug("easeOutToggled(%d)",state);
}

void UserSettingsDialog::apply()
{
  UserSettings::setFog(ui->useFogCheckbox->isChecked());
  UserSettings::setFloorTranslucency(ui->floorTranslucencySpin->value());
  UserSettings::setEaseIn(ui->easeInCheckbox->isChecked());
  UserSettings::setEaseOut(ui->easeOutCheckbox->isChecked());
  emit configChanged();
  qApp->processEvents();
}
