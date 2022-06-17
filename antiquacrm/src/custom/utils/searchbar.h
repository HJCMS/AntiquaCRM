// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SEARCHBAR_UTILS_H
#define SEARCHBAR_UTILS_H

#include <QComboBox>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QList>
#include <QObject>
#include <QJsonObject>
#include <QRegularExpression>
#include <QRegExpValidator>
#include <QToolBar>
#include <QWidget>

class SearchFilter;

/**
 * @brief Suchabfrage
 * Q_DECL_DEPRECATED
 */
struct Q_DECL_DEPRECATED SearchStatement {
  QString SearchField;  /**< Feldname */
  QString SearchString; /**< Suchtext */
};

/**
 * @class SearchBar
 * @brief Primäre Suchbalkenklasse.
 */
class SearchBar final : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Suchfilter
   */
  QComboBox *m_filterSection;

  /**
   * @brief Sucheingabe
   */
  QLineEdit *m_searchLineEdit;

  /**
   * @brief Zeichenketten Validierung
   */
  QRegExpValidator *m_textValidator;

  /**
   * @brief Nummern Validierung
   */
  QDoubleValidator *m_numValidator;

private Q_SLOTS:
  void beforeTextChanged();
  void beforeTextChanged(const QString &);
  void updateEditPlaceHolder(int i);
  void searchFilterChanged(int i);

Q_SIGNALS:
  /**
   * @brief Bei Eingabe suchen ...
   */
  void searchTextChanged(const QString &);

  /**
   * @brief Starte suche Manuel
   */
  void searchClicked();

  /**
   * @short Wird ausgelöst wenn der Filter geändert wurde.
   */
  void currentFilterChanged(int);

public Q_SLOTS:
  /**
   * @brief Focus auf die Sucheinegabe setzen
   */
  void setFilterFocus();

  /**
   * @brief leeren und Fokus auf Sucheingabe
   */
  void clearAndFocus();

  /**
   * @brief Setzt den Suchfilter
   */
  void setSearchFilter(int index);

public:
  enum Validation {
    Pattern = 0, /**< Texteingabe */
    Number = 1   /**< Nummernsuche */
  };
  Q_ENUM(Validation)

  /**
   * @brief SearchBar
   */
  explicit SearchBar(QWidget *parent = nullptr);

  /**
   * @short Ändert bei der Sucheingabe die Validierung.
   * @note Ein richtiges abfangen der Fehlereingaben ist je nach Vererbung
   * Erforderlich und muss in diesen Unterklassen selbst erfolgen!
   * @li @c Pattern - Verhindert nur, das Leerzeichen am Anfang stehen!
   * @li @c Number  - Setzt den Filter auf einen 13 Stelligen Zahlenbereich!
   */
  void setValidation(SearchBar::Validation);

  /**
   * @short Erstelle die Suchfilter für die Auswahlbox!
   * @code
   *  QHash<QString, QString> &filter;
   *  filter.insert("Display Text","User Value");
   *  @endcode
   */
  void addSearchFilters(const QList<SearchFilter> &);

  /**
   * @brief Gibt den Suchfilter mit dem Index zurück.
   */
  const QJsonObject getSearchFilter(int index);

  /**
   * @brief currentFilterIndex
   * @return QComboBox::currentIndex();
   */
  int currentFilterIndex();

  /**
   * @brief Gibt die aktuelle Sucheingabe zurück.
   * Es wird vor der Ausgabe mit Regexp entfernt:
   *  @li doppelte Leerzeichen
   *  @li Tabulatur und Zeilenumbrüche (Copy Paste Problem)
   *  @li Apostroph und Anführungszeichen (SQL-Statement)
   */
  void setSearchText(const QString &);

  /**
   * @brief Rückgabewert von Sucheingabe
   */
  const QString currentSearchText();
};

#endif // SEARCHBAR_UTILS_H
