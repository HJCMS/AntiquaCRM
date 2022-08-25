// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportingdialog.h"
#include "applsettings.h"
#include "daterangewidget.h"
#include "myicontheme.h"
#include "previewtable.h"

#include <QDebug>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLayout>
#include <QMessageBox>
#include <QStatusTipEvent>

#include <SqlCore>

ReportingDialog::ReportingDialog(QWidget *parent) : QDialog{parent} {
  setMinimumSize(500, 500);
  setSizeGripEnabled(true);
  setContentsMargins(5, 5, 5, 5);
  setWindowTitle("AntiquaCRM " + tr("Reports") + " [*]");

  m_cfg = new ApplSettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_infoLabel = new QLabel(this);
  m_infoLabel->setText(tr("Create a monthly sales report."));
  layout->addWidget(m_infoLabel);

  m_stackedWidget = new QStackedWidget(this);

  m_datePicker = new DateRangeWidget(m_stackedWidget);
  m_stackedWidget->insertWidget(0, m_datePicker);

  m_previewTable = new PreviewTable(this);
  m_stackedWidget->insertWidget(1, m_previewTable);

  layout->addWidget(m_stackedWidget);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setOrientation(Qt::Horizontal);
  btn_prev = m_btnBox->addButton(tr("Back"), QDialogButtonBox::ActionRole);
  btn_prev->setObjectName("btn_previous");
  btn_prev->setIcon(myIcon("edit_remove"));
  btn_prev->setEnabled(false);

  btn_next = m_btnBox->addButton(tr("Next"), QDialogButtonBox::ActionRole);
  btn_next->setObjectName("btn_next");
  btn_next->setIcon(myIcon("edit_add"));

  btn_apply = m_btnBox->addButton(tr("Apply"), QDialogButtonBox::AcceptRole);
  btn_apply->setObjectName("btn_apply");
  btn_apply->setIcon(myIcon("button_ok"));
  btn_apply->setEnabled(false);

  btn_quit = m_btnBox->addButton(tr("Quit"), QDialogButtonBox::RejectRole);
  btn_quit->setObjectName("btn_quit");
  btn_quit->setIcon(myIcon("button_cancel"));
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_stackedWidget, SIGNAL(currentChanged(int)), this,
          SLOT(pageEntered(int)));
  connect(btn_prev, SIGNAL(clicked()), this, SLOT(previousPage()));
  connect(btn_next, SIGNAL(clicked()), this, SLOT(nextPage()));
  connect(btn_apply, SIGNAL(clicked()), this, SLOT(apply()));
  connect(btn_quit, SIGNAL(clicked()), this, SLOT(close()));
}

int ReportingDialog::pageIndex() { return m_stackedWidget->currentIndex(); }

const QString ReportingDialog::getSqlQueryString() {
  QStringList fieldset;
  /** ID */
  fieldset << "o_id AS invoice_id";
  /** Artikel */
  fieldset << "a_article_id AS article";
  /** Titel */
  fieldset << "SUBSTRING(a_title FROM 0 FOR 60) AS title";
  /** Mehrwertsteuer */
  fieldset << "o_vat_levels AS vat";
  /** Preis */
  fieldset << "a_sell_price AS price";
  /** Abgeschlossen */
  fieldset << "DATE(o_delivered) AS delivered";
  /** Auswahl */
  QString query("SELECT ");
  query.append(fieldset.join(", "));
  query.append(" FROM inventory_orders");
  query.append(" LEFT JOIN article_orders ON a_order_id=o_id");
  /** Abfrageklausel erstellen */
  QPair<QString, QString> p = m_datePicker->timestampRange();
  query.append(" WHERE o_payment_status=true AND");
  query.append(" o_order_status=5 AND");
  query.append(" o_delivered BETWEEN");
  query.append(" '" + p.first + "' AND '" + p.second + "'");
  query.append(" ORDER BY o_delivered;");
  return query;
}

void ReportingDialog::setSqlRangeQuery() {
  QString query = getSqlQueryString();
  m_previewTable->setQuery(query);
  setWindowModified(true);
  QString s_vol = tr("Sales Volume");
  s_vol.append(": ");
  s_vol.append(m_previewTable->salesVolume());
  s_vol.append(" ");
  s_vol.append(m_cfg->value("payment/currency", "€").toString());
  m_statusBar->showMessage(s_vol, 0);
}

