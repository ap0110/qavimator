#include "whatsnewdialog.h"
#include "ui_whatsnewdialog.h"

WhatsNewDialog::WhatsNewDialog(QString news, QWidget* parent) :
  QDialog(parent,
          Qt::Dialog
          | Qt::MSWindowsFixedSizeDialogHint
          | Qt::CustomizeWindowHint
          | Qt::WindowTitleHint
          | Qt::WindowSystemMenuHint
          | Qt::WindowCloseButtonHint),
  ui(new Ui::WhatsNewDialog)
{
  ui->setupUi(this);

  ui->plainTextEdit->setPlainText(news);
}

WhatsNewDialog::~WhatsNewDialog()
{
}
