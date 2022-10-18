// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlfiles.h"
#include "aglobal.h"
#include "asettings.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

#include <QFile>
#include <QRegExp>
#include <QStandardPaths>
#include <QTextStream>

namespace AntiquaCRM {

static const QRegExp comment_pattern() { return QRegExp("(^|\\s)--.+$"); }

ASqlFiles::ASqlFiles(const QString &file)
    : QFileInfo{ASettings::getDataDir("pgsql"), file + ".sql"},
      comments{comment_pattern()} {
  p_content = QString();
}

bool ASqlFiles::openTemplate() {
  if (!isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Permission Denied!";
#else
    qWarning("Permission Denied!");
#endif
    return false;
  }
  QFile fp(filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    p_content = QString();
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    while (!stream.atEnd()) {
      QString buf = stream.readLine();
      if (!buf.contains(comments))
        p_content += " " + buf.trimmed();
    }
    fp.close();
    return true;
  }
  return false;
}

void ASqlFiles::setWhereClause(const QString &replacement) {
  if (p_content.isEmpty()) {
    qWarning("No Content: Did you forget to call openTemplate?");
    return;
  }
  QString buf(p_content);
  buf.replace("@SQL_WHERE_CLAUSE@", replacement);
  p_content = buf;
  buf.clear();
}

void ASqlFiles::setOrderBy(const QString &replacement) {
  if (p_content.isEmpty()) {
    qWarning("No Content: Did you forget to call openTemplate?");
    return;
  }
  QString buf(p_content);
  buf.replace("@SQL_ORDER_BY@", replacement);
  p_content = buf;
  buf.clear();
}

void ASqlFiles::setSorting(Qt::SortOrder order) {
  if (p_content.isEmpty()) {
    qWarning("No Content: Did you forget to call openTemplate?");
    return;
  }

  QString str = (order == Qt::AscendingOrder) ? "ASC" : "DESC";
  QString buf(p_content);
  buf.replace("@SQL_SORTING@", str);
  p_content = buf;
  buf.clear();
}

void ASqlFiles::setLimits(int limit) {
  if (p_content.isEmpty()) {
    qWarning("No Content: Did you forget to call openTemplate?");
    return;
  }

  QString buf(p_content);
  QString str = (limit < 1) ? "" : " LIMIT " + QString::number(limit);
  buf.replace("@SQL_LIMIT@", str);
  p_content = buf;
  buf.clear();
}

const QString ASqlFiles::getQueryContent() {
  QRegExp check("@([A-Z_]+)@");
  if (p_content.contains(check)) {
    qWarning("Unused template replacements!");
  }
  return p_content.trimmed();
}

const QString ASqlFiles::selectStatement(const QString &name) {
  QFileInfo info(ASettings::getDataDir("pgsql"), name + ".sql");
  QString out;
  if (!info.isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Permission Denied!";
#else
    qWarning("Permission Denied!");
#endif
    return out;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    while (!stream.atEnd()) {
      QString buf = stream.readLine();
      if (!buf.contains(comment_pattern()))
        out += " " + buf.trimmed();
    }
    fp.close();
  }
  return out.trimmed();
}

const QString ASqlFiles::queryStatement(const QString &name) {
  QFileInfo info(ASettings::getDataDir("pgsql"), name + ".sql");
  QString out;
  if (!info.isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Permission Denied!";
#else
    qWarning("Permission Denied!");
#endif
    return out;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    while (!stream.atEnd()) {
      QString buf = stream.readLine();
      if (!buf.contains(comment_pattern()))
        out += " " + buf.trimmed();
    }
    fp.close();
  }
  return out.trimmed();
}

}; // namespace AntiquaCRM
