// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_IMAGETREEPATHVIEW_H
#define ANTIQUACRM_PRIVATE_IMAGETREEPATHVIEW_H

#include <AGlobal>
#include <QFrame>
#include <QDir>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

class ALineEdit;
class ImageFileSource;

class ANTIQUACRM_LIBRARY ImageTreePathView final : public QFrame {
  Q_OBJECT

private:
  ALineEdit *m_edit;
  QPushButton *m_ac;

Q_SIGNALS:
  void sendSelected(const AntiquaCRM::ImageFileSource &);

public Q_SLOTS:
  void openDirectoryDialog();
  void setDirectory(const QDir &);

public:
  explicit ImageTreePathView(QWidget *parent = nullptr);

};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_IMAGETREEPATHVIEW_H
