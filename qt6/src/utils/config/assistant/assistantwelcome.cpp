// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "assistantwelcome.h"

#include <AntiquaCRM>
#include <QFileInfo>
#include <QTextStream>
#include <QVBoxLayout>

AssistantWelcome::AssistantWelcome(QWidget* parent) : QWidget{parent} {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 4, 4, 4);

  m_text = new QLabel(this);
  m_text->setWordWrap(true);
  layout->addWidget(m_text);

  layout->addStretch(1);
  setLayout(layout);

  setContent();
}

void AssistantWelcome::setContent(const QString& key) {
  // @note Currently - only bcp47 (de) is supported in ressource file.
  QString _filename("de"); // = QLocale::system().bcp47Name().toLower();
  _filename.append(".txt");
  _filename.prepend(key.trimmed() + "_");

  QFileInfo _info(AntiquaCRM::ASettings::getDataDir("documents"), _filename);
  if (!_info.isReadable()) {
    qWarning("Missing „%s“ file.", qPrintable(_info.filePath()));
    m_text->setText(QString("Error: Invalid documents target!"));
    return;
  }

  QFile fp(_info.filePath());
  if (fp.open(QFile::ReadOnly)) {
    QString buffer;
    QTextStream in(&fp);
    while (!in.atEnd()) {
      buffer.append(in.readLine());
    }
    m_text->setText(buffer);
  }
  fp.close();
}
