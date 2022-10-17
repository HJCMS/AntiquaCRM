// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_INPUT_EDIT_H
#define ANTIQUA_INPUT_EDIT_H

#include <QFocusEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QMetaType>
#include <QObject>
#include <QSqlField>
#include <QVariant>
#include <QWidget>

class AntiquaILabel;

/**
 * @brief Abstract Input Edit class
 * @section widgets
 */
class InputEdit : public QFrame {
  Q_OBJECT
  Q_PROPERTY(bool required /* Equivalent zu QSqlRecord:required */
                 READ isRequired WRITE setRequired NOTIFY sendRequireChanged)
  Q_PROPERTY(bool modified /* Ableitung für windowModified */
                 READ isModified WRITE setModified NOTIFY sendModifiedChanged)

private:
  bool required = false;
  bool modified = false;

protected:
  AntiquaILabel *m_label;
  QHBoxLayout *m_layout;
  virtual void loadDataset() = 0;

protected Q_SLOTS:
  virtual void skipReturnPressed();
  virtual void focusOutEvent(QFocusEvent *);

Q_SIGNALS:
  void sendHasModified(bool);
  void sendRequireChanged();
  void sendModifiedChanged();
  void sendLeaveEditor();

public Q_SLOTS:
  Q_INVOKABLE void setRequired(bool b);
  Q_INVOKABLE void setModified(bool b);
  Q_INVOKABLE virtual void reset() = 0;
  virtual void setValue(const QVariant &) = 0;
  virtual void setFocus() = 0;

public:
  explicit InputEdit(QWidget *parent = nullptr);
  Q_INVOKABLE bool isRequired();
  Q_INVOKABLE bool isModified();
  virtual void setProperties(const QSqlField &field) = 0;
  virtual const QVariant value() = 0;
  virtual bool isValid() = 0;
  virtual void setInfo(const QString &) = 0;
  virtual const QString info() = 0;
  virtual const QString notes() = 0;
};

typedef QList<InputEdit *> InputEditList;
Q_DECLARE_METATYPE(InputEditList)

#endif // ANTIQUA_INPUT_EDIT_H
