#include "optionsdialog.h"

const QString OptionsDialog::BTN_STYLE =
  "QPushButton {color: #%1%2%3; background-color: #%4%5%6}";

OptionsDialog::OptionsDialog(Options* options, QWidget* parent) :
  QDialog(parent), m_options(options), m_oldoptions(options),
  m_modified(false), normal_color_btn(Q_NULLPTR), normalBackBtn(Q_NULLPTR),
  normalItalicBox(Q_NULLPTR), normalWeightBox(Q_NULLPTR),
  attColorBtn(Q_NULLPTR), attBackBtn(Q_NULLPTR), attItalicBox(Q_NULLPTR),
  attWeightBox(Q_NULLPTR), tagColorBtn(Q_NULLPTR), tagBackBtn(Q_NULLPTR),
  tagItalicBox(Q_NULLPTR), tagWeightBox(Q_NULLPTR), stringColorBtn(Q_NULLPTR),
  stringBackBtn(Q_NULLPTR), stringItalicBox(Q_NULLPTR),
  stringWeightBox(Q_NULLPTR), errorColorBtn(Q_NULLPTR),
  errorBackBtn(Q_NULLPTR), errorItalicBox(Q_NULLPTR),
  errorWeightBox(Q_NULLPTR), styleColorBtn(Q_NULLPTR),
  styleBackBtn(Q_NULLPTR), styleItalicBox(Q_NULLPTR),
  styleWeightBox(Q_NULLPTR), scriptColorBtn(Q_NULLPTR),
  scriptBackBtn(Q_NULLPTR), scriptItalicBox(Q_NULLPTR),
  scriptWeightBox(Q_NULLPTR)
{
  initGui();

  setGeometry(QRect(QPoint(0, 0), m_options->optionsDlgSize()));
}

OptionsDialog::~OptionsDialog() {}

Options* OptionsDialog::options()
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

QFrame* OptionsDialog::initMiscTab()
{
  QFrame* miscFrame = new QFrame(this);
  QFormLayout* l = new QFormLayout;
  miscFrame->setLayout(l);

//  QCheckBox* enablePopupBox = new QCheckBox(this);
//  enablePopupBox->setChecked(m_options->enablepopup);
//  connect(enablePopupBox, &QCheckBox::clicked, this,
//          &OptionsDialog::changeEnablePopup);
//  l->addRow(tr("Enable information popup's"), enablePopupBox);

//  QSpinBox* popupTimeoutBox = new QSpinBox(this);
//  popupTimeoutBox->setValue(m_options->popuptimeout / 1000);
//  connect(popupTimeoutBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
//          &OptionsDialog::changePopupTimeout);
//  l->addRow(tr("Information popup timeout (Secs)"), popupTimeoutBox);

  return miscFrame;
}

