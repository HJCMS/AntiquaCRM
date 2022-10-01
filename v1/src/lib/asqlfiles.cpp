// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlfiles.h"
#include "aglobal.h"
#include "asettings.h"

#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QStandardPaths>
#include <QTextStream>

namespace AntiquaCRM {

ASqlFiles::ASqlFiles(const QString &file)
    : QFileInfo{ASettings::getDataDir("pgsql"), file + ".sql"} {
  p_content = QString();
}

bool ASqlFiles::openTemplate() {
  if (!isReadable()) {
    qWarning("Permission Denied!");
    return false;
  }
  QFile fp(filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    p_content = QString();
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    p_content = stream.readAll();
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
    qWarning("Permission Denied!");
    return out;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    out = stream.readAll();
    fp.close();
  }
  return out.trimmed();
}

const QString ASqlFiles::queryStatement(const QString &name) {
  QFileInfo info(ASettings::getDataDir("pgsql"), name + ".sql");
  QString out;
  if (!info.isReadable()) {
    qWarning("Permission Denied!");
    return out;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    out = stream.readAll();
    fp.close();
  }
  return out.trimmed();
}

}; // namespace AntiquaCRM
