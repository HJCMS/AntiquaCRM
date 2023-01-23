// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdinfo.h"

#include <QDebug>
#include <QMetaObject>

// https://git.savannah.gnu.org/cgit/libcdio.git
#include <cdio/bytesex.h>
#include <cdio/cd_types.h>
#include <cdio/cdtext.h>
#include <cdio/ds.h>
#include <cdio/iso9660.h>
#include <cdio/mmc.h>
#include <cdio/util.h>

CDInfo::CDInfo(const QUrl &device, QObject *parent)
    : QThread{parent}, p_device{device} {
  setObjectName("cd_info_handle");
}

bool CDInfo::open(const QByteArray &device) {
  CdIo_t *p_cdio = nullptr;
  discmode_t discmode = CDIO_DISC_MODE_NO_INFO;
  /* Default option values. */
  opt.silent = false;
  opt.list_drives = false;
  opt.no_header = false;
  opt.no_joliet = 0;
  opt.no_rock_ridge = 0;
  opt.no_xa = 0;
  opt.no_device = 0;
  opt.no_disc_mode = 0;
  opt.debug_level = 0;
  opt.no_tracks = 0;
  opt.print_iso9660 = 0;
  opt.no_cddb = 0;
  opt.no_vcd = 1;
  opt.no_ioctl = 0;
  opt.no_analysis = 0;
  opt.access_mode = NULL;
  // Debugger
  cdio_log_set_handler(messanger);

  p_cdio = cdio_open_am(device.constData(), DRIVER_UNKNOWN, opt.access_mode);
  if (p_cdio != nullptr) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "CDIO:" << cdio_get_driver_name(p_cdio)
             << cdio_get_arg(p_cdio, "access-mode");
#endif

    if (0 == opt.no_device) {
      cdio_hwinfo_t hwinfo;
      if (cdio_get_hwinfo(p_cdio, &hwinfo)) {
        HardwareInfo hw_info_t;
        hw_info_t.vendor = QByteArray(hwinfo.psz_vendor).trimmed();
        hw_info_t.model = QByteArray(hwinfo.psz_model).trimmed();
        hw_info_t.revision = QByteArray(hwinfo.psz_revision).trimmed();
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << "Vendor:" << hw_info_t.vendor << Qt::endl
                 << "Model:" << hw_info_t.model << Qt::endl
                 << "Revision:" << hw_info_t.revision;
#endif
        emit sendMediaInfo(hw_info_t);
      }
    }

    discmode = cdio_get_discmode(p_cdio);
#ifdef ANTIQUA_DEVELOPEMENT
    if (0 == opt.no_disc_mode) {
      qDebug() << "Disc mode:" << discmode2str[discmode];
    }
#endif
    if (cdio_is_discmode_dvd(discmode)) {
      emit sendStatusMessage(tr("DVDs not supported!"));
      return false;
    }

    track_t _first_track = cdio_get_first_track_num(p_cdio);
    if (CDIO_INVALID_TRACK == _first_track) {
      qWarning("No Tracks found, aborted.");
      emit sendStatusMessage(tr("No Media found!"));
      return false;
    }

    track_t _num_tracks = cdio_get_num_tracks(p_cdio);
    qDebug() << Q_FUNC_INFO << "Tracks:" << _num_tracks;

    if(p_cdio != nullptr)
      std::free(p_cdio);

    return true;
  }
  return false;
}

void CDInfo::messanger(cdio_log_level_t level, const char *msg) {
  switch (level) {
  case CDIO_LOG_INFO:
    qInfo("CDInfo(%s)", msg);
    break;

  case CDIO_LOG_WARN:
    qWarning("CDInfo(%s)", msg);
    break;

  default:
    qDebug() << Q_FUNC_INFO << msg;
  };
}

void CDInfo::run() {
  if (p_device.isEmpty()) {
    qWarning("No interface specified!");
    quit();
  } else {
    QByteArray _dev = p_device.toEncoded(QUrl::PreferLocalFile);
    // NOTE - !!! Force init used MetaTypes !!!
    if (qMetaTypeId<HardwareInfo>() != 0 && open(_dev)) {
      qInfo("CDInfo search ...");
    } else {
      qWarning("CDInfo exited without result!");
      quit();
    }
  }
}
