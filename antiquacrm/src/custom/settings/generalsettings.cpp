// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "generalsettings.h"
#include "myicontheme.h"

#include <QtWidgets>

GeneralSettings::GeneralSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("general_settings");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("general_config_layout");

  int row = 0; /**< Counter */
  QString buffer = QString();
  QString txt_open_dir = tr("Open Directory");
  QIcon icon_open_btn = myIcon("folder_green");

  // BEGIN Archives
  QGroupBox *m_grouBox1 = new QGroupBox(this);
  m_grouBox1->setObjectName("groupbox_archives");
  m_grouBox1->setTitle(tr("Archive paths"));
  QGridLayout *lt_groupBox1 = new QGridLayout(m_grouBox1);
  // Images
  m_img_archiv_path = new LineEdit(this);
  m_img_archiv_path->setObjectName("dirs/images");
  m_img_archiv_path->setInfo(tr("Images"));
  lt_groupBox1->addWidget(m_img_archiv_path, row, 0, 1, 1);
  btn_imgsrc = new QToolButton(m_grouBox1);
  btn_imgsrc->setIcon(icon_open_btn);
  btn_imgsrc->setText(txt_open_dir);
  lt_groupBox1->addWidget(btn_imgsrc, row++, 1, 1, 1);
  connect(btn_imgsrc, SIGNAL(clicked()), this, SLOT(setImageArchiv()));
  // Delivery Notes
  m_delivery_path = new LineEdit(this);
  m_delivery_path->setObjectName("dirs/deliverynotes");
  m_delivery_path->setInfo(tr("Delivery Notes"));
  lt_groupBox1->addWidget(m_delivery_path, row, 0, 1, 1);
  btn_delivery = new QToolButton(m_grouBox1);
  btn_delivery->setIcon(icon_open_btn);
  btn_delivery->setText(txt_open_dir);
  lt_groupBox1->addWidget(btn_delivery, row++, 1, 1, 1);
  m_grouBox1->setLayout(lt_groupBox1);
  connect(btn_delivery, SIGNAL(clicked()), this, SLOT(setDeliveryArchiv()));
  // Invoices
  m_invoice_path = new LineEdit(this);
  m_invoice_path->setObjectName("dirs/invoices");
  m_invoice_path->setInfo(tr("Invoices"));
  lt_groupBox1->addWidget(m_invoice_path, row, 0, 1, 1);
  btn_invoice = new QToolButton(m_grouBox1);
  btn_invoice->setIcon(icon_open_btn);
  btn_invoice->setText(txt_open_dir);
  lt_groupBox1->addWidget(btn_invoice, row++, 1, 1, 1);
  connect(btn_invoice, SIGNAL(clicked()), this, SLOT(setInvoiceArchiv()));
  // Cards
  m_cards_path = new LineEdit(this);
  m_cards_path->setObjectName("dirs/cards");
  m_cards_path->setInfo(tr("Cards"));
  lt_groupBox1->addWidget(m_cards_path, row, 0, 1, 1);
  btn_cards = new QToolButton(m_grouBox1);
  btn_cards->setIcon(icon_open_btn);
  btn_cards->setText(txt_open_dir);
  lt_groupBox1->addWidget(btn_cards, row++, 1, 1, 1);
  connect(btn_cards, SIGNAL(clicked()), this, SLOT(setCardsArchiv()));
  m_grouBox1->setLayout(lt_groupBox1);
  layout->addWidget(m_grouBox1);
  buffer.clear();
  // END

  // BEGIN
  row = 0; /**< Counter zurücksetzen */
  QGroupBox *m_grouBox2 = new QGroupBox(this);
  m_grouBox2->setObjectName("groupbox_additional");
  m_grouBox2->setTitle(tr("Additional"));
  QVBoxLayout *lt_groupBox2 = new QVBoxLayout(m_grouBox2);
  // Search
  buffer = tr("From how many chars does the search start when you type in.");
  m_searchStart = new IntSpinBox(5, 50, this);
  m_searchStart->setObjectName("search/startlength");
  m_searchStart->setInfo(buffer);
  lt_groupBox2->addWidget(m_searchStart);
  // Preis
  buffer = tr("The lowest permissible selling price.");
  m_minPrice = new IntSpinBox(5, 100, m_grouBox2);
  m_minPrice->setObjectName("payment/min_price");
  m_minPrice->setInfo(buffer);
  lt_groupBox2->addWidget(m_minPrice);
  buffer.clear();
  // Währung
  m_currency = new Ecurrency(m_grouBox2);
  m_currency->setObjectName("payment/currency");
  m_currency->setInfo(tr("currency for price calculation."));
  lt_groupBox2->addWidget(m_currency);
  // Mehwertsteuer
  QFrame *vat_frame = new QFrame(m_grouBox2);
  QGridLayout *vat_layout = new QGridLayout(vat_frame);
  m_vat1 = new IntSpinBox(vat_frame);
  m_vat1->setObjectName("payment/vat1");
  m_vat1->setInfo(tr("VAT (normal)"));
  m_vat1->setRange(1, 50);
  m_vat1->setValue("19");
  m_vat1->setSuffix("%");
  vat_layout->addWidget(m_vat1, 0, 1, 1, 1, Qt::AlignLeft);
  m_vat2 = new IntSpinBox(vat_frame);
  m_vat2->setObjectName("payment/vat2");
  m_vat2->setInfo(tr("VAT (reduced)"));
  m_vat2->setRange(1, 50);
  m_vat2->setValue("7");
  m_vat2->setSuffix("%");
  vat_layout->addWidget(m_vat2, 1, 1, 1, 1, Qt::AlignLeft);
  vat_frame->setLayout(vat_layout);
  lt_groupBox2->addWidget(vat_frame, Qt::AlignLeft);
  // Minimum Image Size
  QFrame *imageSizeFrame = new QFrame(m_grouBox2);
  QVBoxLayout *image_layout = new QVBoxLayout(imageSizeFrame);
  QString sizeinfo = tr("Set size range for Imagepreviews.");
  image_layout->addWidget(new QLabel(sizeinfo, imageSizeFrame));
  m_minSize = new SizeEdit(imageSizeFrame);
  m_minSize->setObjectName("image/min_size");
  m_minSize->setMinimum(80, 80);
  m_minSize->setMaximum(800, 800);
  m_minSize->setValue(QSize(125, 125));
  m_minSize->setInfo(tr("Minimum Image size"));
  image_layout->addWidget(m_minSize);
  // Maximum Image Size
  m_maxSize = new SizeEdit(imageSizeFrame);
  m_maxSize->setObjectName("image/max_size");
  m_maxSize->setMinimum(80, 80);
  m_maxSize->setMaximum(800, 800);
  m_maxSize->setValue(QSize(320, 320));
  m_maxSize->setInfo(tr("Maximum Image size"));
  image_layout->addWidget(m_maxSize);
  imageSizeFrame->setLayout(image_layout);
  lt_groupBox2->addWidget(imageSizeFrame);

  m_grouBox2->setLayout(lt_groupBox2);
  layout->addWidget(m_grouBox2);
  // END

  // BEGIN
  row = 0; /**< Counter zurücksetzen */
  QGroupBox *m_grouBox3 = new QGroupBox(this);
  m_grouBox3->setObjectName("groupbox_additional");
  m_grouBox3->setTitle(tr("Font Settings"));
  QGridLayout *lt_groupBox3 = new QGridLayout(m_grouBox3);
  // Font Settings
  view_font_config = new QLabel(this);
  view_font_config->setText(tr("fonts representation"));
  lt_groupBox3->addWidget(view_font_config, row++, 0, 1, 1);
  buffer = tr("open font settings");
  btn_fontdialog = new QPushButton(buffer, m_grouBox3);
  btn_fontdialog->setIcon(icon_open_btn);
  lt_groupBox3->addWidget(btn_fontdialog, row++, 1, 1, 1, Qt::AlignLeft);
  layout->addWidget(m_grouBox3);
  connect(btn_fontdialog, SIGNAL(clicked()), this, SLOT(openFontDialog()));
  // END

  layout->addStretch(1);
  setLayout(layout);

  initSignalChanged();
}

