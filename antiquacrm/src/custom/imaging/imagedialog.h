// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEDIALOG_IMAGING_H
#define IMAGEDIALOG_IMAGING_H

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QObject>
#include <QToolBar>
#include <QWidget>

class ApplSettings;
class ImageView;

class ImageDialog : public QFileDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const int p_articleId;
  ApplSettings *config;
  QDir imagesArchiv;
  QAction *ac_rotate;
  QAction *ac_scale;
  ImageView *m_view;
  QToolBar *m_toolBar;

  /**
   * @brief Suche Bildnummer im Archiv
   */
  bool findSourceImage();

  /**
   * @brief Automatsuches schließen unterdrücken!
   * Der Dialog soll sich nicht schließen wenn kein explizites Beenden
   * aufgerufen wird!
   */
  void accept();

private Q_SLOTS:
  void save();
  void imagePreview(const QString &);

public:
  explicit ImageDialog(int articleId, QWidget *parent = nullptr);
  int exec();
};

#endif // IMAGEDIALOG_IMAGING_H
