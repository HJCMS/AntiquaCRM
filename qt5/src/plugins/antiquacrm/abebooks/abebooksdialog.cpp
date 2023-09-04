// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksdialog.h"

#include <QLayout>
#include <QUrl>
#include <QUrlQuery>

AbeBooksDialog::AbeBooksDialog(QWidget *parent)
    : AntiquaCRM::UpdateDialog{parent} {
  setMinimumWidth(500);
  setWindowTitle("AbeBooks");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  int col = 0;
  m_info = new QLabel(this);
  m_info->setWordWrap(true);
  m_info->setTextFormat(Qt::RichText);
  m_info->setOpenExternalLinks(true);
  m_info->setStyleSheet("QLabel {font-style:italic; font-size:smaller;}");
  layout->insertWidget(col++, m_info);

  setLayout(layout);
}

bool AbeBooksDialog::prepareAction() { return false; }

void AbeBooksDialog::networkAction() {}

int AbeBooksDialog::exec(const QString &orderId, const QJsonObject &data) {
  p_order_id = orderId;
  p_system_data = data;

  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup("provider/abebooks");
  p_seller_id = cfg.value("seller_id", QString()).toString();
  cfg.endGroup();

  QUrl url;
  if (p_seller_id.length() > 2) {
    url.setScheme("https");
    url.setHost("www.abebooks.com");
    url.setPath("/servlet/OrderUpdate");
    QUrlQuery _query;
    _query.addQueryItem("abepoid", p_order_id);
    _query.addQueryItem("clientid", p_seller_id);
    url.setQuery(_query);
  }

  QStringList _l;
  _l << tr("This Plugin currently not support any remote actions.");
  if (url.isValid())
    _l << tr("Open <a href='%1'>AbeBooks Orders page.</a>").arg(url.toString());
  m_info->setText(_l.join("<br>"));

  return QDialog::exec();
}
