// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKBINDING_UTILS_H
#define BOOKBINDING_UTILS_H

#include <QComboBox>
#include <QEvent>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

/**
 * @brief Auswahl der vorgegebenen Buchbindingen
 * @class BookBindingBox
 * Vom Dienstleister Untertützte Typen sind:
 * @list Einbandarten Hardcover:
 *  @li Hardcover
 *  @li Leder
 *  @li Halbleder
 *  @li Leinen
 *  @li Halbleinen
 *  @li Pappband
 * @list Einbandarten Softcover:
 *  @li Softcover
 *  @li Kartoniert
 *  @li Broschiert
 *  @li Taschenbuch
 *  @li Heft
 */
class BookBindingBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  explicit BookBindingBox(QWidget *parent = nullptr);
};

/**
 * @brief Auswahlfeld Bucheditor Buchbinding
 */
class BookBindingField : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  BookBindingBox *m_box;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit BookBindingField(QWidget *parent = nullptr);
  void setInfo(const QString &);
  const QString info();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // BOOKBINDING_UTILS_H
