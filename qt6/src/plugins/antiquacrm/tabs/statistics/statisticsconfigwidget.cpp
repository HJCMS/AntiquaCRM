// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsconfigwidget.h"
#include "awhatsthisbutton.h"

#include <QFontDialog>
#include <QGridLayout>

StatisticsConfigWidget::StatisticsConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"statistics", parent} {
  setContentsMargins(0, 0, 0, 0);

  m_signalMapper = new QSignalMapper(this);
  m_signalMapper->setObjectName("statistics_font_signals");

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  QString _title = tr("Font for chart header Label.");
  QString _desc = tr("Here you can change the Font for all chart headers.");
  layout->addWidget(section("header", _title), row, 0, 1, 1);
  layout->addWidget(button("header"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  _title = tr("Font for chart labeling.");
  _desc = tr("Here you can change the Font for all chart labels.");
  layout->addWidget(section("chart", _title), row, 0, 1, 1);
  layout->addWidget(button("chart"), row, 1, 1, 1);
  layout->addWidget(fontTip(_desc), row++, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(m_signalMapper, SIGNAL(mappedString(const QString &)),
          SLOT(setSectionFont(const QString &)));
}

QLabel *StatisticsConfigWidget::section(const QString &suffix,
                                        const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setObjectName(fontPrefix() + suffix);
  lb->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  return lb;
}

QPushButton *StatisticsConfigWidget::button(const QString &objName) {
  QPushButton *btn = new QPushButton(this);
  btn->setObjectName("btn_" + objName);
  btn->setToolTip(tr("Change this font..."));
  btn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
  connect(btn, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(btn, fontPrefix() + objName);
  return btn;
}

AntiquaCRM::AWhatsThisButton *
StatisticsConfigWidget::fontTip(const QString &text) {
  AntiquaCRM::AWhatsThisButton *m_tbn =
      new AntiquaCRM::AWhatsThisButton(text, this);
  m_tbn->setFocusPolicy(Qt::NoFocus);
  return m_tbn;
}

void StatisticsConfigWidget::openFontDialog(QLabel *lb) {
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

void StatisticsConfigWidget::setSectionFont(const QString &name) {
  QLabel *l = findChild<QLabel *>(name, Qt::FindDirectChildrenOnly);
  if (l != nullptr)
    openFontDialog(l);
}

void StatisticsConfigWidget::loadSectionConfig() {
  QList<QLabel *> _list = findChildren<QLabel *>(fontPattern(), // pattern
                                                 Qt::FindDirectChildrenOnly);
  if (_list.count() > 1) {
    config->beginGroup("statistics");
    for (int i = 0; i < _list.count(); i++) {
      QLabel *w = _list.at(i);
      if (w != nullptr) {
        QString name(w->objectName());
        if (name.isEmpty() || !name.startsWith(fontPrefix()))
          continue;

        QFont font;
        font.fromString(config->value(name, qApp->font()).toString());
        w->setFont(font);
      }
    }
    config->endGroup();
  }
}

void StatisticsConfigWidget::saveSectionConfig() {
  QList<QLabel *> _list = findChildren<QLabel *>(fontPattern(), // pattern
                                                 Qt::FindDirectChildrenOnly);
  if (_list.count() > 1) {
    config->beginGroup("statistics");
    for (int i = 0; i < _list.count(); i++) {
      QLabel *w = _list.at(i);
      if (w != nullptr) {
        if (w->objectName().isEmpty())
          continue;

        config->setValue(w->objectName(), w->font().toString());
      }
    }
    config->endGroup();
    setWindowModified(false);
  }
}

const QString StatisticsConfigWidget::fontPrefix() {
  return QString("stats_font_");
}

const QRegularExpression StatisticsConfigWidget::fontPattern() {
  return QRegularExpression("^" + fontPrefix() + "\\w+$");
}

AntiquaCRM::ConfigType StatisticsConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject StatisticsConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "office-chart-area");
  _o.insert("title", tr("Statistics"));
  _o.insert("tooltip", tr("Additional Statistics settings."));
  return _o;
}
