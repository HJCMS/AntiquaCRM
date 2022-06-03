// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEDIALOG_IMAGING_H
#define IMAGEDIALOG_IMAGING_H

#include <QCloseEvent>
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
  ImageView *m_view;
  QToolBar *m_toolBar;

  /**
   * @brief Suche Bildnummer im Archiv
   */
  void findSourceImage();

  /**
   * @brief Lade Einstellungen beim öffnen.
   */
  void showEvent(QShowEvent *event) override;

  /**
   * @brief Automatsuches schließen unterdrücken!
   * Der Dialog soll sich nicht schließen wenn kein explizites Beenden
   * aufgerufen wird!
   */
  void accept() override;

private Q_SLOTS:
  void save();
  void imagePreview(const QString &);

public:
  explicit ImageDialog(int articleId, QWidget *parent = nullptr);
};

#endif // IMAGEDIALOG_IMAGING_H
