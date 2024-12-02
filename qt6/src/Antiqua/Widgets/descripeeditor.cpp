// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "descripeeditor.h"
#include "splitter.h"
#include "antiquaicon.h"

#include <QDebug>

namespace AntiquaCRM
{

DescripeEditorItem::DescripeEditorItem(QListWidget* parent)
    : QListWidgetItem{parent, QListWidgetItem::UserType} {
  p_keyword = QString();
}

void DescripeEditorItem::setKeyword(const QString& keyword) {
  setData(Qt::ToolTipRole, keyword.left(80) + " ...");
  p_keyword = keyword;
}

const QString DescripeEditorItem::getKeyword() {
  return p_keyword;
}

void DescripeEditorItem::setLang(const QString& lng) {
  if(lng.contains("de_DE")) {
    setIcon(AntiquaCRM::antiquaIcon("de_DE"));
  } else {
    setIcon(AntiquaCRM::antiquaIcon("european-flag"));
  }
}

DescripeEditor::DescripeEditor(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  setObjectName("description_edit");

  m_splitter = new Splitter(this);
  layout->addWidget(m_splitter);
  layout->setStretchFactor(m_splitter, 1);

  m_edit = new AntiquaCRM::ATextEdit(this);
  m_splitter->addLeft(m_edit);
  m_splitter->setStretchFactor(0, 1);

  m_list = new QListWidget(this);
  m_list->setToolTip(tr("Predefined text blocks"));
  m_splitter->addRight(m_list);
  m_splitter->setStretchFactor(1, 0);

  initData();

  connect(m_edit, SIGNAL(textChanged()), SLOT(valueChanged()));
  connect(m_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(appendText(QListWidgetItem*)));
}

void DescripeEditor::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
}

void DescripeEditor::valueChanged() {
  setWindowModified(true);
  emit sendInputChanged();
}

void DescripeEditor::appendText(QListWidgetItem* item) {
  DescripeEditorItem* _item = static_cast<DescripeEditorItem*>(item);
  const QString _data = _item->getKeyword();
  m_edit->setFocus();
  m_edit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

  qsizetype _p = _data.indexOf(QChar('\n'), 0, Qt::CaseInsensitive);
  if (m_edit->find(_data.left(_p), QTextDocument::FindBackward)) {
    return;
  }

  QString _buf;
  if (m_edit->text().length() > 0) {
    _buf = m_edit->text().trimmed();
    _buf.append(", ");
    _buf.append(_data);
  } else {
    _buf.append(_data);
  }
  m_edit->clear();
  m_edit->setText(_buf.trimmed());
  _buf.clear();
}

void DescripeEditor::setValue(const QVariant& value) {
  if (value.metaType().id() == QMetaType::QString) {
    m_edit->setText(value.toString());
  } else {
    qWarning("Invalid value add to DescripeEditor");
  }
}

void DescripeEditor::setFocus() {
  m_edit->setFocus();
}

void DescripeEditor::reset() {
  m_edit->setText(QString());
  m_edit->clear();
  setWindowModified(false);
}

void DescripeEditor::setWordsList(const QStringList& list) {
  if (list.size() < 1)
    return;

  if (m_list->count() > 0)
    m_list->clear();

  foreach (QString l, list) {
    m_list->addItem(l);
  }
}

void DescripeEditor::setWordsList(AntiquaCRM::ArticleType t) {
  switch (t) {
    case (AntiquaCRM::ArticleType::BOOK):
    case (AntiquaCRM::ArticleType::MEDIA):
    case (AntiquaCRM::ArticleType::PRINTS):
    case (AntiquaCRM::ArticleType::OTHER):
      break;

    default:
      return;
  }

  AntiquaCRM::ASharedDataFiles dataFiles;
  QJsonDocument doc = dataFiles.getJson("descripeeditor");
  if (doc.isEmpty()) {
    qWarning("DescripeEditor::descripeeditor.json not found!");
    return;
  }

  if (m_list->count() > 0)
    m_list->clear();

  QJsonArray _arr = doc.object().value("descripeeditor").toArray();
  for (int i = 0; i < _arr.count(); i++) {
    QJsonObject _obj = _arr[i].toObject();
    if (_obj.value("type").toInt() == t) {
      DescripeEditorItem* _i = new DescripeEditorItem(m_list);
      _i->setText(_obj.value("name").toString());
      _i->setKeyword(_obj.value("description").toString());
      _i->setLang(_obj.value("lang").toString());
      m_list->addItem(_i);
    }
  }
  setWindowModified(false);
}

void DescripeEditor::setRestrictions(const QSqlField& field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void DescripeEditor::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void DescripeEditor::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool DescripeEditor::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QMetaType DescripeEditor::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant DescripeEditor::getValue() {
  return m_edit->text();
}

const QString DescripeEditor::popUpHints() {
  return tr("A valid Description is required.");
}

const QString DescripeEditor::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
