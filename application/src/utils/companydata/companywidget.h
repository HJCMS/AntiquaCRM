// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPANY_COMPANYWIDGET_H
#define COMPANY_COMPANYWIDGET_H

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @brief Textfield input widget
 * @class TextInput
 * @ingroup _companydata
 */
class TextInput final : public QTextEdit {
  Q_OBJECT

public Q_SLOTS:
  void setValue(const QString &);

public:
  explicit TextInput(QWidget *parent = nullptr);
  const QString value();
};

/**
 * @brief Textline input widget
 * @class LineInput
 * @ingroup _companydata
 */
class LineInput final : public QLineEdit {
  Q_OBJECT

public Q_SLOTS:
  void setValue(const QString &);

public:
  explicit LineInput(QWidget *parent = nullptr);
  const QString value();
};

/**
 * @brief Grouped Input Settings Widget
 * @class CompanyWidget
 * @ingroup _companydata
 */
class CompanyWidget : public QWidget {
  Q_OBJECT

private:
  int layoutRow = 0;
  QGridLayout *m_layout;
  QLabel *infoCell(const QString &title);

Q_SIGNALS:
  void sendSaved(bool);

public Q_SLOTS:
  void saveConfig();

public:
  explicit CompanyWidget(QWidget *parent = nullptr);
  bool loadConfig();
};

#endif // COMPANY_COMPANYWIDGET_H
