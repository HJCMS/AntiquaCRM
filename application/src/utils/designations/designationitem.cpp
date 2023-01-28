// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationitem.h"

DesignationItem::DesignationItem(const QString &title, QListWidget *parent)
    : QListWidgetItem{parent, QListWidgetItem::Type} {
  setText(title);
  p_description = QString();
}

void DesignationItem::setDescription(const QString &txt) {
  p_description = txt;
}

const QString DesignationItem::getDescription() { return p_description; }