const QJsonDocument ReportingDialog::getSqlQueryJson() {
  QJsonDocument doc;
  QString sql = getSqlQueryString();
  if (sql.isEmpty())
    return doc;

  QJsonObject main;
  main.insert("date", QDateTime::currentDateTime().toString(Qt::ISODate));
  main.insert("title", tr("Monthly payment report"));

  QJsonObject fields;
  fields.insert("article", tr("Article"));
  fields.insert("invoice_id", tr("Invoice"));
  fields.insert("price", tr("Price"));
  fields.insert("title", tr("Title"));
  fields.insert("vat", tr("VAT"));
  main.insert("header", QJsonValue(fields));

  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QSqlRecord record = q.record();
    QJsonArray rows;
    while (q.next()) {
      QJsonObject row;
      for (int r = 0; r < record.count(); r++) {
        QSqlField field = record.field(r);
        QString key = field.name();
        row.insert(key, q.value(key).toJsonValue());
      }
      rows.append(QJsonValue(row));
    }
    main.insert("payments", QJsonValue(rows));
  }
  doc.setObject(main);
  return doc;
}

bool ReportingDialog::saveDataExport() {
  bool status = false;
  QFileInfo target = getSaveFile();
  if (!target.dir().exists())
    return status;

  if (target.fileName().isEmpty())
    return status;

  // JSON
  QJsonDocument doc = getSqlQueryJson();
  if (!doc.isNull()) {
    QString filePath = target.filePath();
    filePath.append("_UTF-8");
    filePath.append(".json");
    QFile fp(filePath);
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setCodec("UTF-8");
      out << doc.toJson(QJsonDocument::Compact);
      out << "\n";
      fp.close();
      status = true;
    }
  }

  // CSV
  if (status) {
    QString header = m_previewTable->dataHeader();
    QStringList rows = m_previewTable->dataRows();
    if (rows.count() > 0) {
      QString filePath = target.filePath();
      filePath.append("_ISO-8859-15");
      filePath.append(".csv");
      QFile fp(filePath);
      if (fp.open(QIODevice::WriteOnly)) {
        QTextStream out(&fp);
        out.setCodec("ISO-8859-15");
        out << header + "\n";
        out << rows.join("\n");
        fp.close();
        status = true;
      }
    }
  }

  if (status)
    setWindowModified(false);

  return status;
}

const QFileInfo ReportingDialog::getSaveFile() {
  QDir p_dir(m_cfg->value("dirs/reports", QDir::homePath()).toString());

  QString filename = QDate::currentDate().toString("yyyy-MM");

  QFileInfo info;
  if (p_dir.exists()) {
    info.setFile(p_dir, filename);
    return info;
  }

  QFileDialog *d = new QFileDialog(this, tr("Save to ..."));
  d->setViewMode(QFileDialog::Detail);
  d->setOptions(QFileDialog::ShowDirsOnly);
  d->setDirectory(p_dir);
  d->setFileMode(QFileDialog::Directory);
  if (d->exec() == QDialog::Accepted) {
    QStringList dirs = d->selectedFiles();
    info.setFile(dirs.first(), filename);
  }
  return info;
}

void ReportingDialog::setPage(int index) {
  m_stackedWidget->setCurrentIndex(index);
  // Tabellen Ansichtsseite
  if (index == 1) {
    setSqlRangeQuery();
  }
}

void ReportingDialog::previousPage() {
  if (pageIndex() > 0)
    setPage((pageIndex() - 1));
}

void ReportingDialog::nextPage() {
  if (pageIndex() >= 0 && pageIndex() < m_stackedWidget->count())
    setPage((pageIndex() + 1));
}

void ReportingDialog::pageEntered(int i) {
  int c = m_stackedWidget->count();
  btn_prev->setEnabled((i > 0));
  btn_next->setEnabled(((i + 1) < c));
  btn_apply->setEnabled(((i + 1) == c));
}

void ReportingDialog::close() {
  if (isWindowModified()) {
    QString txt =
        tr("There are unsaved actions.<br>Do you really want to quit?");
    int ret = QMessageBox::question(this, tr("Unsaved Changes"), txt);
    if (ret == QMessageBox::No)
      return;
  }
  reject();
}

void ReportingDialog::apply() {
  if (saveDataExport()) {
    m_statusBar->showMessage(tr("Saved successfully!"), 1000);
    setWindowModified(false);
  }
}

void ReportingDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    return;
  }
  QDialog::keyPressEvent(e);
}

bool ReportingDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}
