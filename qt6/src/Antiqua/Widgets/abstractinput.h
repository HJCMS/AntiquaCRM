// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_ABSTRACTINPUT_H
#define ANTIQUACRM_INPUT_ABSTRACTINPUT_H

#include <AntiquaCRM>
#include <QBoxLayout>
#include <QFocusEvent>
#include <QMetaType>
#include <QObject>
#include <QSqlField>
#include <QVariant>
#include <QWidget>

namespace AntiquaCRM {

class AbstractInputPrivate;

/**
 * @ingroup AntiquaInput
 * @brief In this group all Antiqua CRM Widgets with Input masks defined.
 * This classes only used for Subclassing and not direct in the Interface.
 * You can include it with: #include &gt;AntiquaInput&lt;
 *
 * @class AbstractInput
 */
class ANTIQUACRM_LIBRARY AbstractInput : public QWidget {
  Q_OBJECT
  Q_DECLARE_PRIVATE(AbstractInput);
  Q_DISABLE_COPY(AbstractInput)

protected:
  /**
   * @brief AntiquaCRM Configuration caller
   * This is the main Configuration reader and will initialized in constructor.
   */
  AntiquaCRM::ASettings *config;

  /**
   * @brief By default all mouse wheel Events disabled.
   * This Parameter is reserved for Subclassing Input Widgets. The value will
   * read at Application start from Configuration section „window_behavior“. It
   * is intended to prevent the client from unintentionally changing entries.
   * Experience has shown that older people in particular have problems with
   * large input masks. You can query this parameter from the method
   * windowBehavior(). e.g. call windowBehavior("mouse_wheel_support") to
   * get the changed boolean value.
   */
  bool mouseWheelEvents = false;

  /**
   * @brief central Input Widgetlayout
   * This is the main layout for all Input Widgets and it is empty!
   * Read Qt's Api-Documentation to change the direction behavior.
   * By default it is a „LeftToRight“ layout.
   */
  QBoxLayout *layout;

  /**
   * @brief Data type for current input edit.
   * This Metatype will set by QSqlField in „setRestrictions“ and can used by
   * „setValue“,„isValid“ and „getValue“ to convert QVariant in the right
   * Format. By default, the current Metatype is a QString.
   * @note It is a good choice, to set this in the Subclass constructor.
   */
  QMetaType dataType = QMetaType(QMetaType::QString);

  /**
   * @brief Widget behavior settings
   * @param key - call with key in „window_behavior“ settings group.
   * @param standard - deault value for caller
   */
  bool windowBehavior(const QString &key, bool standard = false);

  /**
   * @brief Is this input Widget required by sql database?
   * You can set this Value manually in Class constructor or change it in Method
   * „setRestrictions“.
   */
  void setRequired(bool);

  /**
   * @brief e.g. init database or model data
   * This virtual function is reserved for loading custom data after construct
   * the subclass.
   * By default it is protected but when needed, you can move it to public.
   *
   * @warning Do not use it Constructors with SQL-Queries! It is a bad idea to
   * init SQL queries inside of constructors. This can freeze the Application
   * when you get Network problems or no access to the database exists.
   */
  virtual void initData() = 0;

protected Q_SLOTS:
  /**
   * @brief focusOutEvent
   * The „focusOutEvent“ will emitted the „sendLeaveInput“ Signal for
   * subclassing Input wigets, to create modify changes checks.
   */
  virtual void focusOutEvent(QFocusEvent *) override;

Q_SIGNALS:
  /**
   * @brief This Signal will emitted, if focus get out from this widget.
   */
  void sendLeaveInput();

  /**
   * @brief This Signal will emitted, if data has been modified!
   * It is a helper signal to query QWidget::isWindowModified()
   * This is a part to find unsaved changes from inputs.
   * @note This procedure must manually implemented in subclasses.
   */
  void inputChanged();

public Q_SLOTS:
  /**
   * @brief set value
   * For a uniform parameter transfer. We read all data from Sql queries which
   * give a QVariant. The conversion of the data takes place in the input class.
   * See also „setRestrictions“ Method and QSqlField.
   */
  virtual void setValue(const QVariant &) = 0;

  /**
   * @brief focus input
   * The Input widget is inside of a Widget::layout, we need this slot to make
   * the right focus to the input widget.
   */
  virtual void setFocus() = 0;

  /**
   * @brief reset input
   */
  virtual void reset() = 0;

public:
  /**
   * @brief Abstract class for edit widgets
   * @param parent - Important, we need a valid handle for AntiquaCRM::ASettings
   */
  explicit AbstractInput(QWidget *parent = nullptr);
  ~AbstractInput();

  /**
   * @brief find data restriction with QSqlField
   * Qt's QSqlField class gives us all Information to prepare input Data in the
   * right format. This method is reserved to do this.
   */
  virtual void setRestrictions(const QSqlField &) = 0;

  /**
   * @brief Helper function to set Tooltips inside input layout.
   */
  virtual void setInputToolTip(const QString &) = 0;

  /**
   * @brief Insert a buddy label before the Input widget
   */
  virtual void setBuddyLabel(const QString &) = 0;

  /**
   * @brief Append a WhatsThis Tool Button to the input widget.
   */
  virtual void setWhatsThisButton(const QString &);

  /**
   * @brief Add layout stretch to tthis Widget::layout
   * @warning Before call this function you need to sure that your Layout
   * already been done.
   */
  virtual void setStretch();

  /**
   * @brief get about status, if this input is required or not.
   * This helper function will called when user clicked save or leave editor.
   * See also „setRequired“ and „setRestrictions“ for more Info to set this.
   */
  bool isRequired();

  /**
   * @brief Caller for validate current Input.
   * Reserved to check current Input with given restrictions.
   * This helper function will called when user clicked save data.
   * If data not valid „setFocus„ and „popUpHints“ will called from Interface.
   */
  virtual bool isValid() = 0;

  /**
   * @brief value from input widget ...
   * Get current Value as QVariant. The Conversion will done in this Section.
   * You can use the „setRestrictions“ to set class environments.
   */
  virtual const QVariant getValue() = 0;

  /**
   * @brief Visual hints for popup messages.
   * this hints function is used to inform clients when data invalid or empty if
   * required is set. It is normally used to make better info popup messages.
   * See also „isValid“ and „isRequired“ about the interface procedure.
   */
  virtual const QString popUpHints() = 0;

  /**
   * @brief Visual hints for status bar messages
   * This is a short form for visual hints with status bar messages.
   */
  virtual const QString statusHints() = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_ABSTRACTINPUT_H
