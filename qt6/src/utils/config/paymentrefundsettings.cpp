// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentrefundsettings.h"

#include <QLayout>

PaymentRefundSettings::PaymentRefundSettings(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_groupBox = new QGroupBox(this);
  m_groupBox->setTitle(tr("Refunding settings"));

  int row = 0;
  QGridLayout *m_glayout = new QGridLayout(m_groupBox);

  QLabel *m_info = infoWidget(tr("In this area you can set different refund "
                                 "costs for some Article groups."));
  m_info->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_info->setWordWrap(true);
  m_glayout->addWidget(m_info, row++, 0, 1, 2);

  m_glayout->addWidget(infoWidget(tr("Default")), row, 0, 1, 1);
  m_default = editWidget("default");
  m_glayout->addWidget(m_default, row++, 1, 1, 1);

  m_glayout->addWidget(infoWidget(tr("Books")), row, 0, 1, 1);
  m_books = editWidget("books");
  m_glayout->addWidget(m_books, row++, 1, 1, 1);

  m_glayout->addWidget(infoWidget(tr("Stitches")), row, 0, 1, 1);
  m_prints = editWidget("prints");
  m_glayout->addWidget(m_prints, row++, 1, 1, 1);

  m_glayout->addWidget(infoWidget(tr("Media")), row, 0, 1, 1);
  m_media = editWidget("cdvinyl");
  m_glayout->addWidget(m_media, row++, 1, 1, 1);

  m_glayout->addWidget(infoWidget(tr("Various")), row, 0, 1, 1);
  m_various = editWidget("various");
  m_glayout->addWidget(m_various, row++, 1, 1, 1);

  m_groupBox->setLayout(m_glayout);
  layout->addWidget(m_groupBox);
  setLayout(layout);
}

QLabel *PaymentRefundSettings::infoWidget(const QString &text) {
  QLabel *lb = new QLabel(text, m_groupBox);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  lb->setIndent(2);
  lb->setWordWrap(false);
  return lb;
}

AntiquaCRM::PriceEdit *PaymentRefundSettings::editWidget(const QString &name) {
  AntiquaCRM::PriceEdit *edit = new AntiquaCRM::PriceEdit(m_groupBox);
  edit->setObjectName(name);
  edit->appendStretch(0);
  edit->setRange(p_min_cost, p_max_cost);
  edit->setSingleStep(0.10);
  return edit;
}

void PaymentRefundSettings::setConfig(const QJsonObject &obj) {
  if (obj.isEmpty())
    return;

  QList<AntiquaCRM::AInputWidget *> _l =
      m_groupBox->findChildren<AntiquaCRM::AInputWidget *>(QString());
  if (_l.size()) {
    QListIterator<AntiquaCRM::AInputWidget *> _it(_l);
    while (_it.hasNext()) {
      AntiquaCRM::AInputWidget *m_e = _it.next();
      if (m_e == nullptr)
        continue;

      QJsonValue _v = obj.value(m_e->objectName());
      if (_v.isNull() || _v.type() != QJsonValue::Double)
        continue;

      m_e->setValue(_v.toDouble());
    }
  }
}

const QJsonObject PaymentRefundSettings::getConfig() {
  QJsonObject _obj;
  QList<AntiquaCRM::AInputWidget *> _l =
      m_groupBox->findChildren<AntiquaCRM::AInputWidget *>(QString());

  if (_l.size()) {
    QListIterator<AntiquaCRM::AInputWidget *> _it(_l);
    while (_it.hasNext()) {
      AntiquaCRM::AInputWidget *m_e = _it.next();
      if (m_e == nullptr)
        continue;

      _obj.insert(m_e->objectName(), m_e->getValue().toDouble());
    }
  }
  return _obj;
}
