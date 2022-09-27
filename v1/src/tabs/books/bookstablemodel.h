// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSTABLEMODEL_H
#define ANTIQUACRM_BOOKSTABLEMODEL_H

#include <QObject>
#include <QWidget>
#include <AntiquaCRM>

class BooksTableModel : public AntiquaCRM::ASqlQueryModel
{
  Q_OBJECT

public:
  explicit BooksTableModel(QObject *parent = nullptr);
};

#endif // ANTIQUACRM_BOOKSTABLEMODEL_H
