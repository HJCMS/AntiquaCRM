// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_BOOKS_SEARCHBAR_H
#define INVENTORY_BOOKS_SEARCHBAR_H

#include <QAction>
#include <QComboBox>
#include <QFlags>
#include <QJsonObject>
#include <QLineEdit>
#include <QObject>
#include <QRegExpValidator>
#include <QRegularExpression>
#include <QToolBar>
#include <QWidget>

/**
 * @brief Suchfilter Auswahl
 * @ingroup Inventory Books
 */
class SearchFilterBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  enum Filter {
    BooksAuthor = 0x000000,  /**< @brief Büchersuche */
    BooksKeyword = 0x000001, /**< @brief Bucher + Schlüsselwortsuche */
    Authors = 0x000010,      /**< @brief Autorensuche */
    ISBN = 0x000100,         /**< @brief ISBN Suche */
    ArticleId = 0x001000,    /**< @brief Artikel Nummernsuche */
    Storage = 0x010000,      /**< @brief Lager Suche */
    Publisher = 0x100000     /**< @brief Verlag/Hersteller Suche */
  };
  Q_DECLARE_FLAGS(Filters, Filter)

  explicit SearchFilterBox(QWidget *parent = nullptr);

  /**
   * @brief Nehme aktuellen Suchfilter
   * @note Wenn -1 dann wird currentIndex() verwendet!
   * @code
   *  QJsonObject(
   *    "search" => QJSonValue().toString()
   *    "fields" => QJSonValue().toString(),
   *  );
   * @endcode
   */
  const QJsonObject getFilter(int index = -1);
};
Q_DECLARE_METATYPE(SearchFilterBox::Filter)

/**
 * @brief Sucheingabe Felder
 * @ingroup Inventory Books
 */
class SearchLineEdit final : public QLineEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Zeichenketten Validierung
   */
  const QRegExp strPattern = QRegExp("^\\S{2}.+");
  QRegExpValidator *m_text_validator;

  /**
   * @brief Numeric Validation
   */
  const QRegExp numPattern = QRegExp("^[0-9]+$");
  QRegExpValidator *m_num_validator;

  /**
   * @brief Article Validation
   */
  const QRegExp articlePattern = QRegExp("^([0-9]{1,9}[\\,]?)+$");
  QRegExpValidator *m_article_validator;

public:
  enum Validator { Strings = 0, Numeric = 1, Article = 2 };
  explicit SearchLineEdit(QWidget *parent = nullptr);
  void setValidation(SearchLineEdit::Validator);
};
Q_DECLARE_METATYPE(SearchLineEdit::Validator)

/**
 * @brief Bücher Suchbalken
 * @ingroup Inventory Books
 */
class BookSearchBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int minLength = 2;
  QString p_currentLeft;
  QString p_currentRight;
  const QRegExp stripJokers = QRegExp("[%*]+");
  const QRegExp stripPattern = QRegExp("[\\'\\\"]+");
  const QRegExp trimPattern = QRegExp("[\\s\\t\\n\\r]+");
  const QRegExp articlePattern = QRegExp("[^0-9\\,]+");
  const QRegExp isbnPattern = QRegExp("[^0-9]+");

  /**
   * @brief Suchfilter
   */
  SearchFilterBox *m_filterSection;

  /**
   * @brief Sucheingabe
   */
  SearchLineEdit *m_searchLeft;
  SearchLineEdit *m_searchRight;

  /**
   * @brief Start Manuelle Suche
   */
  QAction *ac_search;

  /**
   * @brief Positions tausch der Suchworte
   * Wenn die Suche zwei Zeichenketten besitzt dann eine klausel wie folgt
   * erstellen.
   * @code
   *  // @b A/B ist das Ergebnis von search.split(" ");
   *  (fieldname ILIKE 'A%B%' OR fieldname ILIKE 'B%A%')
   * @endcode
   */
  const QString prepareFieldSet(const QString &fieldname,
                                const QString &search) const;

  const QString getTitleSearch(const QStringList &fields);

private Q_SLOTS:
  /**
   * @brief Der Eingabe Filter wurde geändert!
   */
  void filterChanged(int);

  /**
   * @brief Die Suchabfrage vorbereiten
   */
  void prepareSearchQuery();

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
  void setSearchFocus();
  void setFilterFocus();
  void clearAndFocus();

public:
  explicit BookSearchBar(QWidget *parent = nullptr);

  /**
   * @brief Zwichenketten länge zurück geben!
   */
  int searchLength();

  /**
   * @brief Rückgabewert von Sucheingabe
   */
  const QString getSearchStatement();
};

#endif // INVENTORY_BOOKS_SEARCHBAR_H
