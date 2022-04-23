// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LISTVIEW_IMAGING_H
#define LISTVIEW_IMAGING_H

#include <QtCore/QFileInfo>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtWidgets/QListView>

class FileViewModel;

/**
 Verzeichnis Listenansicht für die
 Bilderauswahl.
*/
class ListView : public QListView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
     Ableichtung von QFileSystemModel
   */
  FileViewModel *m_model;

  /**
     Prüfe auf image/jpeg format.
     Alles andere wird verworfen.
   */
  bool checkJpeg(const QFileInfo &);

private Q_SLOTS:
  /**
    Wenn ausgeführt wir die Vorschau aktiviert.
   */
  void checkSingleClicked(const QModelIndex &);

  /**
     Wenn ausgeführt wir der Bildpfad in das
     LineEdit eingfügt und der Speichern Knopf
     aktiviert.
   */
  void checkDoubleClicked(const QModelIndex &);

Q_SIGNALS:
  /**
     Wird bei einmen einfach Klick auf das Bild
     ausgeführt.
     Wird von der Bildvorschau benötigt.
   */
  void s_previewImage(const QString &);

  /**
     Wird ausgeführt wenn ein Doppelklick auf die
     Bilddatei getätigt wurde und in das LineEdit
     eingesetzt.
   */
  void s_selectionChanged(const QString &);

public Q_SLOTS:
  /**
     Sende Verzeichnispfad
  */
  void goTo(const QString &);

  /**
    Sende QDir::cdUp();
  */
  void goUpward();

public:
  explicit ListView(QWidget *parent = nullptr);
};

#endif // LISTVIEW_IMAGING_H
