// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumeroverview.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

Document::Document(QTextEdit *parent) : QTextDocument{parent} {
  setObjectName("AddressDocument");
  setMetaInformation(QTextDocument::DocumentTitle, tr("Costumer"));
  setDocumentMargin(10);
}

CostumerOverview::CostumerOverview(QWidget *parent) : QTextBrowser{parent} {
  setObjectName("CostumerOverview");
  setLineWrapMode(QTextEdit::WidgetWidth);
  setReadOnly(true);
  setOpenExternalLinks(true);
  setTextInteractionFlags(Qt::TextBrowserInteraction);

  document = new Document(this);

  QString css("p { page-break-before: always; }");
  document->setDefaultStyleSheet(css);

  QString html("<html><head>");
  html.append("<meta http-equiv=\"Content-Type\" content=\"text/html; "
              "charset=utf-8\">");
  html.append("</head><body><div>");
  html.append("<h3>@C_COMPANY_NAME@</h3><hr/>");
  html.append("<p><b>@FULLNAME@</b></p>");
  html.append("<p><address>@ADDRESS@</address></p><hr/>");
  html.append("<p>@PHONE@</p>");
  html.append("<p>@MAILTO@</p>");
  html.append("<p>@website@</p>");
  html.append("</div></body></html>");
  document->setHtml(html);

  setDocument(document);
  cursor = QTextCursor(document);
}

bool CostumerOverview::check(const QString &p, const QString &k,
                             const QVariant &v) {
  QString a(p.trimmed().toLower());
  QString b(k.trimmed().toLower());
  QString c(v.toString().trimmed());
  return ((a == b) && !c.isEmpty()) ? true : false;
}

void CostumerOverview::setLinkNode(const QString &key, const QVariant &value) {
  QString link(value.toString().trimmed());
  if (key == "mailto") {
    link.prepend("mailto:");
  } else if (key == "phone") {
    link.prepend("tel:");
  }
  QString html("<p>" + link + "</p>");
  html.append("</a>");
  html.prepend("<a href=\"" + link + "\" target=\"_blank\">");
  QString tag("@" + key.toUpper().trimmed() + "@");
  QTextCursor c = document->find(tag, cursor, QTextDocument::FindWholeWords);
  c.beginEditBlock();
  c.insertHtml(html);
  c.endEditBlock();
}

void CostumerOverview::setTextNode(const QString &key, const QVariant &value) {
  QString txt(value.toString().trimmed());
  QString tag("@" + key.toUpper().trimmed() + "@");
  QTextCursor c = document->find(tag, cursor, QTextDocument::FindWholeWords);
  c.beginEditBlock();
  c.insertText(txt);
  c.endEditBlock();
}

void CostumerOverview::setAddressNode(const QString &key,
                                      const QVariant &value) {
  QString tag("@" + key.toUpper().trimmed() + "@");
  QTextCursor c = document->find(tag, cursor, QTextDocument::FindWholeWords);

  QString html;
  foreach (QString l, value.toString().split("\n")) {
    html.append("<dd>" + l.trimmed() + "</dd>");
  }

  c.beginEditBlock();
  c.insertHtml("<dt>" + html + "</dt>");
  c.endEditBlock();
}

void CostumerOverview::setTextBlock(const QString &key, const QVariant &data) {
  if (key == "c_company_name") {
    setTextNode("c_company_name", data.toString());
    return;
  }
  if (check("fullname", key, data)) {
    setTextNode("fullname", data);
    return;
  }
  if (check("c_postal_address", key, data)) {
    setAddressNode("address", data);
    return;
  }
  if (check("c_email_0", key, data)) {
    setLinkNode("mailto", data);
    return;
  } else if (check("c_email_1", key, data)) {
    setLinkNode("mailto", data);
    return;
  }
  if (check("c_phone_0", key, data)) {
    setLinkNode("phone", data);
    return;
  } else if (check("c_phone_1", key, data)) {
    setLinkNode("phone", data);
    return;
  }
  if (check("c_website", key, data)) {
    setLinkNode("website", data);
    return;
  }
}
