// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_UTILS_H
#define INVENTORY_UTILS_H

#include <QObject>
#include <QVariant>
#include <QWidget>

/**
 * @class Inventory inventory.h Utils
 * @abstract
 * @brief Hauptklasse für alle Inventory Tabulaturen
 */
class Inventory : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(
      bool closable READ isClosable WRITE setClosable NOTIFY closeableChanged)

protected:
  /**
   * @brief Kann das Fenster geschlossen werden?
   * Wird von den Editor Klassen ausgelöst und muss
   * in den vererbten Klassen hierhin weiter geleitet werden.
   * @see Q_PROPERTY
   */
  bool closable = true;

  /**
   * Standard Zeichenanzahl bis die Suchabfrage startet!
   * Diese Variable wird auch in den Einstellungen fest gelegt.
   * Kann auch in den Unterklassen überschrieben werden.
   * Wird z.B. in searchConvert verwendet, damit es nicht zu
   * viele Abfragen auf einemal gibt.
   * @issue
   * @see QSettings::value("search/startlength").toInt();
   */
  int minLength = 5;

  /**
   * @brief Öffnet das Editor Fenster
   * @ref editormain.h
   * Wegen Tastenbindungen ist das Editor Fenster
   * deaktiviert und muss hier wieder aktiviert werden.
   */
  virtual void openEditor(const QString &) = 0;

  /**
   * @brief Darf dieser Tab geschlossen werden?
   */
  void setClosable(bool b = false);

protected Q_SLOTS:
  /**
   * @brief Kopiert Text in das Clipboard
   */
  void copyIntoClipboard(const QVariant &);

  /**
   * @brief Wurde das Fenster geändert
   */
  void setIsModified(bool);

  /**
   * @brief Methode für Suche während eingabe.
   * @param Sucheingabe
   */
  virtual void searchConvert(const QString &search) = 0;

  /**
    @brief Methode für Suchknopf
   */
  virtual void searchConvert() = 0;

  /**
    @brief Öffne Tabellenansicht
    Wird mit SIGNAL s_leaveEditor aufgerufen.
    @note Standard ist Tabellenansicht "Page 1"
  */
  virtual void openTableView() = 0;

  void findModifySignals();

Q_SIGNALS:
  /**
   * @brief Änderungen aufzeichnen
   */
  void s_windowModified(bool);

  /**
    @brief Wegen QML hier eingefügt.
  */
  void closeableChanged(bool);

  /**
    @brief Sende eine Nachricht an das Elternfenster
    @param MessageBody
  */
  void s_postMessage(const QString &);

  /**
   * @brief Auftrage erstellen
   * @param customerId
   */
  void s_createOrder(int customerId);

  /**
   * @brief Einen Artikel zu einem Auftrag hinzufügen!
   */
  void s_addArticleOrder(int articleId);

  /**
   * @brief Signal Artikelanzahl Remote ändern!
   * Das Signal wird von EditorMain empfangen und weiter gereicht!
   * @param articleId  - Artikel Nummer
   * @param count      - Bestand
   */
  void s_articleCount(int articleId, int count);

public Q_SLOTS:
  /**
    @brief Öffnet das Nachrichtenfenster
    @param MessageBody
  */
  void displayMessageBox(const QString &);

  /**
   * @brief Wenn das Tab im Vordergund geht, dann eine Aktionen ausführen.
   * @warning Wird immer aufgerufen wenn ein Tab nach vorne geklickt wird!
   * Behandle diese Funktion mit Vorsicht, denn sie kann Eingaben verwerfen!
   */
  virtual void onEnterChanged() = 0;

public:
  explicit Inventory(QWidget *parent = nullptr);
  /**
     @brief Wird von Workspace::tabRemoved abgefragt!
     @return bool - (child*)->isModified()
  */
  Q_INVOKABLE bool isClosable();
};

#endif // INVENTORY_UTILS_H
