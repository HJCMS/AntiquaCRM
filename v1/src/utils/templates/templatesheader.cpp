
#include "templatesheader.h"

#include <QJsonValue>
#include <QLayout>

TemplatesHeader::TemplatesHeader(QWidget *parent) : QFrame{parent} {
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setColumnStretch(1, 1);

  QLabel *lb_title = new QLabel(this);
  lb_title->setText(tr("Menue entry") + ":");
  layout->addWidget(lb_title, 0, 0, 1, 1, (Qt::AlignRight | Qt::AlignVCenter));

  m_title = new QLineEdit(this);
  m_title->setObjectName("tb_title");
  m_title->setPlaceholderText(tr("Max allowed %1 characters.").arg(60));
  m_title->setToolTip(tr("Menue title"));
  layout->addWidget(m_title, 0, 1, 1, 1);

  m_caller = new QLineEdit(this);
  m_caller->setObjectName("tb_caller");
  m_caller->setPlaceholderText("Macro");
  m_caller->setReadOnly(true);
  m_caller->setMinimumWidth(250);
  m_caller->setStyleSheet("QLineEdit {background:transparent;}");
  layout->addWidget(m_caller, 0, 2, 1, 1);

  QLabel *lb_subject = new QLabel(this);
  lb_subject->setText(tr("Subject") + ":");
  layout->addWidget(lb_subject, 1, 0, 1, 1,
                    (Qt::AlignRight | Qt::AlignVCenter));

  m_subject = new QLineEdit(this);
  m_subject->setObjectName("tb_subject");
  m_subject->setPlaceholderText(tr("Max allowed %1 characters.").arg(128));
  m_subject->setToolTip(tr("eMail subject"));
  layout->addWidget(m_subject, 1, 1, 1, 1);

  QHBoxLayout *hLayout = new QHBoxLayout();
  m_attachment = new QCheckBox(tr("Attachment required"), this);
  m_attachment->setObjectName("tb_attachment");
  m_attachment->setToolTip(tr("Attachment is required for this template!"));
  hLayout->addWidget(m_attachment);

  m_activ = new QCheckBox(tr("Activ"), this);
  m_activ->setObjectName("tb_activ");
  m_activ->setToolTip(tr("Enable/Disable this Template."));
  m_activ->setChecked(false);
  hLayout->addWidget(m_activ);
  layout->addLayout(hLayout, 1, 2, 1, 1, Qt::AlignRight);

  setLayout(layout);
}

void TemplatesHeader::setSelection(const QJsonObject &obj) {
  foreach (QString k, obj.keys()) {
    QCheckBox *cb = findChild<QCheckBox *>(k);
    if (cb != nullptr) {
      cb->setChecked(obj.value(k).toBool());
      continue;
    }

    QLineEdit *le = findChild<QLineEdit *>(k);
    if (le != nullptr) {
      le->setText(obj.value(k).toString());
    }
  }
}

const QJsonObject TemplatesHeader::getHeaderData() {
  QJsonObject obj;
  obj.insert("tb_attachment", m_attachment->isChecked());
  obj.insert("tb_activ", m_activ->isChecked());
  QListIterator<QLineEdit *> it(findChildren<QLineEdit *>(QString()));
  while (it.hasNext()) {
    QLineEdit *e = it.next();
    if (e != nullptr) {
      QString str = e->text().trimmed();
      if (!str.isEmpty())
        obj.insert(e->objectName(), e->text());
    }
  }
  return obj;
}
