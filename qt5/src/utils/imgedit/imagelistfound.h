// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_IMAGELISTFOUND_H
#define ANTIQUA_IMAGELISTFOUND_H

#include <QList>
#include <QTableWidget>
#include <QTableWidgetItem>

class SourceInfo;

/**
 * @class ImageListFound
 * @brief list multiple images
 * @ingroup _imgedit
 */
class ImageListFound final : public QTableWidget {
  Q_OBJECT

private:
  QString p_archivPath;
  QString p_archivUrn;
  void highlightRow(int row = -1);

Q_SIGNALS:
  void sendSelection(const SourceInfo &);

private Q_SLOTS:
  void findSelection();

public Q_SLOTS:
  void addSourceInfo(const SourceInfo &);
  void setSourceInfos(const QList<SourceInfo> &);
  void setSelected(const SourceInfo &);

public:
  explicit ImageListFound(QWidget *parent = nullptr);
};

#endif // ANTIQUA_IMAGELISTFOUND_H
