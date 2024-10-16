// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewsindex.h"
#include "viewstablemodel.h"

#include <QHeaderView>

ViewsIndex::ViewsIndex(QWidget* parent) : AntiquaCRM::TabsIndex{"views_tab", parent} {
  setObjectName("views_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("office-chart-area"));
  setWindowTitle(getTitle());
  setClosable(true);

  m_sql = new AntiquaCRM::ASqlCore(this);

  // Begin MainPage layout
  QWidget* centralWidget = new QWidget(this);
  centralWidget->setContentsMargins(0, 0, 0, 0);
  QVBoxLayout* m_layout = new QVBoxLayout(centralWidget);
  m_layout->setContentsMargins(4, 4, 4, 4);

  QFrame* m_frame = new QFrame(this);
  QHBoxLayout* f_layout = new QHBoxLayout(m_frame);
  f_layout->setContentsMargins(contentsMargins());
  f_layout->addStretch(1);
  QLabel* m_title = new QLabel(m_frame);
  m_title->setText(tr("Select/Show predefined view") + ":");
  f_layout->addWidget(m_title);
  m_comboBox = new AntiquaCRM::AComboBox(m_frame);
  f_layout->addWidget(m_comboBox);
  m_frame->setLayout(f_layout);
  m_layout->addWidget(m_frame);

  m_tableView = new QTableView(centralWidget);
  m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tableView->setCornerButtonEnabled(false);
  m_tableView->setDragEnabled(false);
  m_tableView->setDragDropOverwriteMode(false);
  m_tableView->setWordWrap(false);
  m_tableView->setAlternatingRowColors(true);
  m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tableView->setSortingEnabled(true);

  QHeaderView* h_Header = m_tableView->horizontalHeader();
  h_Header->setDefaultAlignment(Qt::AlignCenter);
  h_Header->setSectionResizeMode(QHeaderView::ResizeToContents);
  h_Header->setStretchLastSection(true);
  m_tableView->setHorizontalHeader(h_Header);

  QHeaderView* v_Header = m_tableView->verticalHeader();
  v_Header->setDefaultAlignment(Qt::AlignRight | Qt::AlignVCenter);
  v_Header->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_tableView->setVerticalHeader(v_Header);

  m_layout->addWidget(m_tableView);

  m_statusBar = new QStatusBar(centralWidget);
  m_layout->addWidget(m_statusBar);

  centralWidget->setLayout(m_layout);
  insertWidget(0, centralWidget);
  // End

  m_tableModel = new ViewsTableModel(m_sql->db(), this);
  m_tableView->setModel(m_tableModel);

  connect(m_comboBox, SIGNAL(currentTextChanged(QString)), SLOT(createSearchQuery(QString)));
}

void ViewsIndex::setDefaultTableView() {
  const QString sql = AntiquaCRM::ASqlFiles::queryStatement("query_views");
  if (sql.isEmpty())
    return;

  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    m_comboBox->clear();
    int i = 0;
    while (q.next()) {
      m_comboBox->insertItem(i, q.value("comment").toString(), q.value("table_name").toString());
      m_comboBox->setItemData(i, windowIcon(), Qt::DecorationRole);
      i++;
    }
  }
}

void ViewsIndex::createSearchQuery(const QString& title) {
  QString _table = m_comboBox->itemData(m_comboBox->currentIndex()).toString();
  if (_table.isEmpty())
    return;

  m_tableView->setToolTip(title);
  m_tableModel->setTable(_table);
  m_tableModel->select();

  const QString _info = tr("View with %1 rows.").arg(m_tableModel->rowCount());
  m_statusBar->showMessage(_info);
}

void ViewsIndex::openStartPage() {
  setCurrentIndex(ViewPage::MainView);
}

void ViewsIndex::onEnterChanged() {
  if (!initialed) {
    setDefaultTableView();
    initialed = true;
  }
}

const QString ViewsIndex::getTitle() const {
  return tr("Views");
}

bool ViewsIndex::customAction(const QJsonObject&) {
  return false;
}

const QStringList ViewsIndex::acceptsCustomActions() const {
  return QStringList();
}
