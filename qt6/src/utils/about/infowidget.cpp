// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "infowidget.h"

#include <QFile>
#include <QPixmap>
#include <QSize>
#include <QTextStream>

InfoWidget::InfoWidget(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  layout = new QGridLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);

  lb_icon = new QLabel(this);
  lb_icon->setAlignment(Qt::AlignRight | Qt::AlignTop);
  lb_icon->setTextFormat(Qt::PlainText);
  lb_icon->setTextInteractionFlags(Qt::NoTextInteraction);
  layout->addWidget(lb_icon, 0, 0, 1, 1);

  lb_title = new QLabel(this);
  lb_title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  lb_title->setTextFormat(Qt::PlainText);
  lb_title->setTextInteractionFlags(Qt::NoTextInteraction);
  lb_title->setIndent(10);
  lb_title->setWordWrap(true);
  QFont ft = font();
  ft.setPointSize(16);
  ft.setBold(true);
  lb_title->setFont(ft);
  layout->addWidget(lb_title, 0, 1, 1, 1);

  lb_text = new QTextBrowser(this);
  lb_text->setStyleSheet("QTextEdit {background:transparent;}");
  lb_title->setTextFormat(Qt::RichText);
  lb_text->setTextInteractionFlags(Qt::TextBrowserInteraction);
  lb_text->setOpenExternalLinks(true);
  lb_text->setReadOnly(true);
  lb_text->setTabChangesFocus(false);
  lb_text->setFocusPolicy(Qt::NoFocus);
  layout->addWidget(lb_text, 1, 0, 1, 2);
  layout->setColumnStretch(1, 1);
  layout->setRowStretch(1, 1);

  lb_homepage = new QLabel(this);
  lb_homepage->setTextFormat(Qt::RichText);
  lb_homepage->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
  lb_homepage->setOpenExternalLinks(true);
  layout->addWidget(lb_homepage, 2, 0, 1, 2, Qt::AlignRight);

  setLayout(layout);
}

void InfoWidget::setInfoIcon(const QIcon &icon) {
  QPixmap pixmap = icon.pixmap(45, 45, QIcon::Normal, QIcon::On);
  lb_icon->setPixmap(pixmap);
}

void InfoWidget::setInfoTitle(const QString &title) {
  lb_title->setText(title);
}

void InfoWidget::setInfoText(const QString &rc_basename) {
  QFile fp("://" + rc_basename + ".txt");
  if (fp.open(QFile::ReadOnly)) {
    QString buffer("<div>");
    QTextStream in(&fp);
    while (!in.atEnd()) {
      buffer.append(in.readLine() + "<br>");
    }
    buffer.append("</div>");
    lb_text->setHtml(buffer);
  }
  fp.close();
}

void InfoWidget::setHomepage(const QUrl &url) {
  QString html("<a href='" + url.toString(QUrl::FullyEncoded) + "'>");
  html.append(url.toString());
  html.append("</a>");
  lb_homepage->setText(html);
}
