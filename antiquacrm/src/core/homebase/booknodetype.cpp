// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booknodetype.h"

BookNodeType::BookNodeType(const QString &typeName, QMetaType::Type mType,
                           int min, int max) {
  p_name = typeName;
  p_type = mType;
  p_minLength = min;
  p_maxLength = max;
}

BookNodeType::BookNodeType(const BookNodeType &p)
    : BookNodeType(p.name(), p.type(), p.minLength(), p.maxLength()) {}

BookNodeType::BookNodeType()
    : BookNodeType(QString(), QMetaType::UnknownType) {}

const QString BookNodeType::name() const { return p_name; }

const QMetaType::Type BookNodeType::type() const { return p_type; }

int BookNodeType::minLength() const { return p_minLength; }

int BookNodeType::maxLength() const { return p_maxLength; }
