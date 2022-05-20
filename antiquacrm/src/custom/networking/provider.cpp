// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provider.h"

#include <QByteArray>
#include <QDebug>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLocale>
#include <iostream>

#ifndef ENABLE_DEBUG
#define ENABLE_DEBUG true
#endif

// BEGIN ProviderRequest

static int cUrlTrace(CURL *handle, curl_infotype type, unsigned char *data,
                     size_t size, void *userp) {
  QString info;
  switch (type) {
  case CURLINFO_TEXT:
    info.append("Info Text:");
    break;

  case CURLINFO_HEADER_IN:
    info.append("Header IN:");
    break;

  case CURLINFO_HEADER_OUT:
    info.append("Header OUT:");
    break;

  case CURLINFO_DATA_IN:
    info.append("cUrl Data IN");
    break;

  case CURLINFO_DATA_OUT:
    info.append("cUrl Data OUT");
    break;

  case CURLINFO_SSL_DATA_IN:
    info.append("SSL Data IN");
    break;

  case CURLINFO_SSL_DATA_OUT:
    info.append("SSL Data OUT");
    break;

  default:
    info.append("Unknown");
  }
  qInfo("%s %s", qPrintable(info), data);
  return 1;
}

static const char *jsonParserError(int err) {
  switch (err) {
  case (QJsonParseError::UnterminatedObject):
    return "Object is not correctly terminated with a closing curly bracket";

  case (QJsonParseError::MissingNameSeparator):
    return "A comma separating different items is missing";

  case (QJsonParseError::UnterminatedArray):
    return "Array is not correctly terminated with a closing square bracket";

  case (QJsonParseError::MissingValueSeparator):
    return "A colon separating keys from values inside objects is missing";

  case (QJsonParseError::IllegalValue):
    return "The value is illegal";

  case (QJsonParseError::TerminationByNumber):
    return "The input stream ended while parsing a number";

  case (QJsonParseError::IllegalNumber):
    return "The number is not well formed";

  case (QJsonParseError::IllegalEscapeSequence):
    return "An illegal escape sequence occurred in the input";

  case (QJsonParseError::IllegalUTF8String):
    return "An illegal UTF8 sequence occurred in the input";

  case (QJsonParseError::UnterminatedString):
    return "A string wasn't terminated with a quote";

  case (QJsonParseError::MissingObject):
    return "An object was expected but couldn't be found";

  case (QJsonParseError::DeepNesting):
    return "Document is too deeply nested for the parser to parse it";

  case (QJsonParseError::DocumentTooLarge):
    return "Document is too large for the parser to parse it";

  case (QJsonParseError::GarbageAtEnd):
    return "Document contains additional garbage characters at the end";

  default:
    return "Unknown";
  };
}

ProviderRequest::ProviderRequest(bool debug) : QRunnable() {
  verbose = debug;

  m_txtCodec = QTextCodec::codecForLocale();
  QString charset = QString::fromLocal8Bit(m_txtCodec->name());
  if (!charset.isEmpty()) {
    p_charset = charset.replace("-", "").toLower();
  }
  charset.clear();

  curl_global_init(CURL_GLOBAL_SSL);
}

const char *ProviderRequest::acceptLanguage() {
  QLocale locale = QLocale::system();
  QString str("Accept-Language: ");
  str.append(locale.bcp47Name());
  str.append(", ");
  str.append(locale.name());
  str.append(".");
  str.append(p_charset);
  str.append("; q=0.8, en;q=0.7");

  QByteArray ret = str.toLocal8Bit();
  char *value = ret.data();
  str.clear();
  ret.clear();
  return value;
}

const char *ProviderRequest::acceptJsonLanguage() {
  QString str("Accept: ");
  str.append("application/ld+json,application/json,text/*;q=0.1");

  QByteArray ret = str.toLocal8Bit();
  char *value = ret.data();
  str.clear();
  ret.clear();
  return value;
}

