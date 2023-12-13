// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellerswidget.h"
#include "sellersconfig.h"
#include "sellerssaleslist.h"
#include "sellerssalestab.h"
#include "sellerssalewidget.h"

SellersWidget::SellersWidget(QWidget *parent)
    : AntiquaCRM::TabsIndex{SELLERS_INTERFACE_TABID, parent} {
  setObjectName("sellers_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("view-financial-transfer"));
  setWindowTitle(tr("Providers"));
  setClosable(false);

  AntiquaCRM::Splitter *m_splitter = new AntiquaCRM::Splitter(this);

  m_pages = new SellersSalesTab(this);
  m_splitter->addLeft(m_pages);

  m_tree = new SellersSalesList(this);
  m_splitter->addRight(m_tree);

  insertWidget(0, m_splitter);

  setCurrentIndex(ViewPage::MainView);

  // Signals:SellersSalesList
  connect(m_tree, SIGNAL(sendQueryOrder(const QString &, const QString &)),
          SLOT(openOrderPage(const QString &, const QString &)));
}

bool SellersWidget::loadProviderPlugins() {
  AntiquaCRM::ProvidersLoader loader(this);
  p_list = loader.interfaces(this);
  if (p_list.size() < 1) {
    qWarning("Provider plugins loader failed!");
    return false;
  }
  return true;
}

bool SellersWidget::findPage(const QString &provider, const QString &oid) {
  Q_UNUSED(provider);
  for (int p = 0; p < m_pages->count(); p++) {
    if (m_pages->widget(p)->windowTitle() == oid) {
      m_pages->setCurrentIndex(p);
      return true;
    }
  }
  return false;
}

void SellersWidget::openProviderAction(const QJsonObject &data) {
  if (!data.contains("PROVIDER"))
    return;

  const QString _provider = data.value("PROVIDER").toString();
  QListIterator<AntiquaCRM::ProviderInterface *> it(p_list);
  while (it.hasNext()) {
    AntiquaCRM::ProviderInterface *iface = it.next();
    if (iface == nullptr)
      continue;

    if (iface->interfaceName().contains(_provider, Qt::CaseInsensitive)) {
      iface->operationWidget(this, data);
    }
  }
}

void SellersWidget::openOrderPage(const QString &provider, const QString &oid) {
  if (provider.isEmpty() || oid.isEmpty())
    return;

  if (findPage(provider, oid))
    return;

  qint64 _cid = -1;
  QString _sql("SELECT pr_customer_id, pr_order_data FROM provider_orders");
  _sql.append(" WHERE pr_name='" + provider + "' AND pr_order='");
  _sql.append(oid + "';");

  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() == 1) {
    _query.next();
    _cid = _query.value("pr_customer_id").toInt();
    QByteArray _data = _query.value("pr_order_data").toString().toLocal8Bit();
    const QJsonDocument _jdoc = QJsonDocument::fromJson(_data);
    if (_jdoc.isEmpty() || _jdoc.object().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << provider << oid << _sql;
#else
      qWarning("Can't set Provider Order data for %s:%s", // info
               qPrintable(provider), qPrintable(oid));
#endif
      return;
    }

    QJsonObject _jobj = _jdoc.object();
    _jobj.insert("c_id", _cid);

    SellersSalesWidget *mp = new SellersSalesWidget(_jobj, m_pages);
    if (mp->init()) {
      int index = m_pages->addPage(oid, mp);
      connect(mp, SIGNAL(sendOpenRemoteAction(const QJsonObject &)),
              SLOT(openProviderAction(const QJsonObject &)));

      m_pages->setCurrentIndex(index);
    }
  }
}

void SellersWidget::openStartPage() {
  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);
}

void SellersWidget::onEnterChanged() {
  openStartPage();
  if (!initialed) {
    loadProviderPlugins();
    initialed = true;
  }
  m_tree->loadUpdate();
}

const QString SellersWidget::getTitle() const { return tr("Providers"); }

bool SellersWidget::customAction(const QJsonObject &obj) {
  if (obj.isEmpty() || !obj.contains("ACTION"))
    return false;

  qDebug() << Q_FUNC_INFO << "customAction" << obj;

  return false;
}

const QStringList SellersWidget::acceptsCustomActions() const {
  return QStringList({"provider_order"});
}
