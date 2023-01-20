// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "companywidget.h"

#include <AntiquaCRM>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif
#include <QRect>
#include <QRegExp>
#include <QString>

static const QString stripInput(const QString &text) {
  QString buff = text.trimmed();
  QRegExp pattern("[\\'\\>\\<]+");
  buff.replace(pattern, "");
  return buff;
}

TextInput::TextInput(QWidget *parent) : QTextEdit{parent} {
  setMinimumHeight(150);
  setAcceptRichText(false);
  setAutoFormatting(QTextEdit::AutoNone);
  setLineWrapMode(QTextEdit::NoWrap);
}

void TextInput::setValue(const QString &txt) {
  setPlainText(txt.trimmed());
  int nh = qRound(cursorRect().height() * (txt.count("\n") + 2.5));
  setFixedHeight(nh);
}

const QString TextInput::value() { return stripInput(toPlainText()); }

LineInput::LineInput(QWidget *parent) : QLineEdit{parent} {}

void LineInput::setValue(const QString &txt) { setText(txt.trimmed()); }

const QString LineInput::value() { return stripInput(text()); };

CompanyWidget::CompanyWidget(QWidget *parent) : QWidget{parent} {
  m_layout = new QGridLayout(this);
  m_layout->setObjectName("company_config_layout");
  QString info_txt =
      tr("On this page you configure your company's presentation settings.");
  QLabel *info_label = new QLabel(info_txt, this);
  m_layout->addWidget(info_label, layoutRow++, 0, 1, 2);
  setLayout(m_layout);
}

QLabel *CompanyWidget::infoCell(const QString &title) {
  QLabel *lb = new QLabel(title + ":", this);
  return lb;
}

void CompanyWidget::saveConfig() {
  Qt::FindChildOptions opt = Qt::FindDirectChildrenOnly;
  LineInput *cpn = findChild<LineInput *>("COMPANY_FULLNAME", opt);
  if (cpn != nullptr) {
    AntiquaCRM::ASettings cfg(this);
    cfg.setValue("company_fullname", cpn->value());
  }

  QStringList queries;
  QList<LineInput *> le = findChildren<LineInput *>(QString(), opt);
  for (int i = 0; i < le.count(); i++) {
    LineInput *me = le.at(i);
    QString update("UPDATE antiquacrm_company SET");
    update.append(" ac_value='" + me->value() + "'");
    update.append(" WHERE ac_class='" + me->objectName() + "';");
    queries.append(update);
  }
  QList<TextInput *> lt = findChildren<TextInput *>(QString(), opt);
  for (int i = 0; i < lt.count(); i++) {
    TextInput *me = lt.at(i);
    QString update("UPDATE antiquacrm_company SET");
    update.append(" ac_value='" + me->value() + "'");
    update.append(" WHERE ac_class='" + me->objectName() + "';");
    queries.append(update);
  }

  AntiquaCRM::ASqlCore sql(this);
  sql.query(queries.join("\n"));
  if (!sql.lastError().isEmpty()) {
    qWarning("SQL-ERROR Company save ...");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << sql.lastError();
#endif
    emit sendSaved(false);
  } else {
    emit sendSaved(true);
  }
}

bool CompanyWidget::loadConfig() {
  bool status = false;
  QString sql("SELECT * FROM antiquacrm_company ORDER BY ac_sort;");
  AntiquaCRM::ASqlCore asql(this);
  QSqlQuery q = asql.query(sql);
  if (q.size() > 0) {
    status = true;
    while (q.next()) {
      int _type = q.value("ac_edit").toInt();
      if (_type == 1) {
        QLabel *m_info = infoCell(q.value("ac_info").toString());
        m_layout->addWidget(m_info, layoutRow, 0, 1, 1,
                            (Qt::AlignTop | Qt::AlignRight));
        TextInput *m_edit = new TextInput(this);
        QString ac_class = q.value("ac_class").toString();
        m_edit->setObjectName(ac_class);
        m_edit->setPlaceholderText(ac_class);
        m_edit->setValue(q.value("ac_value").toString());
        m_layout->addWidget(m_edit, layoutRow, 1, 1, 1);
      } else {
        QLabel *m_info = infoCell(q.value("ac_info").toString());
        m_layout->addWidget(m_info, layoutRow, 0, 1, 1, Qt::AlignRight);
        LineInput *m_edit = new LineInput(this);
        QString ac_class = q.value("ac_class").toString();
        m_edit->setObjectName(ac_class);
        m_edit->setPlaceholderText(ac_class);
        m_edit->setValue(q.value("ac_value").toString());
        m_layout->addWidget(m_edit, layoutRow, 1, 1, 1);
      }
      layoutRow++;
    }
  }
  m_layout->setRowStretch(layoutRow, 1);
  return status;
}