const char *ProviderRequest::contentTypeJson() {
  QString str("Content-Type: application/json");
  if (!p_charset.isEmpty())
    str.append("; charset=" + p_charset);

  QByteArray ret = str.toLocal8Bit();
  char *value = ret.data();
  str.clear();
  ret.clear();
  return value;
}

curl_slist *ProviderRequest::setHeader() {
  curl_slist *list = NULL;
  list = curl_slist_append(list, acceptLanguage());
  list = curl_slist_append(list, acceptJsonLanguage());
  // list = curl_slist_append(list, "Accept-Encoding: gzip, deflate");
  list = curl_slist_append(list, contentTypeJson());
  return list;
}

int ProviderRequest::writeData(char *data, size_t size, size_t nitems,
                               std::string *buffer) {
  qint64 ret = 0;
  if (buffer != NULL) {
    std::string str(data);
#if ENABLE_DEBUG == true
    qDebug() << Q_FUNC_INFO << data;
#endif
    buffer->append(str, 0, (size * nitems));
    ret = (size * nitems);
  }
  return ret;
}

size_t ProviderRequest::readFile(char *data, size_t size, size_t nmemb,
                                 std::string *buffer) {
  /**
   * @todo FileUpload
   *  https://curl.se/libcurl/c/CURLOPT_READFUNCTION.html
   */
  FILE *readhere = (FILE *)buffer;
  curl_off_t nread;
  size_t retcode = fread(data, size, nmemb, readhere);
  nread = (curl_off_t)retcode;
  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T " bytes from file\n",
          nread);

  return retcode;
}

const QString ProviderRequest::getBuffer() {
  QByteArray array;
  for (int i = 0; i < dataBuffer.length(); i++) {
    array.append(dataBuffer[i]);
  }
  QByteArray buf = QByteArray::fromStdString(dataBuffer);
  return m_txtCodec->fromUnicode(buf);
}

bool ProviderRequest::createDocument() {
  QString data = getBuffer();
  QJsonParseError parser;
  QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit(), &parser);
  if (parser.error != QJsonParseError::NoError) {
    qWarning("Json Parse Error:(%s)!", jsonParserError(parser.error));
    return false;
  }
  p_json.swap(doc);
  return true;
}

void ProviderRequest::setRemoteUrl(const QUrl &url) {
  QByteArray ba = QUrl(url).toEncoded();
  char *_url = reinterpret_cast<char *>(ba.data());
  curl_easy_setopt(m_curl, CURLOPT_URL, _url);
  curl_easy_setopt(m_curl, CURLOPT_PORT, 443L);
}

bool ProviderRequest::initDefaultOptions() {
  if (m_curl == nullptr)
    return false;

  // https://everything.curl.dev/libcurl/options/all
  // Fail on HTTP 4xx errors
  curl_easy_setopt(m_curl, CURLOPT_FAILONERROR, true);
  curl_easy_setopt(m_curl, CURLOPT_ERRORBUFFER, &errorBuffer);
  curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &dataBuffer);
  // TODO curl_easy_setopt(m_curl, CURLOPT_READDATA , &fileBuffer);
  curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
  // TODO curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, readFile);
  curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, true);
  curl_easy_setopt(m_curl, CURLOPT_USE_SSL, true);
  // is redirected, so we tell cUrl to follow redirection
  curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt(m_curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);
  curl_easy_setopt(m_curl, CURLOPT_REDIR_PROTOCOLS, CURLPROTO_HTTPS);
  curl_easy_setopt(m_curl, CURLOPT_DEFAULT_PROTOCOL, "https");
  curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, true);
  curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 1);
  curl_easy_setopt(m_curl, CURLOPT_SERVICE_NAME, "AntiquaCRM");
  curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "AntiquaCRM/0.1");
  // curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/ca-bundle.pem");

  if (verbose) {
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, cUrlTrace);
  }

  return true;
}

