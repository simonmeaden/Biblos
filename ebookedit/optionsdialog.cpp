#include "optionsdialog.h"

OptionsDialog::OptionsDialog(Options options, QWidget* parent)
  : QDialog(parent)
  , m_options(options)
  , m_oldoptions(options)
  , m_modified(false)
{
  initGui();
}

OptionsDialog::~OptionsDialog() {}

Options
OptionsDialog::options()
{
  if (result()) // Accepted
    return m_options;
  else // Rejected
    return m_oldoptions;
}

bool OptionsDialog::modified() const
{
  return m_modified;
}

void
OptionsDialog::initGui()
{
  QGridLayout* l = new QGridLayout;
  setLayout(l);

  QTabWidget* tabs = new QTabWidget(this);
  l->addWidget(tabs, 0, 0);

  QFrame *btnFrame = new QFrame(this);
  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnFrame->setLayout(btnLayout);
  l->addWidget(btnFrame, 1, 0);

  QPushButton* cancelBtn = new QPushButton(tr("Cancel"), this);
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
  btnLayout->addWidget(cancelBtn);

  QPushButton* acceptBtn = new QPushButton(tr("Accept"), this);
  connect(acceptBtn, &QPushButton::clicked, this, &QDialog::accept);
  btnLayout->addWidget(acceptBtn);

  QFrame* miscFrame = new QFrame(this);
  QFormLayout* miscLayout = new QFormLayout;
  miscFrame->setLayout(miscLayout);

  QCheckBox* enablePopupBtn = new QCheckBox(this);
  enablePopupBtn->setChecked(m_options.enablepopup);
  connect(enablePopupBtn,
          &QCheckBox::clicked,
          this,
          &OptionsDialog::changeEnablePopup);
  miscLayout->addRow(tr("Enable information popup's"), enablePopupBtn);

  QSpinBox* popupTimeoutBox = new QSpinBox(this);
  popupTimeoutBox->setValue(m_options.popuptimeout / 1000);
  connect(popupTimeoutBox,
          qOverload<int>(&QSpinBox::valueChanged),
          this,
          &OptionsDialog::changePopupTimeout);
  miscLayout->addRow(tr("Information popup timeout (Secs)"), popupTimeoutBox);

  tabs->addTab(miscFrame, tr("Miscellaneous"));
}

void
OptionsDialog::changePopupTimeout(int value)
{
  if (m_options.popuptimeout != value) {
    m_options.popuptimeout = value * 1000;
    m_modified = true;
  }
}

void
OptionsDialog::changeEnablePopup(bool value)
{
  if (m_options.enablepopup != value) {
    m_options.enablepopup = value;
    m_modified = true;
  }
}
