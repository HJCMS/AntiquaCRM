// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationsdialog.h"
#include "designationedit.h"
#include "designationslist.h"

#include <QDebug>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>

DesignationsDialog::DesignationsDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("designations_edit_dialog");
  setSizeGripEnabled(true);
  setWindowTitle(tr("Edit Designation") + "[*]");
  setMinimumSize(550, 500);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_label = new QLabel(this);
  m_label->setText(tr("Edit predefined book cover description."));
  layout->addWidget(m_label);

  m_splitter = new QSplitter(Qt::Horizontal, this);
  layout->addWidget(m_splitter);
  layout->setStretch(1, 1);

  m_edit = new DesignationEdit(m_splitter);
  m_splitter->insertWidget(0, m_edit);

  m_listView = new DesignationsList(m_splitter);
  m_splitter->insertWidget(1, m_listView);

  m_btnBox = new QDialogButtonBox(this);
  QPushButton *btn_close = m_btnBox->addButton(QDialogButtonBox::Close);
  btn_close->setText(tr("Close"));
  btn_close->setIcon(QIcon("://icons/action_quit.png"));
  layout->addWidget(m_btnBox);

  setLayout(layout);
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));

  connect(m_listView, SIGNAL(sendEditItem(const DesignationData &)), m_edit,
          SLOT(setEditItem(const DesignationData &)));

  connect(m_edit, SIGNAL(sendSaveQuery(const QString &)),
          SLOT(setSaveQuery(const QString &)));
}

bool DesignationsDialog::loadBookDesignations() {
  AntiquaCRM::ASqlFiles sqlFile("query_designation_edit");
  if (!sqlFile.openTemplate())
    return false;

  QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
  if (q.size() > 0) {
    m_listView->clear();
    m_listView->insertQuery(q);
    return true;
  }
  return false;
}

void DesignationsDialog::setSaveQuery(const QString &sql) {
  m_sql->query(sql);
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << sql << m_sql->lastError();
#endif
  if (m_sql->lastError().isEmpty()) {
    m_edit->setMessage(tr("Saving success!"));
    loadBookDesignations();
  } else {
    m_edit->setMessage(tr("An error has occurred!"));
  }
}

void DesignationsDialog::resizeEvent(QResizeEvent *e) {
  if (e->type() == QEvent::Resize) {
    m_cfg->beginGroup("dialog/designation");
    m_cfg->setValue("geometry", saveGeometry());
    m_cfg->setValue("windowState", m_splitter->saveState());
    m_cfg->endGroup();
  }
  QDialog::resizeEvent(e);
}

int DesignationsDialog::exec() {
  m_cfg = new AntiquaCRM::ASettings(this);
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!loadBookDesignations())
    return QDialog::Rejected;

  if (m_cfg->contains("dialog/designation/geometry")) {
    m_cfg->beginGroup("dialog/designation");
    restoreGeometry(m_cfg->value("geometry").toByteArray());
    if (m_cfg->contains("windowState"))
      m_splitter->restoreState(m_cfg->value("windowState").toByteArray());
    m_cfg->endGroup();
  }
  return QDialog::exec();
}

DesignationsDialog::~DesignationsDialog() {
  if (m_cfg != nullptr)
    m_cfg->deleteLater();

  if (m_sql != nullptr)
    m_sql->deleteLater();
}
