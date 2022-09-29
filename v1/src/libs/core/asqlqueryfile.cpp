// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlqueryfile.h"

#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QStandardPaths>
#include <QTextStream>

namespace AntiquaCRM {

static const QString subDirs() {
  QString p;
  p.append(QDir::separator());
  p.append("data");
  p.append(QDir::separator());
  p.append("pgsql");
  return p;
}

static const QDir templatesDir() {
  // Windows and Developement
  QDir sourceDir(QDir::currentPath() + subDirs());
  if (sourceDir.isReadable())
    return sourceDir;

  // Unix
  QStringList list =
      QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  foreach (QString d, list) {
    qDebug() << Q_FUNC_INFO << d << subDirs();
    QDir test(d + subDirs());
    if (test.isReadable()) {
      sourceDir.setPath(d + subDirs());
      if (sourceDir.isReadable())
        break;
    }
  }
  return sourceDir;
}

ASqlQueryFile::ASqlQueryFile(const QString &file)
    : QFileInfo{templatesDir(), file + ".sql"} {
  p_content = QString();
}

bool ASqlQueryFile::openTemplate() {
  if (!isReadable()) {
    qWarning("Permission Denied!");
    return false;
  }
  QFile fp(filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    p_content = QString();
    QTextStream stream(&fp);
    stream.setCodec("UTF8");
    p_content = stream.readAll();
    fp.close();
    return true;
  }
  return false;
}

void ASqlQueryFile::setWhereClause(const QString &replacement) {
  if (p_content.isEmpty()) {
    qWarning("No Content: Did you forget to call openTemplate?");
    return;
  }
  QString buf(p_content);
  buf.replace("@SQL_WHERE_CLAUSE@", replacement);
  p_content = buf;
  buf.clear();
}

void ASqlQueryFile::setOrderBy(const QString &replacement) {
  if (p_content.isEmpty()) {
    qWarning("No Content: Did you forget to call openTemplate?");
    return;
  }
  QString buf(p_content);
  buf.replace("@SQL_ORDER_BY@", replacement);
  p_content = buf;
  buf.clear();
}

void ASqlQueryFile::setSorting(Qt::SortOrder order) {
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

void ASqlQueryFile::setLimits(int limit) {
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

const QString ASqlQueryFile::getQueryContent() {
  QRegExp check("@([A-Z_]+)@");
  if (p_content.contains(check)) {
    qWarning("Unused template replacements!");
  }
  return p_content.trimmed();
}

}; // namespace AntiquaCRM
