// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ACOMPRESSEDHEADER_H
#define ANTIQUACRM_ACOMPRESSEDHEADER_H

#include <AGlobal>
#include <QByteArray>
#include <QString>

#include "zlib.h"

namespace AntiquaCRM {

class ACompressedHeader final {
public:
  enum operation {
    COMPRESS, /**< @note Currently not supported! */
    DECOMPRESS
  };
  Q_ENUM(operation);

  enum format { DEFLATE, GZIP };
  Q_ENUM(format);

  explicit ACompressedHeader(operation o, format f);
  const QByteArray decompressDeflate(const QByteArray &data);
  const QByteArray decompressGzip(const QByteArray &data);

private:
  operation p_operation;
  format p_format;
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ACOMPRESSEDHEADER_H
