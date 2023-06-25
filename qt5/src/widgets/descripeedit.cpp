// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "descripeedit.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QSplitter>

DescripeEdit::DescripeEdit(QWidget *parent) : InputEdit{parent} {
  QSplitter *m_splitter = new QSplitter(Qt::Horizontal, this);

  m_edit = new QTextEdit(m_splitter);
  m_edit->setObjectName("TextField");
  m_edit->setAcceptDrops(false);
  m_edit->setAcceptRichText(false);
  m_edit->setTextInteractionFlags(Qt::TextEditorInteraction);
  m_edit->setAutoFormatting(QTextEdit::AutoNone);
  m_splitter->insertWidget(0, m_edit);

  m_list = new QListWidget(m_splitter);
  m_list->setToolTip(tr("Appends text blocks into the Description."));
  m_list->setSelectionMode(QAbstractItemView::SingleSelection);
  m_list->setAlternatingRowColors(true);
  m_splitter->insertWidget(1, m_list);

  m_splitter->setStretchFactor(0, 1);
  m_splitter->setStretchFactor(1, 0);
  m_layout->addWidget(m_splitter);

  loadDataset();
  setRequired(false);

  connect(m_edit, SIGNAL(textChanged()), SLOT(dataChanged()));

  connect(m_list, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          SLOT(addTextModule(QListWidgetItem *)));
}

void DescripeEdit::loadDataset() {
  QSqlField _f;
  _f.setRequiredStatus(QSqlField::Optional);
  setProperties(_f);

  QStringList _list;
  _list << tr("Nice clean copy");
  _list << tr("Clean copy");
  _list << tr("Copy a bit dusty");
  _list << tr("Year may differ from entry");
  _list << tr("Age-related traces");
  _list << tr("With bookplate");
  _list << tr("Richly illustrated");
  _list << tr("With ownership stamp");

  foreach (QString l, _list) {
    m_list->addItem(l);
  }

  setWindowModified(false);
}

void DescripeEdit::addTextModule(QListWidgetItem *item) {
  const QString _txt = item->text();
  m_edit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  if (m_edit->find(_txt, QTextDocument::FindBackward))
    return; // aussteigen

  QString _buf;
  if (m_edit->toPlainText().length() > 0) {
    _buf = m_edit->toPlainText().trimmed();
    _buf.append(" ");
    _buf.append(_txt);
    _buf.append(".");
  } else {
    _buf.append(_txt);
    _buf.append(".");
  }
  m_edit->clear();
  m_edit->setPlainText(_buf.trimmed());
  _buf.clear();
  m_edit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
  setWindowModified(true);
}

void DescripeEdit::dataChanged() { setWindowModified(true); }

void DescripeEdit::reset() {
  m_edit->setPlainText(QString());
  m_edit->clear();
  setModified(false);
}

void DescripeEdit::setValue(const QVariant &val) {
  QString txt = val.toString().trimmed();
  m_edit->setPlainText(txt);
}

void DescripeEdit::setFocus() {
  m_edit->setFocus();
}

void DescripeEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant DescripeEdit::value() {
  QString txt = m_edit->toPlainText();
  return stripString(txt);
}

bool DescripeEdit::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void DescripeEdit::setInfo(const QString &info) { m_edit->setToolTip(info); }

const QString DescripeEdit::info() { return m_edit->toolTip(); }

const QString DescripeEdit::notes() {
  return tr("Description requires a valid input.");
}
