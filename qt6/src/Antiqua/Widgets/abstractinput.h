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
#include <QIcon>
#include <QMetaType>
#include <QObject>
#include <QResource>
#include <QSqlField>
#include <QVariant>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup IconTheme
 * @brief Icon from Theme or Application Resource file ...
 * Load qrc://icons/<image> from icon QResource.
 * @note The QResource must initialed in application first!
 * @param name  - Iconname
 * @return QIcon
 */
ANTIQUACRM_LIBRARY inline const QIcon AntiquaApplIcon(const QString &name) {
  QIcon _back(":/icons/" + name + ".png");
  QIcon _icon = QIcon::fromTheme(name, _back);
#ifdef ANTIQUA_DEVELOPEMENT
  if (_icon.isNull()) {
    qWarning("Missing required ThemeIcon(%s)!", qPrintable(name));
  }
#endif
  return _icon;
}

class ALabel;

/**
 * @class AbstractInput
 * @brief In this group all Antiqua CRM Widgets with Input masks defined.
 * This classes only used for Subclassing and not direct in the Interface.
 *
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY AbstractInput : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY
                 sendRequiredChanged)

private:
  bool required;

protected:
  /**
   * @brief AntiquaCRM Configuration caller
   * This is the main Configuration reader and will initialized in constructor.
   */
  AntiquaCRM::ASettings *config;

  /**
   * @brief By default all mouse wheel Events disabled.
   * This Parameter is reserved for Subclassing Input Widgets.
   * It is intended to prevent the client from unintentionally changing entries.
   * Experience has shown that older people in particular have problems with
   * large input masks.
   *
   * - It is loaded when this Class will initialised.
   * - Parameter: „window_behavior/mouse_wheel_support.
   * - After change it, a application restart is required.
   */
  bool mouseWheelEvents = false;

  /**
   * @brief Show Tooltip icon buttons to open WhatsThis PopUp’s.
   * By default this Variable is true, you can change it in the Interface
   * Configuration to hide it on all Editor masks.
   *
   * - It is loaded when this Class will initialised.
   * - Parameter: „window_behavior/display_tooltip_buttons“.
   * - After change it, a application restart is required.
   */
  bool displayToolTips = true;

  /**
   * @brief central Input Widgetlayout
   * This is the main layout for all Input Widgets and it is empty!
   * Read Qt's Api-Documentation to change the direction behavior.
   * By default it is a „LeftToRight“ layout.
   */
  QBoxLayout *layout;

  /**
   * @brief e.g. initial database, container or model data
   * This virtual function is reserved for loading custom data after construct
   * the subclass.
   * By default it is protected but when needed, you can move it to public.
   *
   * @warning It is a bad idea to initial SQL/Network-Queries inside of a
   * constructor. This can freeze the Application when you get Network problems
   * or no access to the database exists.
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
   * @brief emitted when Object Required changed.
   */
  void sendRequiredChanged();

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
  void sendInputChanged();

public Q_SLOTS:
  /**
   * @brief Is this input Widget required by sql database?
   * You can set this Value manually in Class constructor or change it in Method
   * „setRestrictions“.
   */
  void setRequired(bool);

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
  virtual ~AbstractInput();

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
   * @brief Prepend a title label to the layout
   */
  AntiquaCRM::ALabel *addTitleLabel(const QString &);

  /**
   * @brief get about status, if this input is required or not.
   * This helper function will called when user clicked save or leave editor.
   * See also „setRequired“ and „setRestrictions“ for more Info to set this.
   */
  bool isRequired();

  /**
   * @brief Insert a buddy label before the Input widget
   * When set, prepend a Textlabel to the Input edit field.
   * @note Use addTitleLabel() to set and get the Buddy label.
   */
  virtual void setBuddyLabel(const QString &) = 0;

  /**
   * @brief Append a WhatsThis Tool Button to the input widget.
   * On not X11 Platforms the What’s this Button is missing on the taskbar.
   * We need it for additional User Information.
   */
  virtual void setWhatsThisText(const QString &);

  /**
   * @brief Finally appending a stretch to the Widget::layout
   * @warning Before call this function you need to sure that your Layout
   * already been done.
   */
  virtual void appendStretch(int expanding = 0);

  /**
   * @brief Caller for validate current Input.
   * Reserved to check current Input with given restrictions.
   * This helper function will called when user clicked save data.
   * If data not valid „setFocus„ and „popUpHints“ will called from Interface.
   */
  virtual bool isValid() = 0;

  /**
   * @brief Default Metatype for this Input.
   */
  virtual const QMetaType getType() const = 0;

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
