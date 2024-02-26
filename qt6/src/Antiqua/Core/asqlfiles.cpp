// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlfiles.h"
#include "aglobal.h"
#include "asettings.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

namespace AntiquaCRM {

ASqlFiles::ASqlFiles(const QString &file)
    : QFileInfo{ASettings::getDataDir("pgsql"), file + ".sql"} {
  p_content = QString();
}

const QRegularExpression ASqlFiles::commentPattern() {
  return QRegularExpression("(^|\\s)--.+$");
}

const QRegularExpression ASqlFiles::macrosPattern() {
  return QRegularExpression("@([A-Z_]+)@");
}

bool ASqlFiles::openTemplate() {
  if (!isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Missing or permission Denied!";
#else
    qWarning("Missing or permission Denied!");
#endif
    return false;
  }

  QFile fp(filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    p_content = QString();
    QTextStream stream(&fp);
    while (!stream.atEnd()) {
      QString buf = stream.readLine();
      if (!buf.contains(commentPattern()))
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
  if (p_content.contains(macrosPattern())) {
    qWarning("Unused template replacements!");
  }
  return p_content.trimmed();
}

const QString ASqlFiles::queryStatement(const QString &basename) {
  const QFileInfo _info(ASettings::getDataDir("pgsql"), basename + ".sql");
  QString _out;
  if (!_info.isReadable()) {
    qWarning("Sql file (%s) not found or Permission Denied!",
             qPrintable(basename));
    return _out;
  }

  QFile _fp(_info.filePath());
  if (_fp.open(QIODevice::ReadOnly)) {
    QTextStream stream(&_fp);
    while (!stream.atEnd()) {
      QString buf = stream.readLine();
      if (!buf.contains(commentPattern()))
        _out += " " + buf.trimmed();
    }
    _fp.close();
  }
  return _out.trimmed();
}

}; // namespace AntiquaCRM
