// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "varioussearchedit.h"

VariousSearchEdit::VariousSearchEdit(QWidget *parent)
    : SearchLineEdit{parent} {}

const QString VariousSearchEdit::getSearch() { return text().trimmed(); }
