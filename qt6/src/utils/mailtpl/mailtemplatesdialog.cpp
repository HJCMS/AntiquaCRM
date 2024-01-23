// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailtemplatesdialog.h"
#include "mailtemplatestree.h"
#include "templateeditor.h"

#include <QLabel>
#include <QLayout>

MailTemplatesDialog::MailTemplatesDialog(QWidget *parent)
    : AntiquaCRM::ADialog{parent} {
  setObjectName("mail_tpl_dialog");
  setWindowTitle(tr("Template Editor") + "[*]");
  setMinimumSize(760, 500);

  m_frame = new QFrame(this);
  m_frame->setContentsMargins(1, 1, 1, 1);
  QGridLayout *gridLayout = new QGridLayout(m_frame);
  gridLayout->setContentsMargins(m_frame->contentsMargins());

  QLabel *m_title0 = new QLabel(m_frame);
  m_title0->setText(tr("Menu title") + ":");
  m_title0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  gridLayout->addWidget(m_title0, 0, 0, 1, 1);

  m_tb_title = new QLineEdit(m_frame);
  m_tb_title->setObjectName("tb_title");
  m_tb_title->setPlaceholderText(tr("Max allowed %1 characters.").arg(60));
  m_tb_title->setToolTip(tr("Menu title"));
  gridLayout->addWidget(m_tb_title, 0, 1, 1, 1);

  m_tb_caller = new QLineEdit(m_frame);
  m_tb_caller->setObjectName("tb_caller");
  m_tb_caller->setPlaceholderText("Macro");
  m_tb_caller->setReadOnly(true);
  m_tb_caller->setMinimumWidth(250);
  m_tb_caller->setStyleSheet("QLineEdit {background:transparent;}");
  gridLayout->addWidget(m_tb_caller, 0, 2, 1, 2);

  QLabel *m_title1 = new QLabel(m_frame);
  m_title1->setText(tr("Mail Subject") + ":");
  m_title1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  gridLayout->addWidget(m_title1, 1, 0, 1, 1);

  m_tb_subject = new QLineEdit(m_frame);
  m_tb_subject->setObjectName("tb_subject");
  m_tb_subject->setPlaceholderText(tr("Max allowed %1 characters.").arg(128));
  m_tb_subject->setToolTip(tr("eMail subject"));
  gridLayout->addWidget(m_tb_subject, 1, 1, 1, 1);

  m_tb_attachment = new QCheckBox(tr("Attachment required"), m_frame);
  m_tb_attachment->setObjectName("tb_attachment");
  m_tb_attachment->setToolTip(tr("Attachment is required for this template!"));
  gridLayout->addWidget(m_tb_attachment, 1, 2, 1, 1);

  m_tb_activ = new QCheckBox(tr("Activ"), m_frame);
  m_tb_activ->setObjectName("tb_activ");
  m_tb_activ->setToolTip(tr("Enable/Disable this Template."));
  m_tb_activ->setChecked(false);
  gridLayout->addWidget(m_tb_activ, 1, 3, 1, 1);

  m_frame->setLayout(gridLayout);
  layout->addWidget(m_frame);

  m_splitter = new AntiquaCRM::Splitter(this);
  layout->addWidget(m_splitter);
  layout->setStretch(1, 1);

  m_edit = new TemplateEditor(m_splitter);
  m_splitter->addLeft(m_edit);

  m_treeMenu = new MailTemplatesTree(m_splitter);
  m_splitter->addRight(m_treeMenu);

  btn_apply->setText(tr("Save"));
  btn_apply->setIcon(AntiquaCRM::antiquaIcon("action-save"));

  connect(m_treeMenu, SIGNAL(sendTemplate(const QJsonObject &)),
          SLOT(openTemplate(const QJsonObject &)));
  connect(btn_apply, SIGNAL(clicked()), SLOT(save()));
  connect(btn_reject, SIGNAL(clicked()), SLOT(reject()));
}

const QString MailTemplatesDialog::boolSet(const QString &field, bool b) const {
  QString _str(field.trimmed());
  if (b) {
    _str.append("=true");
  } else {
    _str.append("=false");
  }
  return _str;
}

void MailTemplatesDialog::openTemplate(const QJsonObject &data) {
  foreach (QString k, data.keys()) {
    QCheckBox *cb = m_frame->findChild<QCheckBox *>(k);
    if (cb != nullptr) {
      cb->setChecked(data.value(k).toBool());
      continue;
    }
    QLineEdit *le = m_frame->findChild<QLineEdit *>(k);
    if (le != nullptr) {
      le->setText(data.value(k).toString());
    }
  }
  m_edit->setBody(data.value("tb_message").toString());
  btn_apply->setEnabled(true);
}

void MailTemplatesDialog::save() {
  const QString caller(m_tb_caller->text());
  if (caller.isEmpty() || !caller.contains("_")) {
    m_statusBar->showMessage(tr("Nothing to save"));
    return;
  }

  QStringList _fields;
  _fields << boolSet("tb_attachment", m_tb_attachment->isChecked());
  _fields << boolSet("tb_activ", m_tb_activ->isChecked());

  QListIterator<QLineEdit *> it(m_frame->findChildren<QLineEdit *>(QString()));
  QString _sql("UPDATE ui_template_body SET ");
  while (it.hasNext()) {
    QLineEdit *e = it.next();
    if (e == nullptr || e->objectName().startsWith("tb_caller"))
      continue;

    QString _data = e->text().trimmed();
    if (!_data.isEmpty())
      _fields << QString("%1='%2'").arg(e->objectName(), _data);
  }
  _fields << QString("tb_message='%1'").arg(m_edit->getBody());
  _sql.append(_fields.join(","));
  _sql.append(" WHERE tb_caller='" + caller + "';");
  _fields.clear();

  m_sql->query(_sql);
  if (m_sql->lastError().isEmpty()) {
    m_treeMenu->init(m_sql);
    m_statusBar->showMessage(tr("Save successfully"));
  } else {
    m_statusBar->showMessage(tr("Update failed!"));
  }
}

int MailTemplatesDialog::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  if (!m_treeMenu->init(m_sql))
    return QDialog::Rejected;

  return QDialog::exec();
}
