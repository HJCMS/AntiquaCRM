// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEDIALOG_IMAGING_H
#define IMAGEDIALOG_IMAGING_H

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QObject>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

class ApplSettings;
class ImageView;

class FileBrowser final : public QFileDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  /**
   * @brief Automatsuches schließen unterdrücken!
   * Der Dialog soll sich nicht schließen wenn kein explizites Beenden
   * aufgerufen wird!
   */
  void accept() override;
  void reject() override;

Q_SIGNALS:
  void s_close();
  void s_save();

public:
  explicit FileBrowser(QWidget *parent);
};

class ImageDialog final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const int p_articleId;
  ApplSettings *config;
  FileBrowser *browser;
  QDir imagesArchiv;
  QAction *ac_rotate;
  QAction *ac_scale;
  QAction *ac_save;
  QAction *ac_close;
  ImageView *m_view;
  QToolBar *m_toolBar;
  QStatusBar *m_statusBar;
  QPushButton *btn_save;

  /**
   * @brief Suche Bildnummer im Archiv
   */
  bool findSourceImage();

private Q_SLOTS:
  void save();
  void imagePreview(const QString &);

public Q_SLOTS:
  void notifyStatus(const QString &);

public:
  explicit ImageDialog(int articleId, QWidget *parent = nullptr);
  int exec();
};

#endif // IMAGEDIALOG_IMAGING_H