QFrame* OptionsDialog::initEditorTab()
{
  QFrame* f = new QFrame(this);
  QFormLayout* l = new QFormLayout;
  f->setLayout(l);

  copy_book_to_store_box = new QCheckBox(this);
  copy_book_to_store_box->setChecked(m_options->copyBooksToStore());
  copy_book_to_store_box->setToolTip(
    tr("Copy the book to the book store before"
       "you start to edit it. This allows the "
       "original copy to be retained unmodified."));
  connect(copy_book_to_store_box, &QCheckBox::clicked, this,
          &OptionsDialog::setCopyBookToStore);
  l->addRow(tr("Copy Books to Book Store Before Editing"),
            copy_book_to_store_box);

  delete_old_book_box = new QCheckBox(this);
  if (!m_options->copyBooksToStore())
    delete_old_book_box->setEnabled(false);
  else
    delete_old_book_box->setEnabled(true);
  delete_old_book_box->setChecked(m_options->deleteOldBook());
  delete_old_book_box->setToolTip(tr("Delete the original copy of the book "
                                     "when the book has been copied to the "
                                     "book store."));
  connect(delete_old_book_box, &QCheckBox::clicked, this,
          &OptionsDialog::setDeleteOldBook);
  l->addRow(tr("Delete the old copy of the stored book"), delete_old_book_box);

  toc_visible_box = new QCheckBox(this);
  toc_visible_box->setChecked(m_options->tocVisible());
  l->addRow(tr("Show Table of Contents"), toc_visible_box);
  connect(toc_visible_box, &QCheckBox::clicked, this,
          &OptionsDialog::setShowTocEdit);

  toc_position_btn = new QPushButton(this);
  if (m_options->tocPosition() == Options::LEFT) {
    toc_position_btn->setText(tr("Move TOC to Right"));
  } else {
    toc_position_btn->setText(tr("Move TOC to Left"));
  }
  connect(toc_position_btn, &QPushButton::clicked, this,
          &OptionsDialog::setTocPosition);

  never_confirm_delete_box = new QCheckBox(this);
  if (!m_options->copyBooksToStore())
    never_confirm_delete_box->setEnabled(false);
  else
    never_confirm_delete_box->setEnabled(true);
  never_confirm_delete_box->setChecked(m_options->neverConfirmDelete());
  never_confirm_delete_box->setToolTip(
    tr("Delete the original copy of the book "
       "when the book has been copied to the "
       "book store."));
  connect(never_confirm_delete_box, &QCheckBox::clicked, this,
          &OptionsDialog::setNeverConfirmDelete);
  l->addRow(tr("Never Confirm Deleteion of Old Book"),
            never_confirm_delete_box);

  return f;
}

void OptionsDialog::setColorConnection(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setNormalColor);
    break;
  }
  case Options::TAG: {
    connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setAttributeColor);
    break;
  }
  case Options::ATTRIBUTE: {
    connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setTagColor);
    break;
  }
  case Options::ERROR: {
    connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setErrorColor);
    break;
  }
  case Options::STRING: {
    connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setStringColor);
    break;
  }
  case Options::SCRIPT: {
    connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setScriptColor);
    break;
  }
  case Options::STYLE: {
  }
  connect(this->getColorBtn(options), &QAbstractButton::clicked, this,
          &OptionsDialog::setStyleColor);
  break;
  }
}

void OptionsDialog::setBackgroundConnection(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setNormalBackground);
    break;
  }
  case Options::TAG: {
    connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setAttributeBackground);
    break;
  }
  case Options::ATTRIBUTE: {
    connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setTagBackground);
    break;
  }
  case Options::ERROR: {
    connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setErrorBackground);
    break;
  }
  case Options::STRING: {
    connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setStringBackground);
    break;
  }
  case Options::SCRIPT: {
    connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setScriptBackground);
    break;
  }
  case Options::STYLE: {
  }
  connect(this->getBackgroundBtn(options), &QAbstractButton::clicked, this,
          &OptionsDialog::setStyleBackground);
  break;
  }
}

void OptionsDialog::setItalicConnection(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setNormalItalic);
    break;
  }
  case Options::TAG: {
    connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setAttributeItalic);
    break;
  }
  case Options::ATTRIBUTE: {
    connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setTagItalic);
    break;
  }
  case Options::ERROR: {
    connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setErrorItalic);
    break;
  }
  case Options::STRING: {
    connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setStringItalic);
    break;
  }
  case Options::SCRIPT: {
    connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
            &OptionsDialog::setScriptItalic);
    break;
  }
  case Options::STYLE: {
  }
  connect(this->getItalicBox(options), &QAbstractButton::clicked, this,
          &OptionsDialog::setStyleItalic);
  break;
  }
}

void OptionsDialog::setWeightConnection(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    connect(this->getWeightBox(options),
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &OptionsDialog::setNormalWeight);
    break;
  }
  case Options::TAG: {
    connect(this->getWeightBox(options),
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &OptionsDialog::setAttributeWeight);
    break;
  }
  case Options::ATTRIBUTE: {
    connect(this->getWeightBox(options),
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &OptionsDialog::setTagWeight);
    break;
  }
  case Options::ERROR: {
    connect(this->getWeightBox(options),
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &OptionsDialog::setErrorWeight);
    break;
  }
  case Options::STRING: {
    connect(this->getWeightBox(options),
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &OptionsDialog::setStringWeight);
    break;
  }
  case Options::SCRIPT: {
    connect(this->getWeightBox(options),
            QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &OptionsDialog::setScriptWeight);
    break;
  }
  case Options::STYLE: {
  }
  connect(this->getWeightBox(options),
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OptionsDialog::setStyleWeight);
  break;
  }
}

