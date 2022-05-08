// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNRESULTS_INVENTORY_H
#define ISBNRESULTS_INVENTORY_H

#include <QContextMenuEvent>
#include <QListWidget>
#include <QObject>

class IsbnRequest;

class ISBNResults final : public QListWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Aktuelle ISBN
   */
  QVariant p_isbn;

  /**
   * @brief ISBN Abfrage Klasse
   */
  IsbnRequest *m_isbnRequest;

private Q_SLOTS:
  /**
     @brief Konvertiert das ISBN ergebnis.
     Wenn ein Eintrag gefunden wurde werden hier eingefügt.
     @see fetchIsbnData()
   */
  void setIsbnInfo(bool);

  /**
   * @brief Ergebnis Text von ausgewählter Spalte kopieren!
   */
  void copySelectedItem();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  /**
   * @brief Wenn eine Ergebnisliste erstellt wurde.
   */
  void requestFinished();

public Q_SLOTS:
  /**
     @brief Sendet eine ISBN Anfragen
   */
  void fetchIsbnData(const QVariant &isbn);

public:
  explicit ISBNResults(QWidget *parent = nullptr);
  ~ISBNResults();
};

#endif // ISBNRESULTS_INVENTORY_H
