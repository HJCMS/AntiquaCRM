// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnresults.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "isbnrequest.h"
#include "myicontheme.h"

#include <QApplication>
#include <QClipboard>
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
  }
  m_isbnRequest = new IsbnRequest(query, this);
  connect(m_isbnRequest, SIGNAL(requestFinished(bool)), this,
          SLOT(setIsbnInfo(bool)));

  m_isbnRequest->triggerRequest();
}

void ISBNResults::setIsbnInfo(bool b) {
  clear();

  Qt::ItemFlags flags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                      Qt::ItemNeverHasChildren);

  if (!b) {
    QString ref_isbn = p_isbn.toString();

    QString bookfinder_url("https://www.bookfinder.com/?mode=isbn&isbn=");
    bookfinder_url.append(ref_isbn);
    QListWidgetItem *bookfinder = new QListWidgetItem(this);
    bookfinder->setData(Qt::DisplayRole,
                        tr("No Result: Search with %1").arg("Bookfinder.com"));
    bookfinder->setIcon(myIcon("html"));
    bookfinder->setFlags(flags);
    bookfinder->setData(Qt::UserRole, "ib_website:" + bookfinder_url);
    addItem(bookfinder);

    QString gsearch;
    gsearch.append("https://books.google.com/advanced_book_search?lr=lang_de");
    gsearch.append("&hl=de&isbn=" + ref_isbn);

    QListWidgetItem *google = new QListWidgetItem(this);
    google->setData(Qt::DisplayRole,
                    tr("No Result: Search with %1").arg("Google"));
    google->setIcon(myIcon("html"));
    google->setFlags(flags);
    google->setData(Qt::UserRole, "ib_website:" + gsearch);
    addItem(google);

    emit requestFinished();
  }

  if (m_isbnRequest == nullptr)
    return;

  const QMap<QString, QVariant> isbnData = m_isbnRequest->getResponse();
  if (isbnData.size() < 1)
    return;

  // ib_title
  QListWidgetItem *title = new QListWidgetItem(this);
  QString title_txt = isbnData.value("title").toString();
  title->setData(Qt::DisplayRole, tr("Booktitle") + ": " + title_txt);
  title->setData(Qt::UserRole, "ib_title:" + title_txt);
  title->setIcon(myIcon("edit"));
  title->setFlags(flags);
  addItem(title);

  if (!isbnData.value("title_extended").isNull()) {
    QListWidgetItem *title_ex = new QListWidgetItem(this);
    QString title_ext_txt = isbnData.value("title_extended").toString();
    title_ex->setData(Qt::DisplayRole, tr("Subtitle") + ": " + title_ext_txt);
    title_ex->setData(Qt::UserRole, "ib_title_extended:" + title_ext_txt);
    title_ex->setIcon(myIcon("edit"));
    title_ex->setFlags(flags);
    addItem(title_ex);
  }

  if (!isbnData.value("authors").isNull()) {
    QListWidgetItem *author = new QListWidgetItem(this);
    QString authors = isbnData.value("authors").toString();
    author->setData(Qt::DisplayRole, tr("Authors") + ": " + authors);
    author->setData(Qt::UserRole, "ib_author:" + authors);
    author->setIcon(myIcon("edit_group"));
    author->setFlags(flags);
    addItem(author);
  }

  if (!isbnData.value("year").isNull()) {
    QListWidgetItem *year = new QListWidgetItem(this);
    QString year_txt = isbnData.value("year").toString();
    year->setData(Qt::DisplayRole, tr("Year") + ": " + year_txt);
    year->setData(Qt::UserRole, "ib_year:" + year_txt);
    year->setIcon(myIcon("edit"));
    year->setFlags(flags);
    addItem(year);
  }

  if (!isbnData.value("publisher").isNull()) {
    QString publisher_txt = isbnData.value("publisher").toString();
    QListWidgetItem *publisher = new QListWidgetItem(this);
    publisher->setData(Qt::DisplayRole, tr("Publisher") + ": " + publisher_txt);
    publisher->setData(Qt::UserRole, "ib_publisher:" + publisher_txt);
    publisher->setIcon(myIcon("group"));
    publisher->setFlags(flags);
    addItem(publisher);
  }

  if (!isbnData.value("url").isNull()) {
    QListWidgetItem *website = new QListWidgetItem(this);
    website->setData(Qt::DisplayRole,
                     tr("Open Webpage in Browser for full Description."));
    website->setData(Qt::UserRole,
                     "ib_website:" + isbnData.value("url").toString());
    website->setIcon(myIcon("html"));
    website->setToolTip(tr("External Book Description"));
    website->setFlags(flags);
    addItem(website);
  }

  if (!isbnData.value("pages").isNull()) {
    QListWidgetItem *pages = new QListWidgetItem(this);
    QString pages_txt = isbnData.value("pages").toString();
    pages->setData(Qt::DisplayRole, tr("Pages") + ": " + pages_txt);
    pages->setData(Qt::UserRole, "ib_pagecount:" + pages_txt);
    pages->setIcon(myIcon("edit"));
    pages->setFlags(flags ^ Qt::ItemIsEnabled);
    addItem(pages);
  }

  if (isbnData.value("images").toBool()) {
    QListWidgetItem *graphs = new QListWidgetItem(this);
    graphs->setData(Qt::DisplayRole, tr("An image exists on OpenLibrary.org"));
    graphs->setData(Qt::UserRole, true);
    graphs->setIcon(myIcon("image"));
    graphs->setFlags(flags ^ Qt::ItemIsEnabled);
    addItem(graphs);
  }

  QListWidgetItem *donate = new QListWidgetItem(this);
  donate->setData(Qt::DisplayRole,
                  tr("OpenLibrary is free to use, but we need your Help!"));
  donate->setData(Qt::UserRole, "ib_website:https://archive.org/donate/");
  donate->setIcon(myIcon("html"));
  donate->setFlags(flags);
  addItem(donate);

  emit requestFinished();
}

void ISBNResults::copySelectedItem() {
  QListWidgetItem *item = currentItem();
  if(item != nullptr) {
    QString buffer(item->text().trimmed());
    QRegExp strip("([\\'\\\";:\\[\\]\\{\\}]+)");
    buffer.replace(strip,"");
    buffer = buffer.trimmed();
    if(!buffer.isEmpty())
      qApp->clipboard()->setText(buffer, QClipboard::Clipboard);
  }
}

void ISBNResults::contextMenuEvent(QContextMenuEvent *ev) {
  QMenu *m = new QMenu("Actions", this);
  // Von Eintrag die ID Kopieren
  QAction *ac_copy = m->addAction(myIcon("edit"), tr("Copy Article Id"));
  ac_copy->setObjectName("ac_context_copy_book");
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copySelectedItem()));

  m->exec(ev->globalPos());
  delete m;
}

ISBNResults::~ISBNResults() {
  p_isbn.clear();
  clear();
}
