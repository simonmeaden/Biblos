#include "aboutdialog.h"

//#include <qlogger/qlogger.h>
// using namespace qlogger;

#include <csvsplitter/csvsplitter.h>

#include "logging.h"

AboutDialog::AboutDialog(QWidget* parent)
  : QDialog(parent)
{

  setWindowTitle(tr("About EbookEditor"));

  QGridLayout* main_layout = new QGridLayout;
  setLayout(main_layout);

  m_tabs = new QTabWidget(this);
  main_layout->addWidget(m_tabs);

  QFrame* about_frame = new QFrame(this);
  QGridLayout* about_layout = new QGridLayout;
  about_frame->setLayout(about_layout);
  QLabel* copyright =
    new QLabel(tr("Copyright 2018-2019 Simon Meaden. All Rights Reserved."));
  about_layout->addWidget(copyright, 0, 0);
  m_tabs->addTab(about_frame, tr("About EbookEditor"));

  QFrame* attrib_frame = new QFrame(this);
  QGridLayout* attrib_layout = new QGridLayout;
  attrib_frame->setLayout(attrib_layout);
  m_attribution = new QTableWidget(this);
  m_attribution->setColumnCount(4);
  attrib_layout->addWidget(m_attribution, 0, 0);
  m_tabs->addTab(attrib_frame, tr("Attributions"));

  QPushButton* quit_Btn = new QPushButton(tr("Close Dialog"), this);
  connect(quit_Btn, &QPushButton::clicked, this, &QDialog::close);
  main_layout->addWidget(quit_Btn, 1, 0);

  QStringList headers;
  headers << tr("Supplied") << tr("Creator") << tr("Vendor") << tr("License");
  m_attribution->setHorizontalHeaderLabels(headers);
  m_attribution->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  m_attribution->horizontalHeader()->setStretchLastSection(true);

  QString fileName(":/files/attributions");
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly)) {
    qCDebug(LOG_ABOUT_DIALOG()) << tr("Attributions file not opened.");
  } else {
    QString data = file.readLine();
    QStringList list;
    int row = 0;
    //    QString supplied, text, textlink, vendor, vendorlink, license,
    //    licenselink;
    while (!data.isEmpty()) {
      m_attribution->insertRow(row);
      list = CSVSplitter::splitOnComma(data);
      if (list.length() < 7) {
        data = file.readLine();
        continue;
      }
      QLabel* supplied = new QLabel(list.at(0), this);
      QString t = "<a href=\"%1\">%2</a>";
      QString t2 = t.arg(list.at(2)).arg(list.at(1));
      QLabel* text = new QLabel(t2, this);
      text->setOpenExternalLinks(true);
      t2 = t.arg(list.at(4)).arg(list.at(3));
      QLabel* vendor = new QLabel(t2, this);
      vendor->setOpenExternalLinks(true);
      t2 = t.arg(list.at(6)).arg(list.at(5));
      QLabel* license = new QLabel(t2, this);
      license->setOpenExternalLinks(true);

      m_attribution->setCellWidget(row, 0, supplied);
      m_attribution->setCellWidget(row, 1, text);
      m_attribution->setCellWidget(row, 2, vendor);
      m_attribution->setCellWidget(row, 3, license);
      data = file.readLine();
      row++;
    }
  }

  file.close();
  int x = parent->x() + (parent->width() / 2) - 450;
  int y = parent->y() + (parent->height() / 2) - 150;
  setGeometry(x, y, 900, 300);
  update();
}

AboutDialog::~AboutDialog() {}