QPushButton* OptionsDialog::getColorBtn(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    if (!normal_color_btn) {
      normal_color_btn = new QPushButton(this);
    }
    return normal_color_btn;
  }
  case Options::TAG: {
    if (!tagColorBtn) {
      tagColorBtn = new QPushButton(this);
    }
    return tagColorBtn;
  }
  case Options::ATTRIBUTE: {
    if (!attColorBtn) {
      attColorBtn = new QPushButton(this);
    }
    return attColorBtn;
  }
  case Options::ERROR: {
    if (!errorColorBtn) {
      errorColorBtn = new QPushButton(this);
    }
    return errorColorBtn;
  }
  case Options::STRING: {
    if (!stringColorBtn) {
      stringColorBtn = new QPushButton(this);
    }
    return stringColorBtn;
  }
  case Options::SCRIPT: {
    if (!scriptColorBtn) {
      scriptColorBtn = new QPushButton(this);
    }
    return scriptColorBtn;
  }
  case Options::STYLE: {
    if (!styleColorBtn) {
      styleColorBtn = new QPushButton(this);
    }
    return styleColorBtn;
  }
  }
  return Q_NULLPTR;
}

QPushButton* OptionsDialog::getBackgroundBtn(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    if (!normalBackBtn) {
      normalBackBtn = new QPushButton(this);
    }
    return normalBackBtn;
  }
  case Options::TAG: {
    if (!tagBackBtn) {
      tagBackBtn = new QPushButton(this);
    }
    return tagBackBtn;
  }
  case Options::ATTRIBUTE: {
    if (!attBackBtn) {
      attBackBtn = new QPushButton(this);
    }
    return attBackBtn;
  }
  case Options::ERROR: {
    if (!errorBackBtn) {
      errorBackBtn = new QPushButton(this);
    }
    return errorBackBtn;
  }
  case Options::STRING: {
    if (!stringBackBtn) {
      stringBackBtn = new QPushButton(this);
    }
    return stringBackBtn;
  }
  case Options::SCRIPT: {
    if (!scriptBackBtn) {
      scriptBackBtn = new QPushButton(this);
    }
    return scriptBackBtn;
  }
  case Options::STYLE: {
    if (!styleBackBtn) {
      styleBackBtn = new QPushButton(this);
    }
    return styleBackBtn;
  }
  }
  return Q_NULLPTR;
}

QCheckBox* OptionsDialog::getItalicBox(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    if (!normalItalicBox) {
      normalItalicBox = new QCheckBox(this);
    }
    return normalItalicBox;
  }
  case Options::TAG: {
    if (!tagItalicBox) {
      tagItalicBox = new QCheckBox(this);
    }
    return tagItalicBox;
  }
  case Options::ATTRIBUTE: {
    if (!attItalicBox) {
      attItalicBox = new QCheckBox(this);
    }
    return attItalicBox;
  }
  case Options::ERROR: {
    if (!errorItalicBox) {
      errorItalicBox = new QCheckBox(this);
    }
    return errorItalicBox;
  }
  case Options::STRING: {
    if (!stringItalicBox) {
      stringItalicBox = new QCheckBox(this);
    }
    return stringItalicBox;
  }
  case Options::SCRIPT: {
    if (!scriptItalicBox) {
      scriptItalicBox = new QCheckBox(this);
    }
    return scriptItalicBox;
  }
  case Options::STYLE: {
    if (!styleItalicBox) {
      styleItalicBox = new QCheckBox(this);
    }
    return styleItalicBox;
  }
  }
  return Q_NULLPTR;
}

