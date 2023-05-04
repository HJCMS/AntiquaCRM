// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printerfonts.h"
#include "awhatsthisbutton.h"

#include <QDialog>
#include <QFontDialog>
#include <QGridLayout>

PrinterFonts::PrinterFonts(QWidget *parent) : QGroupBox{parent} {
  setTitle(tr("Primary font settings for all documents to be printed."));

  m_signalMapper = new QSignalMapper(this);
  m_signalMapper->setObjectName("print_font_signals");

  QGridLayout *layout = new QGridLayout(this);

  int row = 0;
  QString _title = tr("Document heading");
  QString _desc = tr("Letterhead on invoices and delivery notes.");
  layout->addWidget(section("header", _title), row, 0, 1, 1);
  layout->addWidget(button("header"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Document Body");
  _desc = tr("Standard font for table content and text blocks.");
  layout->addWidget(section("body", _title), row, 0, 1, 1);
  layout->addWidget(button("body"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Document Footer");
  _desc = tr("Font format in document footer.");
  layout->addWidget(section("footer", _title), row, 0, 1, 1);
  layout->addWidget(button("footer"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Normal inline text");
  _desc = tr("Standard font for content and text blocks");
  layout->addWidget(section("normal", _title), row, 0, 1, 1);
  layout->addWidget(button("normal"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Letter Address text");
  _desc = tr("Font for letterhead recipient address.");
  layout->addWidget(section("address", _title), row, 0, 1, 1);
  layout->addWidget(button("address"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Letter subject text");
  _desc = tr("Font for letterhead subject table.");
  layout->addWidget(section("subject", _title), row, 0, 1, 1);
  layout->addWidget(button("subject"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Small inline text");
  _desc = tr("Very small font for notes and additions.");
  layout->addWidget(section("small", _title), row, 0, 1, 1);
  layout->addWidget(button("small"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(m_signalMapper, SIGNAL(mappedString(const QString &)),
          SLOT(setSectionFont(const QString &)));
}

QLabel *PrinterFonts::section(const QString &suffix, const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setObjectName(p_font_prefix + suffix);
  lb->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return lb;
}

QPushButton *PrinterFonts::button(const QString &objName) {
  QPushButton *btn = new QPushButton(this);
  btn->setObjectName("btn_" + objName);
  btn->setToolTip(tr("Change this font..."));
  btn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
  connect(btn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(btn, p_font_prefix + objName);
  return btn;
}

AntiquaCRM::AWhatsThisButton *PrinterFonts::fontTip(const QString &text) {
  AntiquaCRM::AWhatsThisButton *m_tbn =
      new AntiquaCRM::AWhatsThisButton(text, this);
  m_tbn->setFocusPolicy(Qt::NoFocus);
  return m_tbn;
}

void PrinterFonts::updateSection(QLabel *lb) {
  QFontDialog *d = new QFontDialog(this);
  d->setOption(QFontDialog::ScalableFonts, true);
  d->setOption(QFontDialog::NonScalableFonts, false);
  d->setOption(QFontDialog::MonospacedFonts, false);
  d->setMinimumSize(QSize(450, 450));
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

void PrinterFonts::loadSection(AntiquaCRM::ASettings *config) {
  QList<QLabel *> labels = findChildren<QLabel *>(fontPattern(), // pattern
                                                  Qt::FindDirectChildrenOnly);
  if (labels.count() > 1) {
    for (int i = 0; i < labels.count(); i++) {
      QLabel *w = labels.at(i);
      if (w != nullptr) {
        QString name(w->objectName());
        if (name.isEmpty() || !name.contains(p_font_prefix))
          continue;

        QFont font;
        font.fromString(config->value(name, qApp->font()).toString());
        w->setFont(font);
      }
    }
  }
}

void PrinterFonts::saveSection(AntiquaCRM::ASettings *config) {
  QList<QLabel *> labels = findChildren<QLabel *>(fontPattern(), // pattern
                                                  Qt::FindDirectChildrenOnly);
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
}

const QRegularExpression PrinterFonts::fontPattern() {
  return QRegularExpression("^print_font_\\w+$");
}