void GeneralSettings::initSignalChanged() {
  UtilsMainList l =
      findChildren<UtilsMain *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      UtilsMain *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(hasModified(bool)), this, SLOT(chieldModified(bool)));
      }
    }
  }
}

void GeneralSettings::openFontDialog() {
  bool b = true;
  QFont font = QFontDialog::getFont(&b, qApp->font(), this);
  if (b) {
    view_font_config->setFont(font);
    chieldModified(true);
  }
}

void GeneralSettings::setImageArchiv() {
  QVariant spath = config->value("dirs/images", QDir::homePath());
  QString src = getDirectory(spath.toString());
  if (!src.isEmpty()) {
    m_img_archiv_path->setValue(src);
  }
}

void GeneralSettings::setDeliveryArchiv() {
  QVariant spath = config->value("dirs/deliverynotes", QDir::homePath());
  QString src = getDirectory(spath.toString());
  if (!src.isEmpty()) {
    m_delivery_path->setValue(src);
  }
}

void GeneralSettings::setInvoiceArchiv() {
  QVariant spath = config->value("dirs/invoices", QDir::homePath());
  QString src = getDirectory(spath.toString());
  if (!src.isEmpty()) {
    m_invoice_path->setValue(src);
  }
}

void GeneralSettings::setCardsArchiv() {
  QVariant spath = config->value("dirs/cards", QDir::homePath());
  QString src = getDirectory(spath.toString());
  if (!src.isEmpty()) {
    m_cards_path->setValue(src);
  }
}

void GeneralSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString GeneralSettings::getPageTitle() { return pageTitle; }

void GeneralSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon GeneralSettings::getPageIcon() { return pageIcon; }

void GeneralSettings::loadSectionConfig() {
  UtilsMainList l =
      findChildren<UtilsMain *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      UtilsMain *w = l.at(i);
      if (w != nullptr && config->contains(w->objectName())) {
        w->setValue(config->value(w->objectName()));
      }
    }
  }
  config->beginGroup("application");
  QFont font = qApp->font();
  if (!font.fromString(config->value("font").toString())) {
    font.setFamily(config->value("font_family").toString());
    font.setPointSize(config->value("font_size", font.pointSize()).toInt());
  }
  view_font_config->setFont(font);
  view_font_config->setText(tr("fonts representation"));
  config->endGroup();
}

void GeneralSettings::saveSectionConfig() {
  QFont font = view_font_config->font();
  config->beginGroup("application");
  config->setValue("font", font.toString());
  config->setValue("font_size", font.pointSize());
  config->setValue("font_family", font.family());
  config->endGroup();

  UtilsMainList l =
      findChildren<UtilsMain *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      UtilsMain *w = l.at(i);
      if (w != nullptr) {
        config->setValue(w->objectName(), w->value());
      }
    }
  }
}
