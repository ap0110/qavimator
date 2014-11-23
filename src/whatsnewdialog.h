#ifndef WHATSNEWDIALOG_H
#define WHATSNEWDIALOG_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
class WhatsNewDialog;
}

class WhatsNewDialog : public QDialog
{
  Q_OBJECT

  public:
    explicit WhatsNewDialog(QString news, QWidget* parent = nullptr);
    ~WhatsNewDialog();

  private:
    QScopedPointer<Ui::WhatsNewDialog> ui;
};

#endif
