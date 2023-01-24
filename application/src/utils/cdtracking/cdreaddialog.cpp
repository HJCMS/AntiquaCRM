// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdreaddialog.h"
#include "cddiscid.h"
#include "discinfo.h"

#include <QDebug>
#include <QJsonParseError>
#include <QJsonValue>
#include <QLayout>
#include <QUrl>

CDReadDialog::CDReadDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("CD title reader"));
  setMinimumSize(500, 450);
  setContentsMargins(2, 2, 2, 2);
  setSizeGripEnabled(true);

  m_cfg = new AntiquaCRM::ASettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new QToolBar(this);
  layout->addWidget(m_toolBar);

  m_hwInfo = new QTextEdit(this);
  m_hwInfo->setReadOnly(true);
  // m_hwInfo->setStyleSheet("QTextEdit {border:none;
  // background:transparent;}");
  layout->addWidget(m_hwInfo);

  m_centralWidget = new QWidget(this);
  layout->addWidget(m_centralWidget);

  layout->addStretch(1);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_btnBox, SIGNAL(accepted()), SLOT(accept()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
}

const QJsonObject CDReadDialog::getRelease(const QJsonArray &array) {
  QStringList iso_lang({"DE", "EN", "US"});
  foreach (QString lng, iso_lang) {
    for (int i = 0; i < array.size(); i++) {
      QJsonObject obj = array[i].toObject();
      if (obj.value("country").toString().toUpper() == lng) {
        return obj;
        break;
      }
    }
  }
  return QJsonObject();
}

void CDReadDialog::getCDInfo() {
  CDDiscId *m_discid = new CDDiscId(this);
  connect(m_discid, SIGNAL(finished()), m_discid, SLOT(deleteLater()));
  connect(m_discid, SIGNAL(sendStatusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
  connect(m_discid, SIGNAL(sendQueryDiscChanged(const QUrl &)),
          SLOT(setQueryDiscId(const QUrl &)));

  m_hwInfo->clear();
  m_discid->start();
}

void CDReadDialog::queryResponses() {
  QJsonDocument doc;
  QJsonParseError parseHandle;
  QFile fp("/home/heinemann/.cache/w.FOgXeU6WxoNyhaFBBbNjLZBB4-.json");
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream data(&fp);
    data.setCodec(ANTIQUACRM_TEXTCODEC);
    QByteArray buffer = data.readAll().toLocal8Bit();
    doc = QJsonDocument::fromJson(buffer, &parseHandle);
    if (parseHandle.error != QJsonParseError::NoError) {
      qWarning("Json Document Error: '%s'.",
               qPrintable(parseHandle.errorString()));
      doc = QJsonDocument();
    }
    fp.close();
    buffer.clear();
  }

  QJsonObject js = doc.object();
  if (js.size() < 2)
    return;

  QJsonObject release;
  foreach (QString k, js.keys()) {
    if (k == "releases" && js.value(k).type() == QJsonValue::Array) {
      QJsonArray array = js.value(k).toArray();
      release = getRelease(array);
      break;
    }
  }

  if (!release.isEmpty()) {
    DiscInfo p_disc(release);
    qDebug() << Q_FUNC_INFO
             << p_disc.getTitle()
             << p_disc.getArtists()
             << p_disc.getBarcode()
             << p_disc.getTracks().size()
             << p_disc.getReleaseYear();
  }
}

void CDReadDialog::setQueryDiscId(const QUrl &url) {
  Q_UNUSED(url);
  //#ifndef ANTIQUA_DEVELOPEMENT
  //  qDebug() << Q_FUNC_INFO << url.toString();
  //#endif
  queryResponses();
}

int CDReadDialog::exec() {
  getCDInfo();
  return QDialog::exec();
}

CDReadDialog::~CDReadDialog() {}
