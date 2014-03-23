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

#include "metadata.h"
#include "updatersettings.h"

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent) :
  QDialog(parent,
          Qt::WindowTitleHint
          | Qt::WindowSystemMenuHint
          | Qt::WindowCloseButtonHint
          | Qt::MSWindowsFixedSizeDialogHint),
  ui(new Ui::AboutDialog)
{
  ui->setupUi(this);

  ui->versionLabel->setText(
        QString("%1 Version %2 (%3)"
        ).arg(Metadata::updateChannel()
        ).arg(Metadata::versionNumberString()
        ).arg(Metadata::buildNumber()));
  ui->dateTimeLabel->setText(Metadata::buildDateTime());
  setLastUpdateCheckLabel(UpdaterSettings::lastSuccessfulCheck());
  ui->autoUpdatesCheckBox->setChecked(UpdaterSettings::hasAutomaticUpdates());
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::updateCheckFinished()
{
  setLastUpdateCheckLabel(UpdaterSettings::lastSuccessfulCheck());
  ui->checkUpdatesButton->setDisabled(false);
}

void AboutDialog::on_checkUpdatesButton_clicked()
{
  ui->checkUpdatesButton->setDisabled(true);
  ui->lastUpdateCheckLabel->setText("Checking for updates...");
  emit checkUpdates();
}

void AboutDialog::on_autoUpdatesCheckBox_stateChanged(int state)
{
  UpdaterSettings::setHasAutomaticUpdates(state == Qt::Checked);
}

void AboutDialog::setLastUpdateCheckLabel(const QString& lastChecked)
{
  ui->lastUpdateCheckLabel->setText("Last Checked: " + lastChecked);
}