QComboBox* OptionsDialog::getWeightBox(Options::CodeOptions options)
{
  switch (options) {
  case Options::NORMAL: {
    if (!normalWeightBox) {
      normalWeightBox = new QComboBox(this);
    }
    return normalWeightBox;
  }
  case Options::TAG: {
    if (!tagWeightBox) {
      tagWeightBox = new QComboBox(this);
    }
    return tagWeightBox;
  }
  case Options::ATTRIBUTE: {
    if (!attWeightBox) {
      attWeightBox = new QComboBox(this);
    }
    return attWeightBox;
  }
  case Options::ERROR: {
    if (!errorWeightBox) {
      errorWeightBox = new QComboBox(this);
    }
    return errorWeightBox;
  }
  case Options::STRING: {
    if (!stringWeightBox) {
      stringWeightBox = new QComboBox(this);
    }
    return stringWeightBox;
  }
  case Options::SCRIPT: {
    if (!scriptWeightBox) {
      scriptWeightBox = new QComboBox(this);
    }
    return scriptWeightBox;
  }
  case Options::STYLE: {
    if (!styleWeightBox) {
      styleWeightBox = new QComboBox(this);
    }
    return styleWeightBox;
  }
  }
  return Q_NULLPTR;
}

QString OptionsDialog::convertColorToStyle(QColor color, QColor back)
{
  QString style, rs1, gs1, bs1, rs2, gs2, bs2;
  rs1 = QString("%1").arg(color.red(), 2, 16, QChar('0'));
  gs1 = QString("%1").arg(color.green(), 2, 16, QChar('0'));
  bs1 = QString("%1").arg(color.blue(), 2, 16, QChar('0'));
  rs2 = QString("%1").arg(back.red(), 2, 16, QChar('0'));
  gs2 = QString("%1").arg(back.green(), 2, 16, QChar('0'));
  bs2 = QString("%1").arg(back.blue(), 2, 16, QChar('0'));
  style =
    QString(BTN_STYLE).arg(rs1).arg(gs1).arg(bs1).arg(rs2).arg(gs2).arg(bs2);
  return style;
}

QFrame* OptionsDialog::initCodeTab(Options::CodeOptions options)
{
  QFrame* frm;
  QFormLayout* form;
  QStringList weights = {"Thin",   "ExtraLight", "Light",
                         "Normal", "Medium",     "DemiBold",
                         "Bold",   "ExtraBold",  "Black"
                        };
  //  QPalette pal;
  QComboBox* combo = getWeightBox(options);
  QCheckBox* box = getItalicBox(options);
  QColor color = m_options->color(options);
  QColor back = m_options->background(options);
  bool italic = m_options->italic(options);
  QFont::Weight weight = m_options->weight(options);
  QString name = Options::codeOptionToString(options);
  QPushButton* color_btn = getColorBtn(options);
  QPushButton* back_btn = getBackgroundBtn(options);
  QString style;

  frm = new QFrame(this);
  form = new QFormLayout;
  //  l->addWidget(frm);
  frm->setLayout(form);

  color_btn->setText(tr("Change %1 Color").arg(name));
  style = convertColorToStyle(color, back);
  color_btn->setStyleSheet(style);
  form->addRow(tr("%1 Text Color").arg(name), color_btn);
  setColorConnection(options);

  back_btn->setText(tr("Change %1 Background").arg(name));
  //  style = convertColorToStyle(color, back);
  back_btn->setStyleSheet(style);
  form->addRow(tr("%1 Background Color").arg(name), back_btn);
  setBackgroundConnection(options);

  combo->addItems(weights);
  combo->setCurrentText(m_options->weightToString(weight));
  form->addRow(tr("%1 Font Weight").arg(name), combo);
  setWeightConnection(options);

  box->setChecked(italic);
  form->addRow(tr("%1 Font Italic").arg(name), box);
  setItalicConnection(options);

  return frm;
}

