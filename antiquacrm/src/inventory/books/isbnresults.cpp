// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnresults.h"
#include "isbnrequest.h"
#include "myicontheme.h"

#include <QApplication>
#include <QClipboard>
#include <QTextCodec>
#include <QtWidgets>

ISBNResults::ISBNResults(QWidget *parent) : QListWidget{parent} {
  setObjectName("isbn_results_list");
}

void ISBNResults::fetchIsbnData(const QVariant &isbn) {
  p_isbn = isbn;
  QString query = p_isbn.toString().trimmed();
  int l = query.length();
  if (l > 13 && l != 10) {
    return;
  } else if (l < 10) {
    return;
  }

  m_isbnRequest = new IsbnRequest(query, this);
  connect(m_isbnRequest, SIGNAL(requestFinished(bool)), this,
          SLOT(setIsbnInfo(bool)));

  m_isbnRequest->triggerRequest();
}

QListWidgetItem *ISBNResults::createItem(QListWidget *parent) const {
  Qt::ItemFlags flags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                      Qt::ItemNeverHasChildren);

  QListWidgetItem *item = new QListWidgetItem(parent);
  item->setFlags(flags);
  return item;
}

void ISBNResults::setIsbnInfo(bool b) {
  clear();

  if (!b) {
    QListWidgetItem *notFound = createItem(this);
    notFound->setData(Qt::DisplayRole, tr("No Result"));
    addItem(notFound);

    emit requestFinished();
    return;
  }

  if (m_isbnRequest == nullptr)
    return;

  const QMap<QString, QVariant> isbnData = m_isbnRequest->getResponse();
  if (isbnData.size() < 1)
    return;

  // ib_title
  QListWidgetItem *title = createItem(this);
  QString title_txt = isbnData.value("title").toString();
  title->setData(Qt::DisplayRole, tr("Booktitle") + ": " + title_txt);
  title->setData(Qt::UserRole, title_txt);
  addItem(title);

  if (!isbnData.value("title_extended").isNull()) {
    QListWidgetItem *title_ex = createItem(this);
    QString title_ext_txt = isbnData.value("title_extended").toString();
    title_ex->setData(Qt::DisplayRole, tr("Subtitle") + ": " + title_ext_txt);
    title_ex->setData(Qt::UserRole, title_ext_txt);
    addItem(title_ex);
  }

  if (!isbnData.value("authors").isNull()) {
    QListWidgetItem *author = createItem(this);
    QString authors = isbnData.value("authors").toString();
    author->setData(Qt::DisplayRole, tr("Authors") + ": " + authors);
    author->setData(Qt::UserRole, authors);
    addItem(author);
  }

  if (!isbnData.value("year").isNull()) {
    QListWidgetItem *year = createItem(this);
    QString year_txt = isbnData.value("year").toString();
    year->setData(Qt::DisplayRole, tr("Year") + ": " + year_txt);
    year->setData(Qt::UserRole, year_txt);
    addItem(year);
  }

  if (!isbnData.value("publisher").isNull()) {
    QString publisher_txt = isbnData.value("publisher").toString();
    QListWidgetItem *publisher = createItem(this);
    publisher->setData(Qt::DisplayRole, tr("Publisher") + ": " + publisher_txt);
    publisher->setData(Qt::UserRole, publisher_txt);
    addItem(publisher);
  }

  if (!isbnData.value("pages").isNull()) {
    QListWidgetItem *pages = createItem(this);
    QString pages_txt = isbnData.value("pages").toString();
    pages->setData(Qt::DisplayRole, tr("Pages") + ": " + pages_txt);
    pages->setData(Qt::UserRole, pages_txt);
    addItem(pages);
  }

  emit requestFinished();
}

void ISBNResults::copySelectedItem() {
  QListWidgetItem *item = currentItem();
  if (item != nullptr) {
    QString out;
    QVariant data = item->data(Qt::UserRole);
    QByteArray buf = data.toByteArray();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    out = codec->toUnicode(buf);
    if (!out.isEmpty())
      qApp->clipboard()->setText(out, QClipboard::Clipboard);
  }
}

void ISBNResults::contextMenuEvent(QContextMenuEvent *ev) {
  QMenu *m = new QMenu("Actions", this);
  // Von Eintrag die ID Kopieren
  QAction *ac_copy = m->addAction(myIcon("edit"), tr("Copy selected item row"));
  ac_copy->setObjectName("ac_context_copy_book");
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copySelectedItem()));

  m->exec(ev->globalPos());
  delete m;
}

ISBNResults::~ISBNResults() {
  p_isbn.clear();
  clear();
}
