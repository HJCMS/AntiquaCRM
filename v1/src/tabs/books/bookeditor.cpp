// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookeditor.h"

#include <AntiquaCRM>
#include <QDebug>

BookEditor::BookEditor(QWidget *parent)
    : InventoryEditor{"^ib_[a-z_]+\\b$", parent} {
  setObjectName("book_editor");
  setWindowTitle(tr("Edit Book"));

  AntiquaCRM::ASettings config(this);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  // Row 0
  QHBoxLayout *row0 = new QHBoxLayout();
  ib_id = new SerialID(this);
  ib_id->setObjectName("ib_id");
  ib_id->setInfo(tr("Article ID"));
  ib_id->setRequired(true);
  row0->addWidget(ib_id);

  ib_count = new IntSpinBox(this);
  ib_count->setObjectName("ib_count");
  ib_count->setInfo(tr("Count"));
  row0->addWidget(ib_count);

  double minPrice = config.value("payment/min_price", 5.00).toDouble();
  ib_price = new PriceEdit(this);
  ib_price->setObjectName("ib_price");
  ib_price->setRequired(true);
  ib_price->setInfo(tr("Price"));
  ib_price->setMinimum(minPrice);
  row0->addWidget(ib_price);

  ib_including_vat = new BoolBox(this);
  ib_including_vat->setObjectName("ib_including_vat");
  ib_including_vat->setInfo(tr("incl. vat"));
  ib_including_vat->setRequired(false);
  ib_including_vat->setToolTip(tr("VAT Settings must be set."));
  row0->addWidget(ib_including_vat);

  ib_signed = new BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setInfo(tr("Signed Version"));
  row0->addWidget(ib_signed);

  ib_restricted = new BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setInfo(tr("Restricted Sale"));
  ib_restricted->setToolTip(
      tr("Is the title not for sale nationally or is it on a censorship list. "
         "This is relevant for the Shopsystem."));

  row0->addWidget(ib_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  QHBoxLayout *row1 = new QHBoxLayout();

  ib_edition = new IntSpinBox(this);
  ib_edition->setObjectName("ib_edition");
  ib_edition->setRequired(true);
  ib_edition->setInfo(tr("Edition"));
  row1->addWidget(ib_edition);

  ib_pagecount = new IntSpinBox(this);
  ib_pagecount->setObjectName("ib_pagecount");
  ib_pagecount->setContextMenuPolicy(Qt::DefaultContextMenu);
  ib_pagecount->setRange(10, 100000);
  ib_pagecount->setSingleStep(10);
  ib_pagecount->setInfo(tr("Page count"));
  ib_pagecount->setRequired(true);
  row1->addWidget(ib_pagecount);

  ib_weight = new IntSpinBox(this);
  ib_weight->setObjectName("ib_weight");
  ib_weight->setRange(100, 100000000);
  ib_weight->setSuffix(tr("g"));
  ib_weight->setInfo(tr("Weight"));
  ib_weight->setRequired(true);
  row1->addWidget(ib_weight);

  ib_year = new YearEdit(this);
  ib_year->setObjectName("ib_year");
  ib_year->setRequired(true);
  ib_year->setInfo(tr("Year"));
  row1->addWidget(ib_year);

  ib_volume = new IntSpinBox(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));
  ib_volume->setInfo(tr("Volume"));
  row1->addWidget(ib_volume);

  row1->addStretch(1);
  mainLayout->addLayout(row1);

  // Row 2
  QWidget *row2Widget = new QWidget(this);
  row2Widget->setMinimumHeight(380);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);

  AntiquaILabel *lb_title = new AntiquaILabel(tr("Booktitle"), row2Widget);
  row2->addWidget(lb_title, row2c, 0, 1, 1);
  ib_title = new LineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setToolTip(lb_title->text());
  row2->addWidget(ib_title, row2c++, 1, 1, 1);

  AntiquaILabel *lb_ext_title = new AntiquaILabel(tr("Book Subtitle"), this);
  row2->addWidget(lb_ext_title, row2c, 0, 1, 1);
  ib_title_extended = new LineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setToolTip(lb_ext_title->text());
  row2->addWidget(ib_title_extended, row2c++, 1, 1, 1);

  AntiquaILabel *lb_author = new AntiquaILabel(tr("Author"), this);
  row2->addWidget(lb_author, row2c, 0, 1, 1);
  ib_author = new LineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setToolTip(lb_author->text());
  row2->addWidget(ib_author, row2c++, 1, 1, 1);

  AntiquaILabel *lb_publisher = new AntiquaILabel(tr("Publisher"), this);
  row2->addWidget(lb_publisher, row2c, 0, 1, 1);
  ib_publisher = new LineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setToolTip(lb_publisher->text());
  row2->addWidget(ib_publisher, row2c++, 1, 1, 1);

  AntiquaILabel *lb_condition = new AntiquaILabel(tr("Condition"), this);
  row2->addWidget(lb_condition, row2c, 0, 1, 1);
  QHBoxLayout *conditionlayout = new QHBoxLayout();
  ib_condition = new ConditionEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setToolTip(tr("Condition of this Book."));
  conditionlayout->addWidget(ib_condition, Qt::AlignLeft);
  AntiquaILabel *lb_language = new AntiquaILabel(tr("Language"), this);
  conditionlayout->addWidget(lb_language, Qt::AlignRight);
  ib_language = new Language(this);
  ib_language->setObjectName("ib_language");
  conditionlayout->addWidget(ib_language);
  row2->addLayout(conditionlayout, row2c++, 1, 1, 1);

  AntiquaILabel *lb_designation = new AntiquaILabel(tr("Designation"), this);
  row2->addWidget(lb_designation, row2c, 0, 1, 1);
  // Bucheinband und Bindungsart
  QHBoxLayout *binding_layout = new QHBoxLayout();
  binding_layout->setContentsMargins(0, 0, 0, 0);
  ib_binding = new BookBinding(this);
  ib_binding->setObjectName("ib_binding");
  ib_binding->setToolTip(tr("Cover"));
  binding_layout->addWidget(ib_binding);
  ib_designation = new LineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setToolTip(lb_designation->text());
  binding_layout->addWidget(ib_designation);
  row2->addLayout(binding_layout, row2c++, 1, 1, 1);

  AntiquaILabel *lb_storage = new AntiquaILabel(tr("Storage"), this);
  row2->addWidget(lb_storage, row2c, 0, 1, 1);
  ib_storage = new StorageLocation(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setToolTip(lb_storage->text());
  row2->addWidget(ib_storage, row2c++, 1, 1, 1);

  AntiquaILabel *lb_keywords = new AntiquaILabel(tr("Keywords"), this);
  row2->addWidget(lb_keywords, row2c, 0, 1, 1);
  // TODO KeywordLineEdit
  row2c++;

  AntiquaILabel *lb_isbn = new AntiquaILabel(tr("ISBN"),this);
  row2->addWidget(lb_isbn, row2c, 0, 1, 1);
  // TODO ISBN Edit
  row2c++;

  // TODO Image Toolbar
  row2c++;

  QSize maxSize = config.value("image/max_size", QSize(320, 320)).toSize();
  QWidget *m_imageView = new QWidget(this);
  m_imageView->setFixedSize(maxSize);
  row2->addWidget(m_imageView, 0, 2, (row2c + 1), 1);

  row2Widget->setLayout(row2);
  mainLayout->addWidget(row2Widget);

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("tab_widget");
  m_tabWidget->setMinimumHeight(180);
  m_tabWidget->setContentsMargins(1, 1, 1, 1);
  // TODO
  mainLayout->addWidget(m_tabWidget);
  setLayout(mainLayout);
  setEnabled(false);
}

