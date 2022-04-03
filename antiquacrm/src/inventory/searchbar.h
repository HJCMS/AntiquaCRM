// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtGui/QDoubleValidator>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

struct SearchFilter {
  int index;
  QString title;
  QString filter;
};

struct SearchStatement {
  QString SearchField;
  QString SearchString;
};

/**
 @class SearchBar
 @brief Primäre Suchbalkenklasse.
*/
class SearchBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QComboBox *m_filterSection;
  QLineEdit *m_searchLineEdit;
  QRegExpValidator *m_textValidator;
  QDoubleValidator *m_numValidator;

private Q_SLOTS:
  void beforeTextChanged();
  void beforeTextChanged(const QString &);
  void updateEditPlaceHolder();
  void searchFilterChanged(int);

Q_SIGNALS:
  /**
    Die Suche wird erst ausgelöst wenn in die
    Eingabezeile mehr als 1 Zeichen enthält.
    @list Auslöser sind:
     @li QLineEdit::textChanged()
   */
  void searchTextChanged(const QString &);

  /**
    Starte suche Manuel
    @list Auslöser sind:
     @li QPuchButton::released()
     @li QLineEdit::enterPressed()
   */
  void searchClicked();

  /**
     @short Wird ausgelöst wenn der Filter geändert wurde.
   */
  void currentFilterChanged(int);

public:
  enum Validation { Pattern, Number };
  Q_ENUM(Validation)

public Q_SLOTS:
  /**
     @short Setzt den Suchfilter
   */
  void setSearchFilter(int index);

public:
  explicit SearchBar(QWidget *main);
  /**
     @short Ändert bei der Sucheingabe die Validierung.
     @note Ein richtiges abfangen der Fehlereingaben
        ist je nach vererbung Erforderlich und muss
        in diesen Unterklassen selbst erfolgen!
     @li @c Pattern - Verhindert nur, das Leerzeichen am Anfang stehen!
     @li @c Number  - Setzt den Filter auf einen 13 Stelligen Zahlenbereich!
   */
  void setValidation(SearchBar::Validation);

  /**
    @short Erstelle die Suchfilter für die Auswahlbox!
    @code
        QHash<QString, QString> &filter;
        filter.insert("Display Text","User Value");
    @endcode
   */
  void addSearchFilters(QList<SearchFilter> &);

  /**
     @brief Gibt den Suchfilter mit dem Index zurück.
     @return Suchfilter \b Wert, nicht den Anzeigetext!
   */
  const QString getSearchFilter(int index);

  /**
     @brief currentFilterIndex
     @return QComboBox::currentIndex();
   */
  int currentFilterIndex();

  /**
     @brief currentSearchText
     @return QLineEdit::text();
   */
  const QString currentSearchText();
};

Q_DECLARE_METATYPE(SearchStatement)
Q_DECLARE_METATYPE(SearchFilter)

#endif // SEARCHBAR_H
