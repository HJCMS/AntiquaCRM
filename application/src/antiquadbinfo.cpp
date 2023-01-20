// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquadbinfo.h"

#include <AntiquaCRM>
#include <QLabel>

AntiquaDbInfo::AntiquaDbInfo(QWidget *parent)
  : QDialog{parent, Qt::Tool} {
  setWindowTitle(tr("Databaseinfo"));
  setMinimumSize(380, 160);
  setSizeGripEnabled(true);
  m_layout = new QGridLayout(this);
  m_layout->setColumnStretch(3, 1);
  setLayout(m_layout);
}

void AntiquaDbInfo::addRow(const QString &title, QString &value) {
  QLabel *m_a = new QLabel(title, this);
  m_a->setAlignment(Qt::AlignRight);
  m_layout->addWidget(m_a, row_count, 0, 1, 1);
  QLabel *m_b = new QLabel(":", this);
  m_b->setAlignment(Qt::AlignCenter);
  m_layout->addWidget(m_b, row_count, 1, 1, 1);
  QLabel *m_c = new QLabel(value, this);
  m_c->setAlignment(Qt::AlignLeft);
  m_layout->addWidget(m_c, row_count++, 2, 1, 1);
}

int AntiquaDbInfo::exec() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(
      AntiquaCRM::ASqlFiles::queryStatement("query_connection_info"));
  if (q.size() > 0) {
    QStringList data;
    QSqlRecord r = q.record();
    while (q.next()) {
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        QString _n = AntiquaCRM::AUtil::ucFirst(f.name());
        if (q.value(f.name()).type() == QVariant::String) {
          QString _v = q.value(f.name()).toString();
          addRow(_n, _v);
        }
      }
    }
    m_layout->setRowStretch(row_count++, 1);
  }
  return QDialog::exec();
}
