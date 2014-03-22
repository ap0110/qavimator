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

#ifndef USERSETTINGSDIALOG_H
#define USERSETTINGSDIALOG_H

/*
  @author Zi Ree <Zi Ree @ Second Life>
*/

#include <QDialog>

namespace Ui {
class UserSettingsDialog;
}

class UserSettingsDialog : public QDialog
{
  Q_OBJECT

  public:
    UserSettingsDialog(QWidget* parent = nullptr);
    ~UserSettingsDialog();

  signals:
    void configChanged();

  protected slots:
    void on_applyButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void on_useFogCheckbox_toggled(bool state);
    void on_floorTranslucencySpin_valueChanged(int value);

    void on_easeInCheckbox_toggled(bool state);
    void on_easeOutCheckbox_toggled(bool state);

  private:
    void apply();

    QScopedPointer<Ui::UserSettingsDialog> ui;
};

#endif
