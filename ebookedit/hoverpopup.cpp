#include "hoverpopup.h"

const QString HoverPopup::STYLESHEET = ""
                                        "HoverDialog::pane {"
                                        "background-color: lightgrey;"
                                        "}";

HoverPopup::HoverPopup(QWidget* parent)
  : QFrame(parent, Qt::Popup)
{
  initGui();
}

HoverPopup::HoverPopup(QVariant data, QWidget* parent)
  : QFrame(parent, Qt::Popup)
{
  initGui();
  m_data = data.value<EBookData>();
  setLabels();
}

void
HoverPopup::setLabels()
{
  m_titlelbl->setText(m_data.title);
  m_authorlbl->setText(m_data.author_names);
  m_publisherlbl->setText(m_data.publisher);
  QDateTime date = m_data.date;
  QString datestring = date.toString("d MMMM yyyy");
  m_datelbl->setText(datestring);
  m_subjectlbl->setText(m_data.subject);
  QLocale locale(m_data.language);
  m_langlbl->setText(locale.bcp47Name());
  setStyleSheet(STYLESHEET);
}

HoverPopup::~HoverPopup() {}

void
HoverPopup::setData(QVariant& data)
{
  m_data = data.value<EBookData>();
  setLabels();
}

void
HoverPopup::initGui()
{
  setAttribute(Qt::WA_ShowWithoutActivating);

  QGridLayout* l = new QGridLayout;
  setLayout(l);

  QLabel* lbl = new QLabel(tr("Title :"), this);
  l->addWidget(lbl, 0, 0);
  lbl = new QLabel(tr("Author(s) :"), this);
  l->addWidget(lbl, 1, 0);
  lbl = new QLabel(tr("Publisher :"), this);
  l->addWidget(lbl, 2, 0);
  lbl = new QLabel(tr("Date :"), this);
  l->addWidget(lbl, 3, 0);
  lbl = new QLabel(tr("Subject :"), this);
  l->addWidget(lbl, 4, 0);
  lbl = new QLabel(tr("Language :"), this);
  l->addWidget(lbl, 5, 0);

  m_titlelbl = new QLabel(this);
  l->addWidget(m_titlelbl, 0, 1);
  m_authorlbl = new QLabel(this);
  l->addWidget(m_authorlbl, 1, 1);
  m_publisherlbl = new QLabel(this);
  l->addWidget(m_publisherlbl, 2, 1);
  m_datelbl = new QLabel(this);
  l->addWidget(m_datelbl, 3, 1);
  m_subjectlbl = new QLabel(this);
  l->addWidget(m_subjectlbl, 4, 1);
  m_langlbl = new QLabel(this);
  l->addWidget(m_langlbl, 5, 1);
}