const QString ProviderRequest::getResponseError() {
  QString data = QString::fromStdString(errorBuffer);
#if ENABLE_DEBUG == true
  qDebug() << Q_FUNC_INFO << data;
#endif
  return data;
}

const QJsonDocument ProviderRequest::getResponseData() { return p_json; }

bool ProviderRequest::get(const QUrl &url) {
  m_curl = curl_easy_init();
  if (m_curl == nullptr) {
    qWarning("cUrl initialisation failed!");
    return false;
  }
  initDefaultOptions();
  setRemoteUrl(url);
  // curl_easy_setopt(m_curl, );
  return true;
}

bool ProviderRequest::post(const QUrl &url, const QByteArray &body) {
  m_curl = curl_easy_init();
  if (m_curl == nullptr) {
    qWarning("cUrl initialisation failed!");
    return false;
  }
  initDefaultOptions();
  setRemoteUrl(url);
  curl_easy_setopt(m_curl, CURLOPT_POST, 1);
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, body.data());
  return true;
}

bool ProviderRequest::postForm(const QUrl &url, const QString &param,
                               const QByteArray &body) {
#if ENABLE_DEBUG == true
  qDebug() << Q_FUNC_INFO << "TODO" << url << body;
#endif
  // curl_easy_setopt(m_curl,CURLOPT_HTTPPOST, true);
  // curl_easy_setopt(m_curl,CURLOPT_POST, );
  // curl_easy_setopt(m_curl,CURLOPT_POSTFIELDS, );
  // curl_easy_setopt(m_curl,CURLOPT_POSTFIELDSIZE, );
  // curl_easy_setopt(m_curl,CURLOPT_POSTFIELDSIZE_LARGE, );
  return false; // start();
}

void ProviderRequest::run() {
  if (m_curl == nullptr) {
    qFatal("cUrl use (get|post|postForm) first!");
    return;
  }
  curl_slist *header = setHeader();
  curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, header);
  CURLcode res = curl_easy_perform(m_curl);
  if (res != CURLE_OK) {
    qWarning("failed to perform cUrl request!");
    curl_slist_free_all(header);
    curl_easy_cleanup(m_curl);
    return;
  }
  long result;
  curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &result);
  if (result != 200) {
    QByteArray data = QString::number(result).toLocal8Bit();
    qWarning("Failed with HTTP Status: %s", data.data());
  }
  createDocument();
  curl_slist_free_all(header);
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();
}

ProviderRequest::~ProviderRequest() {
  if (m_curl != nullptr) {
    curl_easy_cleanup(m_curl);
  }
  curl_global_cleanup();
}
// END

// BEGIN Provider
Provider::Provider(QObject *parent, bool debug) : QThread{parent} {
  verbose = debug;
}

void Provider::sendGet(const QUrl &url) {
  p_type = Provider::GET;
  p_url = url;
  run();
}

void Provider::sendPost(const QUrl &url, const QByteArray &body) {
  p_type = Provider::POST;
  p_url = url;
  p_body = body;
  run();
}

void Provider::sendPostForm(const QUrl &url, const QString &param,
                            const QByteArray &body) {
  p_type = Provider::FORM;
  p_url = url;
  p_formdata = param;
  p_body = body;
  run();
}

const QJsonDocument Provider::getDocument() { return p_json; }

void Provider::run() {
  p_mutex.lock();
  ProviderRequest *req = new ProviderRequest(verbose);
  bool status = false;
  switch (p_type) {
  case (Provider::POST):
    status = req->post(p_url, p_body);
    break;

  case (Provider::FORM):
    status = req->postForm(p_url, p_formdata, p_body);
    break;

  default:
    status = req->get(p_url);
  };
  if (status) {
    req->run();
    p_json = req->getResponseData();
  } else {
    emit errors(req->getResponseError());
  }
#if ENABLE_DEBUG == true
  qDebug() << status << p_url << p_body << p_formdata << p_json;
#endif
  p_mutex.unlock();
  emit responsed(p_json);
  emit requestFinished(status);
}

// END
