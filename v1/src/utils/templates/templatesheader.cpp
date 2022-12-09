
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
  layout->addWidget(m_subject, 1, 1, 1, 1);

  m_attachment = new QCheckBox(tr("Attachment required"), this);
  m_attachment->setObjectName("tb_attachment");
  layout->addWidget(m_attachment, 1, 2, 1, 1, Qt::AlignRight);

  setLayout(layout);
}

void TemplatesHeader::setSelection(const QJsonObject &obj) {
  foreach (QString k, obj.keys()) {
    if (k == "tb_attachment") {
      m_attachment->setChecked(obj.value(k).toBool());
      continue;
    }
    QLineEdit *e = findChild<QLineEdit *>(k);
    if (e != nullptr) {
      e->setText(obj.value(k).toString());
    }
  }
}

const QJsonObject TemplatesHeader::getHeaderData() {
  QJsonObject obj;
  obj.insert("tb_attachment", m_attachment->isChecked());
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
