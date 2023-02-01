// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdreaddialog.h"
#include "cddiscid.h"
#include "discinfo.h"
#include "trackslistwidget.h"

#include <AntiquaCRM>
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

  m_mainFrame = new QFrame(this);
  QGridLayout *gLayout = new QGridLayout(m_mainFrame);
  int grow = 0;
  // cv_title
  infolabel = new AntiquaILabel(tr("Title"), m_mainFrame);
  gLayout->addWidget(infolabel, grow, 0, 1, 1);
  m_title = new LineEdit(this);
  m_title->setObjectName("cv_title");
  m_title->setToolTip(infolabel->text());
  gLayout->addWidget(m_title, grow++, 1, 1, 1);
  // cv_author
  infolabel = new AntiquaILabel(tr("Artists"), m_mainFrame);
  gLayout->addWidget(infolabel, grow, 0, 1, 1);
  m_artists = new LineEdit(this);
  m_artists->setObjectName("cv_author");
  m_artists->setToolTip(infolabel->text());
  gLayout->addWidget(m_artists, grow++, 1, 1, 1);
  // cv_year
  infolabel = new AntiquaILabel(tr("Release"), m_mainFrame);
  gLayout->addWidget(infolabel, grow, 0, 1, 1);
  m_year = new YearEdit(m_mainFrame);
  m_year->setObjectName("cv_year");
  m_year->setRequired(true);
  m_year->setInfo(tr("Year"));
  m_year->setValue(1970);
  gLayout->addWidget(m_year, grow++, 1, 1, 1, Qt::AlignLeft);
  // cv_eangtin
  infolabel = new AntiquaILabel(tr("Barcode"), m_mainFrame);
  gLayout->addWidget(infolabel, grow, 0, 1, 1);
  m_barcode = new LineEdit(this);
  m_barcode->setObjectName("cv_eangtin");
  m_barcode->setToolTip(infolabel->text());
  gLayout->addWidget(m_barcode, grow++, 1, 1, 1);

  m_mainFrame->setLayout(gLayout);
  layout->addWidget(m_mainFrame);

  m_tracksList = new TracksListWidget(this);
  layout->addWidget(m_tracksList);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_btnBox, SIGNAL(accepted()), SLOT(accept()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
}

const QJsonObject CDReadDialog::getRelease(const QJsonArray &array) {
  QStringList country({"DE", "GB", "US"});
  foreach (QString lng, country) {
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

  m_discid->start();
}

void CDReadDialog::queryResponses(const QJsonDocument &doc) {
  QJsonObject js = doc.object();
  if (js.size() < 2)
    return;

#ifdef ANTIQUA_DEVELOPEMENT
  QFile fp(QDir::homePath() + "/.cache/discid_info.json");
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream data(&fp);
    data.setCodec(ANTIQUACRM_TEXTCODEC);
    data << doc.toJson(QJsonDocument::Indented);
    fp.close();
  }
#endif

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
    m_title->setValue(p_disc.getTitle());
    m_artists->setValue(p_disc.getArtists());
    m_year->setValue(p_disc.getReleaseYear());
    m_barcode->setValue(p_disc.getBarcode());
    m_tracksList->setTracks(p_disc.getTracks());
  }
}

void CDReadDialog::setQueryDiscId(const QUrl &url) {
  AntiquaCRM::PluginQueryType _t = AntiquaCRM::PluginQueryType::JSON_QUERY;
  AntiquaCRM::ANetworker *m_net = new AntiquaCRM::ANetworker(_t, this);
  connect(m_net, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(queryResponses(const QJsonDocument &)));

  m_net->getRequest(url);
}

int CDReadDialog::exec() {
  getCDInfo();
  m_tracksList->setFocus();
  return QDialog::exec();
}

const QJsonObject CDReadDialog::data() {
  QJsonObject obj;
  foreach (LineEdit *e, findChildren<LineEdit *>(QString())) {
    obj.insert(e->objectName(), e->value().toString());
  }
  obj.insert(m_year->objectName(), m_year->value().toInt());
  obj.insert("tracks", m_tracksList->getTracks());
  return obj;
}

CDReadDialog::~CDReadDialog() {}