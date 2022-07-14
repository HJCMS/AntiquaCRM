// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYSUBJECT_UTILS_H
#define CATEGORYSUBJECT_UTILS_H

#include <QComboBox>
#include <QJsonObject>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QToolButton>
#include <QWheelEvent>
#include <QWidget>

#include <SqlCore>
#include <UtilsMain>

class CategorySubjectBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  CategorySubjectBox(QWidget *parent = nullptr);
};

class CategorySubject : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  CategorySubjectBox *m_boxSub;
  CategorySubjectBox *m_boxMain;
  QPushButton *m_syncBtn;
  QLineEdit *m_search;
  QToolButton *m_helpBtn;

  /**
   * @brief Hauptkategorien
   * @note Wird nur beim start geladen!
   */
  void setMainCategories();

  /**
   * @brief Autovervollständigung
   */
  void setCompleter(const QStringList &list);

  /**
   * @brief Suche Untergruppe mit "ce_id"
   */
  void setSubCategories(int ceId = 0);

  /**
   * @brief MessageBox mit SQL Error
   */
  void sqlErrorPopUp(const QString &error);

private Q_SLOTS:
  void mainCategoryChanged(int index);
  void searchChanged(const QString &);
  void syncronizeClicked();
  void openHelperDialog();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &val);
  void findIndex(const QString &match);

public:
  explicit CategorySubject(QWidget *parent = nullptr);
  void loadDataset();

  /**
   * @brief Alle aktuell ausgewählten Kategorien
   * @code
   *  QJsonObject {
   *    "main" = QJsonValue(QString),
   *    "sub" = QJsonValue(QString),
   *    "id" = QJsonValue(qint64)
   *  }
   * @endcode
   * @return
   */
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // CATEGORYSUBJECT_UTILS_H
