// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "acompressedheader.h"

namespace AntiquaCRM {

//
constexpr std::size_t default_buffer_size = 256 * 1024;
// Maximum value for windowBits in deflateInit2 and inflateInit2
constexpr int default_window_bits = MAX_WBITS;
// Maximum value for memLevel in deflateInit2
constexpr int default_mem_level = MAX_MEM_LEVEL;

ACompressedHeader::ACompressedHeader(operation o, format f)
    : p_operation{o}, p_format{f} {
    qWarning("Experimental class - not ready!");
}

const QByteArray ACompressedHeader::decompressDeflate(const QByteArray &data) {
  // TODO
  Q_UNUSED(data);
  z_stream m_zlib_stream;
  if (inflateInit(&m_zlib_stream) != Z_OK) {
    qWarning("inflateInit failed while decompressing.");
    return QByteArray();
  }
//  std::string str = data.toStdString();
//  m_zlib_stream.next_in = (Bytef *)str.data();
//  m_zlib_stream.avail_in = str.size();

  return QByteArray();
}

/**
 * https://zlib.net/zlib_faq.html#faq20
 * https://github.com/Stiffstream/restinio/blob/master/dev/restinio/transforms/zlib.hpp
 */
const QByteArray ACompressedHeader::decompressGzip(const QByteArray &data) {
  z_stream m_zlib_stream;
  memset(&m_zlib_stream, 0, default_mem_level);
  if (inflateInit2(&m_zlib_stream, default_window_bits) != Z_OK) {
    qWarning("inflateInit2 failed while decompressing.");
    return QByteArray();
  }

  std::string str = data.toStdString();
  m_zlib_stream.next_in = (Bytef *)str.data();
  m_zlib_stream.avail_in = str.size();

  int ret;
  std::size_t buffer_size = default_buffer_size;
  char outbuffer[buffer_size];
  std::string buffer;
  do {
    m_zlib_stream.next_out = reinterpret_cast<Bytef *>(outbuffer);
    m_zlib_stream.avail_out = buffer_size;
    ret = inflate(&m_zlib_stream, 0);
    if (buffer.size() < m_zlib_stream.total_out) {
      buffer.append(outbuffer, m_zlib_stream.total_out - buffer.size());
    }
  } while (ret == Z_OK);

  inflateEnd(&m_zlib_stream);

  if (ret != Z_STREAM_END) {
    qWarning("Error - ZLIB Decompression: %s", qPrintable(m_zlib_stream.msg));
    return QByteArray();
  }

  return QByteArray::fromStdString(buffer);
}

}; // namespace AntiquaCRM
