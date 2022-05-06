// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderarticle.h"

#include <QDebug>

OrderArticle::OrderArticle() {
  p_payment = -1;
  p_article = -1;
  p_orderid = -1;
  p_costumer = -1;
  p_count = -1;
  p_price = 0;
  p_sell_price = 0;
  p_title = QString();
  p_summary = QString();
}

void OrderArticle::setPayment(int id) { p_payment = id; }

int OrderArticle::payment() { return p_payment; }

void OrderArticle::setArticle(int id) { p_article = id; }

int OrderArticle::article() { return p_article; }

void OrderArticle::setOrder(int id) { p_orderid = id; }

int OrderArticle::order() { return p_orderid; }

void OrderArticle::setCostumer(int id) { p_costumer = id; }

int OrderArticle::costumer() { return p_costumer; }

void OrderArticle::setCount(int c) { p_count = c; }

int OrderArticle::count() { return p_count; }

void OrderArticle::setPrice(double d) { p_price = d; }

double OrderArticle::price() { return p_price; }

void OrderArticle::setSellPrice(double d) { p_sell_price = d; }

double OrderArticle::sellPrice() { return p_sell_price; }

void OrderArticle::setTitle(const QString &str) { p_title = str; }

const QString OrderArticle::title() { return p_title; }

void OrderArticle::setSummary(const QString &str) { p_summary = str; }

const QString OrderArticle::summary() { return p_summary; }
