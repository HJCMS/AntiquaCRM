// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_EDITOR_H
#define KEYWORD_EDITOR_H

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QJsonObject>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QHash>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSpinBox>
#include <QWidget>

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit group box
 */
class KeywordEditor : public QGroupBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QSpinBox *m_id;
  QLineEdit *m_keyword;
  QCheckBox *m_booksUsage;
  QPushButton *m_btnClear;
  QPushButton *m_btnCommit;
  const QRegExp p_validateRegExp = QRegExp("([\\w\\d]+)");
  const QRegExp p_stripRegExp = QRegExp("([^\\w\\d]+)");
  QRegExpValidator *m_validator;

private Q_SLOTS:
  void prepareCommit();

Q_SIGNALS:
  void updateKeyword(const QJsonObject &);
  void insertKeyword(const QJsonObject &);

public Q_SLOTS:
  void clear();
  void setKeyId(int);
  void setKeyword(const QString &);
  void setBookUsage(bool b = true);

public:
  explicit KeywordEditor(QWidget *parent = nullptr);
  int getKeyId();
  const QString getKeyword();
  bool getBookUsage();
};

#endif // KEYWORD_EDITOR_H
