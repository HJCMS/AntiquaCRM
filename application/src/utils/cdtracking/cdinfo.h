// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_CDINFO_H
#define CDTRACKING_CDINFO_H

#include <QMetaType>
#include <QObject>
#include <QString>
#include <QThread>
#include <QUrl>

#include <cdio/cdio.h>
#include <cdio/logging.h>

struct HardwareInfo {
  QByteArray model;
  QByteArray vendor;
  QByteArray revision;
};

class CDInfo final : public QThread {
  Q_OBJECT

private:
  QUrl p_device;

  struct options {
    int no_tracks;
    int no_ioctl;
    int no_analysis;
    char *access_mode; /* Access method driver should use for control */
    int no_cddb;       /* If set the below are meaningless. */
    int no_vcd;
    int show_dvd;
    int no_device;
    int no_disc_mode;
    uint32_t debug_level;
    int version_only;
    int silent;
    int no_header;
    int no_joliet;
    int no_xa;
    int no_rock_ridge;
    int print_iso9660;
    int list_drives;
  } opt;

  bool open(const QByteArray &);

  static void messanger(cdio_log_level_t, const char *);

//private Q_SLOTS:
//  Q_INVOKABLE void prepareMessage(const QString &);

Q_SIGNALS:
  void sendStatusMessage(const QString &);
  void sendMediaInfo(const HardwareInfo &);

public:
  explicit CDInfo(const QUrl &device, QObject *parent = nullptr);
  void run() override;
};

Q_DECLARE_METATYPE(HardwareInfo)

#endif // CDTRACKING_CDINFO_H
