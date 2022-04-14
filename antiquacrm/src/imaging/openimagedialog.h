// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef OPENIMAGEDIALOG_H
#define OPENIMAGEDIALOG_H

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QObject>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSplitter>

class ToolBar;
class ImageView;
class ListView;

/**
   Eigene Implementierung von FileDialog.
   @note Es werden nur JPG-Daten angenommen.
*/
class OpenImageDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
    Einfacher Toolbar für die Pfadauswahl.
  */
  ToolBar *m_toolBar;

  /**
    Trennt Vorschau und Liste
  */
  QSplitter *m_splitter;

  /**
     Bilder Vorschau
  */
  ImageView *m_thumbsView;

  /**
   Verzeichnisliste
  */
  ListView *m_listView;

  /**
   Pfad zur Bilddatei
  */
  QLineEdit *m_lineEdit;

private Q_SLOTS:
  /**
     Wenn Bild Leserechte Besitzt
     dann in @ref m_listView einfügen
     und mit Signal @ref imageChanged
     Den Speichern Knopf aktivieren.
  */
  void isValid(const QString &fp);

Q_SIGNALS:
  /**
    Wird ausgelöst wenn die Textzeile
    sich ändert!
  */
  void imageChanged(bool);

public:
  OpenImageDialog(QWidget *parent = nullptr);

  /**
     Setze den rootPath auf ...
  */
  void setStart(const QString &path);

  /**
     Gibt die ausgewählte Bilddatei aus.
     @return QImage
  */
  const QImage image();

  /**
     Pfad der ausgewählten Bilddatei
     @return QFileInfo
  */
  const QFileInfo file();

  virtual ~OpenImageDialog();
};
#endif // OPENIMAGEDIALOG_H