void BookEditor::setInputList() {
  m_bookData = new AntiquaCRM::ASqlTable("inventory_books");
  ignoreList << "ib_json_category";
  inputList = m_bookData->columnNames();
  if (inputList.isEmpty()) {
    qWarning("Books InputList is Empty!");
  }
  // Werden Manuel gesetzt!
  inputList.removeOne("ib_changed");
}

bool BookEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  bool required = (field.requiredStatus() == QSqlField::Required);

  InputEdit *inp = findChild<InputEdit *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    inp->setProperties(field);
    return true;
  }

  qDebug() << "Unknown:" << key << "|" << value << "|" << required;
  return false;
}

void BookEditor::importSqlResult() {
  if (m_bookData == nullptr)
    return;

  QHashIterator<QString, QVariant> it(m_bookData->getDataset());
  while (it.hasNext()) {
    it.next();
    QSqlField field = m_bookData->getProperties(it.key());
    setDataField(field, it.value());
  }
  setResetModified(inputList);
}

bool BookEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
}

const QHash<QString, QVariant> BookEditor::createSqlDataset() {
  QHash<QString, QVariant> list;
  qDebug() << Q_FUNC_INFO << "TODO";
  return list;
}

void BookEditor::createSqlUpdate() { qDebug() << Q_FUNC_INFO << "TODO"; }

void BookEditor::createSqlInsert() { qDebug() << Q_FUNC_INFO << "TODO"; }

void BookEditor::setSaveData() { qDebug() << Q_FUNC_INFO << "TODO"; }

void BookEditor::setCheckLeaveEditor() { qDebug() << Q_FUNC_INFO << "TODO"; }

void BookEditor::setFinalLeaveEditor() { qDebug() << Q_FUNC_INFO << "TODO"; }

void BookEditor::setRestore() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool BookEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString ib_id = QString::number(articleId);
  if (ib_id.isEmpty())
    return status;

  setInputList();
  QString table = m_bookData->tableName();
  QString query("SELECT * FROM " + table + " WHERE ib_id=" + ib_id + ";");
  QSqlQuery q = m_sql->query(query);
  if (q.size() != 0) {
    QSqlRecord r = m_bookData->record();
    while (q.next()) {
      foreach (QString key, inputList) {
        m_bookData->setValue(key, q.value(r.indexOf(key)));
      }
    }
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    importSqlResult();
    setEnabled(true);
  }

  return status;
}

bool BookEditor::createNewEntry() {
  setInputList();
  setEnabled(true);
  qDebug() << Q_FUNC_INFO << "TODO";
  // setResetModified(inputList);
  return true;
}