QFrame* OptionsDialog::initCodeEditorTab()
{
  QFrame* f = new QFrame(this);
  QGridLayout* l = new QGridLayout;
  f->setLayout(l);

  QTabWidget* tabs = new QTabWidget(this);
  l->addWidget(tabs);

  QFrame* frm = new QFrame(this);
  QFormLayout* form = new QFormLayout;
  l->addWidget(frm);
  frm->setLayout(form);

  font_btn = new QPushButton(tr("Change Font"), this);
  font_btn->setFont(m_options->codeFont());
  form->addRow(tr("Font"), font_btn);
  connect(font_btn, &QAbstractButton::clicked, this,
          &OptionsDialog::setCodeFont);
  tabs->addTab(frm, tr("Font"));

  frm = initCodeTab(Options::NORMAL);
  tabs->addTab(frm, Options::codeOptionToString(Options::NORMAL));
  frm = initCodeTab(Options::ATTRIBUTE);
  tabs->addTab(frm, Options::codeOptionToString(Options::ATTRIBUTE));
  frm = initCodeTab(Options::TAG);
  tabs->addTab(frm, Options::codeOptionToString(Options::TAG));
  frm = initCodeTab(Options::STRING);
  tabs->addTab(frm, Options::codeOptionToString(Options::STRING));
  frm = initCodeTab(Options::ERROR);
  tabs->addTab(frm, Options::codeOptionToString(Options::ERROR));
  frm = initCodeTab(Options::STYLE);
  tabs->addTab(frm, Options::codeOptionToString(Options::STYLE));
  frm = initCodeTab(Options::SCRIPT);
  tabs->addTab(frm, Options::codeOptionToString(Options::SCRIPT));

  return f;
}

void OptionsDialog::setCodeFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, m_options->codeFont(), this);
  if (ok) {
    m_options->setCodeFont(font);
    emit codeChanged();
    font_btn->setFont(font);
    m_modified = true;
  }
}

