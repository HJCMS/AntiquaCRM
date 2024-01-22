// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "apopupmessage.h"

namespace AntiquaCRM {

APopUpMessage::APopUpMessage(QWidget *parent) : QMessageBox{parent} {
  setWindowFlags(Qt::Tool);
  setDefaultButton(QMessageBox::Ok);
  setTextFormat(Qt::PlainText);
  setTextInteractionFlags(Qt::TextSelectableByMouse);
  setSizeGripEnabled(true);
}

void APopUpMessage::setErrorMessage(const QString &error,
                                    const QString &details) {
  setIcon(QMessageBox::Critical);
  setTextFormat(Qt::PlainText);
  if (windowTitle().isEmpty())
    setWindowTitle(tr("SQL-Error"));

  if (text().isEmpty())
    setText(tr("Detailed message"));

  setDetailedText(details);
  setInformativeText(error);
  setSizeGripEnabled(false);
}

void APopUpMessage::setWarnMessage(const QString &title, const QString &body) {
  setIcon(QMessageBox::Warning);
  setWindowTitle(title);
  setTextFormat(Qt::RichText);
  setText(body);
}

void APopUpMessage::setNoticeMessage(const QString &body) {
  setIcon(QMessageBox::Warning);
  setWindowTitle(tr("Warning"));
  setTextFormat(Qt::RichText);
  setText(body);
}

void APopUpMessage::setSuccessMessage(const QString &body) {
  setIcon(QMessageBox::Information);
  setWindowTitle(tr("Success"));
  setText(body);
}

void APopUpMessage::setUnsavedMessage(const QString &body, bool ask) {
  setIcon(QMessageBox::Warning);
  if (ask) {
    setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    setDefaultButton(QMessageBox::No);
  }
  setWindowTitle(tr("You have unsaved changes!"));
  setTextFormat(Qt::RichText);

  QString _html = QString("<b>%1</b>").arg(windowTitle());
  _html.append(QString("<p>%1</p>").arg(body));

  setText(_html);
}

int APopUpMessage::exec() {
  if (text().isEmpty() || windowTitle().isEmpty()) {
    qWarning("APopUpMessage use configuration functions before execute!");
    return QDialog::Rejected;
  }

  // We use Standard Buttons!
  switch (QMessageBox::exec()) {
  case (QMessageBox::Ok):
  case (QMessageBox::Yes):
  case (QMessageBox::Ignore):
    return QDialog::Accepted;

  default:
    return QDialog::Rejected;
  }
}

} // namespace AntiquaCRM
