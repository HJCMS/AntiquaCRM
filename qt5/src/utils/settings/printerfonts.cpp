// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerfonts.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QIcon>
#include <QLayout>
#include <QStyle>
#include <QtWidgets>

PrinterFonts::PrinterFonts(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("Primary font settings for all documents to be printed."));

  m_signalMapper = new QSignalMapper(this);
  m_signalMapper->setObjectName("print_font_signals");

  QGridLayout *layout = new QGridLayout(this);

  int row = 0;
  QString _title = tr("Document Header");
  QString _desc = tr("Letterhead on invoices and delivery notes.");
  layout->addWidget(section("header", _title), row, 0, 1, 1);
  layout->addWidget(button("header"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  _title = tr("Document Body");
  _desc = tr("Standard font for table content and text blocks.");
  layout->addWidget(section("body", _title), row, 0, 1, 1);
  layout->addWidget(button("body"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  _title = tr("Document Footer");
  _desc = tr("Font format in document footer.");
  layout->addWidget(section("footer", _title), row, 0, 1, 1);
  layout->addWidget(button("footer"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  _title = tr("Normal Text");
  _desc = tr("Standard font for content and text blocks");
  layout->addWidget(section("normal", _title), row, 0, 1, 1);
  layout->addWidget(button("normal"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  _title = tr("Address Text");
  _desc = tr("Font for letterhead recipient address.");
  layout->addWidget(section("address", _title), row, 0, 1, 1);
  layout->addWidget(button("address"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  _title = tr("Subject Text");
  _desc = tr("Font for letterhead subject table.");
  layout->addWidget(section("subject", _title), row, 0, 1, 1);
  layout->addWidget(button("subject"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  _title = tr("Small Text");
  _desc = tr("Very small font for notes and additions.");
  layout->addWidget(section("small", _title), row, 0, 1, 1);
  layout->addWidget(button("small"), row, 1, 1, 1);
  layout->addWidget(description(_desc), row++, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(m_signalMapper, SIGNAL(mappedString(const QString &)),
          SLOT(setSectionFont(const QString &)));
}

QLabel *PrinterFonts::section(const QString &prefix, const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setObjectName(prefix + "_font");
  lb->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return lb;
}

QPushButton *PrinterFonts::button(const QString &objName) {
  QIcon icon = style()->standardIcon(QStyle::SP_DirIcon);
  QPushButton *btn = new QPushButton(tr("Select font"), this);
  btn->setObjectName("btn_" + objName);
  btn->setToolTip(tr("Select and change font"));
  btn->setIcon(icon);
  connect(btn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(btn, objName + "_font");
  return btn;
}

QLabel *PrinterFonts::description(const QString &text) {
  QLabel *lb = new QLabel(text, this);
  lb->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  return lb;
}

void PrinterFonts::updateSection(QLabel *lb) {
  QFontDialog *d = new QFontDialog(this);
  d->setOption(QFontDialog::ScalableFonts, true);
  d->setOption(QFontDialog::NonScalableFonts, false);
  d->setOption(QFontDialog::MonospacedFonts, false);
  d->setMinimumSize(QSize(400, 450));
  d->setWindowTitle(lb->text());
  d->setCurrentFont(lb->font());
  if (d->exec() == QDialog::Accepted) {
    lb->setFont(d->currentFont());
    setWindowModified(true);
  }
  d->deleteLater();
}

void PrinterFonts::setSectionFont(const QString &name) {
  QLabel *l = findChild<QLabel *>(name, Qt::FindDirectChildrenOnly);
  if (l != nullptr)
    updateSection(l);
}

void PrinterFonts::loadFonts(AntiquaCRM::ASettings *config) {
  config->beginGroup("printer");
  QList<QLabel *> labels =
      findChildren<QLabel *>(p_fontPattern, Qt::FindDirectChildrenOnly);
  if (labels.count() > 1) {
    for (int i = 0; i < labels.count(); i++) {
      QLabel *w = labels.at(i);
      if (w != nullptr) {
        QString name(w->objectName());
        if (name.isEmpty() || !name.contains("font"))
          continue;

        QFont font;
        font.fromString(config->value(name, qApp->font()).toString());
        w->setFont(font);
      }
    }
  }
  config->endGroup();
}

void PrinterFonts::saveFonts(AntiquaCRM::ASettings *config) {
  config->beginGroup("printer");
  QList<QLabel *> labels =
      findChildren<QLabel *>(p_fontPattern, Qt::FindDirectChildrenOnly);
  if (labels.count() > 1) {
    for (int i = 0; i < labels.count(); i++) {
      QLabel *w = labels.at(i);
      if (w != nullptr) {
        if (w->objectName().isEmpty())
          continue;

        config->setValue(w->objectName(), w->font().toString());
      }
    }
  }
  config->endGroup();
}
