#include "searchtoolbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

SearchToolBar::SearchToolBar(QWidget *parent)
    : QToolBar(parent), minlength(10)
{
  setObjectName("SearchToolBar");
  setWindowTitle(tr("&Search"));
  setOrientation(Qt::Horizontal);
  setAllowedAreas((Qt::TopToolBarArea | Qt::BottomToolBarArea));

  QLabel *label = new QLabel(tr("Title search"), this);
  label->setContentsMargins(5, 0, 5, 0);
  addWidget(label);

  QRegExp pattern("^\\S{2}.+");
  QRegExpValidator* m_regv = new QRegExpValidator(pattern,this);

  m_lineEdit = new QLineEdit(this);
  m_lineEdit->setObjectName("SearchEdit");
  m_lineEdit->setValidator(m_regv);
  connect(m_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputSearchChanged(const QString &)));
  addWidget(m_lineEdit);

  addSeparator();

  QPushButton *clearBtn = new QPushButton(this);
  clearBtn->setIcon(myIcon("clear_left"));
  clearBtn->setToolTip(tr("Clear searchinput"));
  connect(clearBtn, SIGNAL(pressed()), m_lineEdit, SLOT(clear()));
  addWidget(clearBtn);

  addSeparator();

  QPushButton *searchBtn = new QPushButton(tr("Search"), this);
  searchBtn->setObjectName("SearchButton");
  searchBtn->setToolTip(tr("Start search"));
  searchBtn->setContentsMargins(5, 0, 15, 0);
  connect(searchBtn, SIGNAL(released()), this, SLOT(startSearchClicked()));
  addWidget(searchBtn);
  addSeparator();

  m_searchExact = new QCheckBox(tr("Exact search"), this);
  m_searchExact->setObjectName("SearchType");
  m_searchExact->setChecked(true);
  addWidget(m_searchExact);
  addSeparator();
}

void SearchToolBar::inputSearchChanged(const QString &str)
{
  if (str.length() <= minlength)
    return;

  startSearchClicked();
}

void SearchToolBar::startSearchClicked()
{
  bool b = m_searchExact->isChecked();
  if (m_lineEdit->text().length() <= 1)
    return;

  if (b) {
    startSimpleSearch();
  } else {
    startExtendetSearch();
  }
}

const QString SearchToolBar::stripString(const QString &str)
{
  QString buf(str);
  QRegExp reg("[\\'\\`\\´\\\"]+");
  buf = buf.replace(reg, "");
  reg.setPattern("(\\s|\\t)+");
  return buf.replace(reg," ");
}

void SearchToolBar::startSimpleSearch()
{
  QString str = stripString(m_lineEdit->text());
  QRegExp reg("[\\t\\s]+");
  str.replace(reg, " ");
  emit signalSearchQuery(str);
}

void SearchToolBar::startExtendetSearch()
{
  QString str = stripString(m_lineEdit->text());
  QRegExp reg("[\\t\\s]+");
  str.replace(reg, "%");
  str.append("'");
  str.prepend("'");
  emit signalSearchQuery(str);
}
