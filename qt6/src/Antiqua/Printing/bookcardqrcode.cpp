// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookcardqrcode.h"

#include <QByteArray>
#include <QSize>

extern "C" {
#include "qrencode.h"
};

namespace AntiquaCRM {

BookCardQrCode::BookCardQrCode(const QUrl &url, int size)
    : p_url(url), p_size(size) {}

const QImage BookCardQrCode::image() {
  QImage image = QImage();
  if (!p_url.isValid())
    return image;

  QByteArray array = p_url.toEncoded(QUrl::None);
  std::string str = array.toStdString();

  QRcode *code = QRcode_encodeString(str.c_str(),  /**< data. */
                                     0,            /**< version */
                                     QR_ECLEVEL_L, /**< level */
                                     QR_MODE_8,    /**< UTF-8 */
                                     1             /**< sensitive */
  );

  image = QImage(code->width + 8, code->width + 8, QImage::Format_RGB32);
  image.fill(0xffffff);
  unsigned char *px = code->data;
  for (int y = 0; y < code->width; y++) {
    for (int x = 0; x < code->width; x++) {
      image.setPixel(x, y, ((*px & 0x1) ? 0x000000 : 0xffffff));
      px++;
    }
  }
  QRcode_free(code);

  return image.scaled(p_size, p_size);
}

} // namespace AntiquaCRM
