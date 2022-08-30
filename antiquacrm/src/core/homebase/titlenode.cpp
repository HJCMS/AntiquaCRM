// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "titlenode.h"

TitleNode::TitleNode(const QDomElement &parent) : BookListingNode{parent} {}

const QString TitleNode::name() const { return "title"; }

QMetaType::Type TitleNode::type() const { return QMetaType::QString; }

int TitleNode::minLength() const { return 0; }

int TitleNode::maxLength() const { return 750; }
