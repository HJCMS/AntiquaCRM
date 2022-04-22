// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_H
#define INVENTORY_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

/**
 @brief Elternklasse für Inventory Tabs
 @see Workspace
*/
class Inventory : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool closable READ isClosable WRITE setClosable NOTIFY closeableChanged)

private:
  /**
    @brief Kann das Fenster geschlossen werden?
    Wird von den Editor Klassen ausgelöst und muss
    in den vererbten Klassen hierhin weiter geleitet werden.
  */
  bool closable = true;

protected Q_SLOTS:
  /**
     @brief Änderungen wie isModified abfangen.
     Wird von {*}Editorklassen ausgelöst.
  */
  void setClosable(bool b);

Q_SIGNALS:
  /**
    @brief Wegen QML hier eingefügt.
  */
  void closeableChanged(bool);

public:
  explicit Inventory(QWidget *parent = nullptr);
  /**
     @brief Wird von Workspace::tabRemoved abgefragt!
     @return bool - (child*)->isModified()
  */
  Q_INVOKABLE bool isClosable();
};

#endif // INVENTORY_H
