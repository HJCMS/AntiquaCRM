// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagetemplates.h"
#include "messageeditor.h"
#include "messagekeywordlist.h"
#include "templatesheader.h"
#include "templatestree.h"

#include <QIcon>
#include <QLayout>

MessageTemplates::MessageTemplates(QWidget *parent) : QDialog{parent} {
  setObjectName("message_templates_dialog");
  setWindowTitle(tr("Template Editor") + "[*]");
  setMinimumSize(780, 550);
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new TemplatesHeader(this);
  layout->addWidget(m_toolBar);

  m_splitter = new QSplitter(Qt::Horizontal, this);
  layout->addWidget(m_splitter); // #1

  m_editor = new MessageEditor(m_splitter);
  m_splitter->insertWidget(0, m_editor);

  m_tabWidget = new QTabWidget(this);

  m_tplTree = new TemplatesTree(m_tabWidget);
  m_tplTree->setWhatsThis(tr("Templates"));
  m_tabWidget->addTab(m_tplTree, QIcon("://icons/view_list.png"),
                      tr("Templates"));

  m_keysList = new MessageKeywordList(m_tabWidget);
  m_keysList->setWhatsThis(tr("Macros"));
  m_tabWidget->addTab(m_keysList, QIcon("://icons/spreadsheet.png"),
                      tr("Macros"));

  m_splitter->insertWidget(1, m_tabWidget);

  m_btnBox = new QDialogButtonBox(
      (QDialogButtonBox::Close | QDialogButtonBox::Save), this);
  btn_save = m_btnBox->button(QDialogButtonBox::Save);
  btn_save->setIcon(QIcon("://icons/action_save.png"));
  btn_save->setText(tr("Save"));
  QPushButton *btn_close = m_btnBox->button(QDialogButtonBox::Close);
  btn_close->setIcon(QIcon("://icons/action_quit.png"));
  btn_close->setText(tr("Close"));
  layout->addWidget(m_btnBox); // #2

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar); // #3

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_tplTree, SIGNAL(templateSelected(const QJsonObject &)),
          SLOT(setEditTemplate(const QJsonObject &)));
  connect(m_tplTree, SIGNAL(templateSelected(const QJsonObject &)), m_toolBar,
          SLOT(setSelection(const QJsonObject &)));
  connect(btn_save, SIGNAL(clicked()), SLOT(saveCurrentTemplate()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
}

bool MessageTemplates::createMacrosTree() {
  AntiquaCRM::ASqlFiles file("union_template_keys");
  if (file.openTemplate()) {
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        QJsonObject obj;
        obj.insert("title", QJsonValue(q.value("title").toString()));
        obj.insert("key", QJsonValue(q.value("macro").toString()));
        m_keysList->addKey(q.value("category").toString(), obj);
      }
    } else if (!m_sql->lastError().isEmpty()) {
      m_statusBar->showMessage(tr("an error occurred"));
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
      return false;
    }
  }
  return true;
}

bool MessageTemplates::createTemplateTree() {
  m_tplData = new AntiquaCRM::ASqlDataQuery("ui_template_body");
  QString sql("SELECT * FROM ui_template_body");
  sql.append(" ORDER BY (tb_category, tb_title);");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QJsonObject objTree;
    QJsonArray array;
    while (q.next()) {
      QJsonObject child;
      foreach (QString f, m_tplData->columnNames()) {
        QVariant value = q.value(f);
        if (!value.isNull())
          child.insert(f, QJsonValue::fromVariant(value));
      }
      array.append(child);
    }
    objTree.insert("tree", array);
    m_tplTree->addTemplates(objTree);
  } else if (!m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("an error occurred"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    return false;
  }
  return true;
}

void MessageTemplates::saveCurrentTemplate() {
  QJsonObject data = m_toolBar->getHeaderData();
  data.insert("tb_message", m_editor->toPlainText());

  QString tb_caller = data.value("tb_caller").toString();
  AntiquaCRM::ASqlDataQuery dataTable("ui_template_body");
  foreach (QString f, data.keys()) {
    dataTable.setValue(f, data.value(f).toVariant());
  }

  QStringList set;
  QStringList ignore({"tb_caller", "tb_category"});
  QHashIterator<QString, QVariant> it(dataTable.getDataset());
  while (it.hasNext()) {
    it.next();
    QString _key = it.key();
    QVariant _val = it.value();
    if (ignore.contains(it.key()) || _val.isNull())
      continue;

    switch (dataTable.getType(_key).id()) {
    case QMetaType::QString:
      set.append(_key + "='" + _val.toString() + "'");
      break;

    case QMetaType::Bool: {
      QString b = _val.toBool() ? "true" : "false";
      set.append(_key + "=" + b);
    } break;

    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::Double:
      set.append(_key + "=" + QString::number(_val.toInt()));
      break;

    default:
      break;
    };
  }

  QString sql("UPDATE ui_template_body SET ");
  sql.append(set.join(","));
  sql.append(" WHERE tb_caller='" + tb_caller + "';");
  // qDebug() << Q_FUNC_INFO << sql;
  m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("Save data successfully!"), (1000 * 6));
  } else {
    m_statusBar->showMessage(tr("an error occurred"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << sql << m_sql->lastError();
#endif
  }

  if (m_tplTree->clearTree())
    createTemplateTree();
}

void MessageTemplates::setEditTemplate(const QJsonObject &obj) {
  if (obj.isEmpty())
    return;

  QStringList keys = obj.keys();
  m_editor->setPlainText(obj.value("tb_message").toString());
}

int MessageTemplates::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  if (!createMacrosTree())
    return QDialog::Rejected;

  if (!createTemplateTree())
    return QDialog::Rejected;

  m_keysList->expandAll();
  m_tplTree->expandAll();

  return QDialog::exec();
}