void OptionsDialog::setCodeColor(Options::CodeOptions options)
{
  QColor color = enactColorDialog(m_options->color(options));
  m_options->setColor(options, color);
  QPushButton* btn = getColorBtn(options);
  QString style = convertColorToStyle(color, m_options->background(options));
  btn->setStyleSheet(style);
  btn = getBackgroundBtn(options);
  btn->setStyleSheet(style);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setNormalColor()
{
  setCodeColor(Options::NORMAL);
}

void OptionsDialog::setCodeBackground(Options::CodeOptions options)
{
  QColor back = enactColorDialog(m_options->background(options));
  m_options->setBackground(options, back);
  QPushButton* btn = getBackgroundBtn(options);
  QString style = convertColorToStyle(m_options->color(options), back);
  btn->setStyleSheet(style);
  btn = getColorBtn(options);
  btn->setStyleSheet(style);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setNormalBackground()
{
  setCodeBackground(Options::NORMAL);
}

void OptionsDialog::setNormalItalic(bool state)
{
  m_options->setItalic(Options::NORMAL, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setNormalWeight(int index)
{
  m_options->setWeight(Options::NORMAL, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setAttributeColor()
{
  setCodeColor(Options::ATTRIBUTE);
}

void OptionsDialog::setAttributeBackground()
{
  setCodeBackground(Options::ATTRIBUTE);
}

void OptionsDialog::setAttributeItalic(bool state)
{
  m_options->setItalic(Options::ATTRIBUTE, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setAttributeWeight(int index)
{
  m_options->setWeight(Options::ATTRIBUTE, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setTagColor()
{
  setCodeColor(Options::TAG);
}

void OptionsDialog::setTagBackground()
{
  setCodeBackground(Options::TAG);
}

void OptionsDialog::setTagItalic(bool state)
{
  m_options->setItalic(Options::TAG, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setTagWeight(int index)
{
  m_options->setWeight(Options::TAG, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setStringColor()
{
  setCodeColor(Options::STRING);
}

void OptionsDialog::setStringBackground()
{
  setCodeBackground(Options::TAG);
}

void OptionsDialog::setStringItalic(bool state)
{
  m_options->setItalic(Options::STRING, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setStringWeight(int index)
{
  m_options->setWeight(Options::STRING, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setErrorColor()
{
  setCodeColor(Options::ERROR);
}

void OptionsDialog::setErrorBackground()
{
  setCodeBackground(Options::ERROR);
}

void OptionsDialog::setErrorItalic(bool state)
{
  m_options->setItalic(Options::ERROR, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setErrorWeight(int index)
{
  m_options->setWeight(Options::ERROR, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setStyleColor()
{
  setCodeColor(Options::STYLE);
}

void OptionsDialog::setStyleBackground()
{
  setCodeBackground(Options::STYLE);
}

void OptionsDialog::setStyleItalic(bool state)
{
  m_options->setItalic(Options::STYLE, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setStyleWeight(int index)
{
  m_options->setWeight(Options::STYLE, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setScriptColor()
{
  setCodeColor(Options::SCRIPT);
}

void OptionsDialog::setScriptBackground()
{
  setCodeBackground(Options::SCRIPT);
}

void OptionsDialog::setScriptItalic(bool state)
{
  m_options->setItalic(Options::SCRIPT, state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setScriptWeight(int index)
{
  m_options->setWeight(Options::SCRIPT, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setCopyBookToStore(bool state)
{
  m_options->setCopyBooksToStore(state);
  emit codeChanged();
  m_modified = true;
  if (state)
    delete_old_book_box->setEnabled(true);
  else
    delete_old_book_box->setEnabled(false);
}

void OptionsDialog::setDeleteOldBook(bool state)
{
  m_options->setDeleteOldBook(state);
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setNeverConfirmDelete(bool state)
{
  m_options->setNeverConfirmDelete(state) ;
  emit codeChanged();
  m_modified = true;
}

void OptionsDialog::setShowTocEdit(bool state)
{
  m_options->setTocVisible(state);
  emit codeChanged();
  emit showToc(state);
  m_modified = true;
}

void OptionsDialog::setTocPosition()
{
  if (m_options->tocPosition() == Options::LEFT) {
    m_options->setTocPosition(Options::RIGHT);
    emit moveToc(Options::RIGHT);
    m_modified = true;
  } else {
    m_options->setTocPosition(Options::LEFT);
    emit moveToc(Options::LEFT);
    m_modified = true;
  }
}

void OptionsDialog::initGui()
{
  QGridLayout* l = new QGridLayout;
  setLayout(l);

  QTabWidget* tabs = new QTabWidget(this);
  l->addWidget(tabs, 0, 0);

  QFrame* BoxFrame = new QFrame(this);
  QHBoxLayout* BoxLayout = new QHBoxLayout;
  BoxFrame->setLayout(BoxLayout);
  l->addWidget(BoxFrame, 1, 0);

  QPushButton* cancelBox = new QPushButton(tr("Cancel"), this);
  connect(cancelBox, &QPushButton::clicked, this, &QDialog::reject);
  BoxLayout->addWidget(cancelBox);

  QPushButton* acceptBox = new QPushButton(tr("Accept"), this);
  connect(acceptBox, &QPushButton::clicked, this, &QDialog::accept);
  BoxLayout->addWidget(acceptBox);

  QFrame* editorFrame = initEditorTab();
  tabs->addTab(editorFrame, tr("Editor/Reader Options"));
  QFrame* codeFrame = initCodeEditorTab();
  tabs->addTab(codeFrame, tr("Code Editor Options"));
  QFrame* miscFrame = initMiscTab();
  tabs->addTab(miscFrame, tr("Miscellaneous Options"));
}

void OptionsDialog::resizeEvent(QResizeEvent* e)
{
  QSize size = e->size();
  m_options->setOptionsDlgSize(size);
  m_modified = true;
}

QColor OptionsDialog::enactColorDialog(QColor old_color)
{
  QColor color =
    QColorDialog::getColor(old_color, this, tr("Select Normal Color"));
  if (color.isValid()) {
    emit codeChanged();
    m_modified = true;
    return color;
  }
  return old_color;
}
