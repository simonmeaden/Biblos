#include "optionsdialog.h"

const QString OptionsDialog::BTN_STYLE =
  "QPushButton {color: #%1%2%3; background-color: #%4%5%6}";

OptionsDialog::OptionsDialog(Options options, QWidget* parent)
  : QDialog(parent)
  , m_options(options)
  , m_oldoptions(Options(options.data()))
  , m_modified(false)
  , normal_color_btn(nullptr)
  , normalBackBtn(nullptr)
  , normalItalicBox(nullptr)
  , normalWeightBox(nullptr)
  , attColorBtn(nullptr)
  , attBackBtn(nullptr)
  , attItalicBox(nullptr)
  , attWeightBox(nullptr)
  , tagColorBtn(nullptr)
  , tagBackBtn(nullptr)
  , tagItalicBox(nullptr)
  , tagWeightBox(nullptr)
  , stringColorBtn(nullptr)
  , stringBackBtn(nullptr)
  , stringItalicBox(nullptr)
  , stringWeightBox(nullptr)
  , errorColorBtn(nullptr)
  , errorBackBtn(nullptr)
  , errorItalicBox(nullptr)
  , errorWeightBox(nullptr)
  , styleColorBtn(nullptr)
  , styleBackBtn(nullptr)
  , styleItalicBox(nullptr)
  , styleWeightBox(nullptr)
  , scriptColorBtn(nullptr)
  , scriptBackBtn(nullptr)
  , scriptItalicBox(nullptr)
  , scriptWeightBox(nullptr)
{
  initGui();

  setGeometry(QRect(QPoint(0, 0), m_options->optionsDlgSize()));
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

bool
OptionsDialog::modified() const
{
  return m_modified;
}

QFrame*
OptionsDialog::initMiscTab()
{
  QFrame* miscFrame = new QFrame(this);
  QFormLayout* l = new QFormLayout;
  miscFrame->setLayout(l);

  return miscFrame;
}

QFrame*
OptionsDialog::initEditorTab()
{
  QFrame* f = new QFrame(this);
  QFormLayout* l = new QFormLayout;
  f->setLayout(l);

  toc_visible_box = new QCheckBox(this);
  toc_visible_box->setChecked(m_options->tocVisible());
  l->addRow(tr("Show Table of Contents"), toc_visible_box);
  connect(
    toc_visible_box, &QCheckBox::clicked, this, &OptionsDialog::setShowTocEdit);

  toc_position_btn = new QPushButton(this);
  if (m_options->tocPosition() == BiblosOptions::LEFT) {
    toc_position_btn->setText(tr("Move TOC to Right"));
  } else {
    toc_position_btn->setText(tr("Move TOC to Left"));
  }
  connect(toc_position_btn,
          &QPushButton::clicked,
          this,
          &OptionsDialog::setTocPosition);

  return f;
}

void
OptionsDialog::setColorConnection(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setNormalColor);
      break;
    }
    case BiblosOptions::TAG: {
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setAttributeColor);
      break;
    }
    case BiblosOptions::ATTRIBUTE: {
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setTagColor);
      break;
    }
    case BiblosOptions::ERROR: {
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setErrorColor);
      break;
    }
    case BiblosOptions::STRING: {
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setStringColor);
      break;
    }
    case BiblosOptions::SCRIPT: {
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setScriptColor);
      break;
    }
    case BiblosOptions::STYLE: {
    }
      connect(this->getColorBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setStyleColor);
      break;
  }
}

void
OptionsDialog::setBackgroundConnection(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setNormalBackground);
      break;
    }
    case BiblosOptions::TAG: {
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setAttributeBackground);
      break;
    }
    case BiblosOptions::ATTRIBUTE: {
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setTagBackground);
      break;
    }
    case BiblosOptions::ERROR: {
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setErrorBackground);
      break;
    }
    case BiblosOptions::STRING: {
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setStringBackground);
      break;
    }
    case BiblosOptions::SCRIPT: {
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setScriptBackground);
      break;
    }
    case BiblosOptions::STYLE: {
    }
      connect(this->getBackgroundBtn(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setStyleBackground);
      break;
  }
}

void
OptionsDialog::setItalicConnection(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setNormalItalic);
      break;
    }
    case BiblosOptions::TAG: {
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setAttributeItalic);
      break;
    }
    case BiblosOptions::ATTRIBUTE: {
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setTagItalic);
      break;
    }
    case BiblosOptions::ERROR: {
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setErrorItalic);
      break;
    }
    case BiblosOptions::STRING: {
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setStringItalic);
      break;
    }
    case BiblosOptions::SCRIPT: {
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setScriptItalic);
      break;
    }
    case BiblosOptions::STYLE: {
    }
      connect(this->getItalicBox(options),
              &QAbstractButton::clicked,
              this,
              &OptionsDialog::setStyleItalic);
      break;
  }
}

void
OptionsDialog::setWeightConnection(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setNormalWeight);
      break;
    }
    case BiblosOptions::TAG: {
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setAttributeWeight);
      break;
    }
    case BiblosOptions::ATTRIBUTE: {
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setTagWeight);
      break;
    }
    case BiblosOptions::ERROR: {
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setErrorWeight);
      break;
    }
    case BiblosOptions::STRING: {
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setStringWeight);
      break;
    }
    case BiblosOptions::SCRIPT: {
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setScriptWeight);
      break;
    }
    case BiblosOptions::STYLE: {
    }
      connect(this->getWeightBox(options),
              QOverload<int>::of(&QComboBox::currentIndexChanged),
              this,
              &OptionsDialog::setStyleWeight);
      break;
  }
}

QPushButton*
OptionsDialog::getColorBtn(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      if (!normal_color_btn) {
        normal_color_btn = new QPushButton(this);
      }
      return normal_color_btn;
    }
    case BiblosOptions::TAG: {
      if (!tagColorBtn) {
        tagColorBtn = new QPushButton(this);
      }
      return tagColorBtn;
    }
    case BiblosOptions::ATTRIBUTE: {
      if (!attColorBtn) {
        attColorBtn = new QPushButton(this);
      }
      return attColorBtn;
    }
    case BiblosOptions::ERROR: {
      if (!errorColorBtn) {
        errorColorBtn = new QPushButton(this);
      }
      return errorColorBtn;
    }
    case BiblosOptions::STRING: {
      if (!stringColorBtn) {
        stringColorBtn = new QPushButton(this);
      }
      return stringColorBtn;
    }
    case BiblosOptions::SCRIPT: {
      if (!scriptColorBtn) {
        scriptColorBtn = new QPushButton(this);
      }
      return scriptColorBtn;
    }
    case BiblosOptions::STYLE: {
      if (!styleColorBtn) {
        styleColorBtn = new QPushButton(this);
      }
      return styleColorBtn;
    }
  }
  return nullptr;
}

QPushButton*
OptionsDialog::getBackgroundBtn(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      if (!normalBackBtn) {
        normalBackBtn = new QPushButton(this);
      }
      return normalBackBtn;
    }
    case BiblosOptions::TAG: {
      if (!tagBackBtn) {
        tagBackBtn = new QPushButton(this);
      }
      return tagBackBtn;
    }
    case BiblosOptions::ATTRIBUTE: {
      if (!attBackBtn) {
        attBackBtn = new QPushButton(this);
      }
      return attBackBtn;
    }
    case BiblosOptions::ERROR: {
      if (!errorBackBtn) {
        errorBackBtn = new QPushButton(this);
      }
      return errorBackBtn;
    }
    case BiblosOptions::STRING: {
      if (!stringBackBtn) {
        stringBackBtn = new QPushButton(this);
      }
      return stringBackBtn;
    }
    case BiblosOptions::SCRIPT: {
      if (!scriptBackBtn) {
        scriptBackBtn = new QPushButton(this);
      }
      return scriptBackBtn;
    }
    case BiblosOptions::STYLE: {
      if (!styleBackBtn) {
        styleBackBtn = new QPushButton(this);
      }
      return styleBackBtn;
    }
  }
  return nullptr;
}

QCheckBox*
OptionsDialog::getItalicBox(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      if (!normalItalicBox) {
        normalItalicBox = new QCheckBox(this);
      }
      return normalItalicBox;
    }
    case BiblosOptions::TAG: {
      if (!tagItalicBox) {
        tagItalicBox = new QCheckBox(this);
      }
      return tagItalicBox;
    }
    case BiblosOptions::ATTRIBUTE: {
      if (!attItalicBox) {
        attItalicBox = new QCheckBox(this);
      }
      return attItalicBox;
    }
    case BiblosOptions::ERROR: {
      if (!errorItalicBox) {
        errorItalicBox = new QCheckBox(this);
      }
      return errorItalicBox;
    }
    case BiblosOptions::STRING: {
      if (!stringItalicBox) {
        stringItalicBox = new QCheckBox(this);
      }
      return stringItalicBox;
    }
    case BiblosOptions::SCRIPT: {
      if (!scriptItalicBox) {
        scriptItalicBox = new QCheckBox(this);
      }
      return scriptItalicBox;
    }
    case BiblosOptions::STYLE: {
      if (!styleItalicBox) {
        styleItalicBox = new QCheckBox(this);
      }
      return styleItalicBox;
    }
  }
  return nullptr;
}

QComboBox*
OptionsDialog::getWeightBox(BiblosOptions::CodeOptions options)
{
  switch (options) {
    case BiblosOptions::NORMAL: {
      if (!normalWeightBox) {
        normalWeightBox = new QComboBox(this);
      }
      return normalWeightBox;
    }
    case BiblosOptions::TAG: {
      if (!tagWeightBox) {
        tagWeightBox = new QComboBox(this);
      }
      return tagWeightBox;
    }
    case BiblosOptions::ATTRIBUTE: {
      if (!attWeightBox) {
        attWeightBox = new QComboBox(this);
      }
      return attWeightBox;
    }
    case BiblosOptions::ERROR: {
      if (!errorWeightBox) {
        errorWeightBox = new QComboBox(this);
      }
      return errorWeightBox;
    }
    case BiblosOptions::STRING: {
      if (!stringWeightBox) {
        stringWeightBox = new QComboBox(this);
      }
      return stringWeightBox;
    }
    case BiblosOptions::SCRIPT: {
      if (!scriptWeightBox) {
        scriptWeightBox = new QComboBox(this);
      }
      return scriptWeightBox;
    }
    case BiblosOptions::STYLE: {
      if (!styleWeightBox) {
        styleWeightBox = new QComboBox(this);
      }
      return styleWeightBox;
    }
  }
  return nullptr;
}

QString
OptionsDialog::convertColorToStyle(QColor color, QColor back)
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

QFrame*
OptionsDialog::initCodeTab(BiblosOptions::CodeOptions options)
{
  QFrame* frm;
  QFormLayout* form;
  QStringList weights = { "Thin",   "ExtraLight", "Light",
                          "Normal", "Medium",     "DemiBold",
                          "Bold",   "ExtraBold",  "Black" };
  //  QPalette pal;
  QComboBox* combo = getWeightBox(options);
  QCheckBox* box = getItalicBox(options);
  QColor color = m_options->color(options);
  QColor back = m_options->background(options);
  bool italic = m_options->italic(options);
  QFont::Weight weight = m_options->weight(options);
  QString name = BiblosOptions::codeOptionToString(options);
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

QFrame*
OptionsDialog::initCodeEditorTab()
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
  connect(
    font_btn, &QAbstractButton::clicked, this, &OptionsDialog::setCodeFont);
  tabs->addTab(frm, tr("Font"));

  frm = initCodeTab(BiblosOptions::NORMAL);
  tabs->addTab(frm, BiblosOptions::codeOptionToString(BiblosOptions::NORMAL));
  frm = initCodeTab(BiblosOptions::ATTRIBUTE);
  tabs->addTab(frm,
               BiblosOptions::codeOptionToString(BiblosOptions::ATTRIBUTE));
  frm = initCodeTab(BiblosOptions::TAG);
  tabs->addTab(frm, BiblosOptions::codeOptionToString(BiblosOptions::TAG));
  frm = initCodeTab(BiblosOptions::STRING);
  tabs->addTab(frm, BiblosOptions::codeOptionToString(BiblosOptions::STRING));
  frm = initCodeTab(BiblosOptions::ERROR);
  tabs->addTab(frm, BiblosOptions::codeOptionToString(BiblosOptions::ERROR));
  frm = initCodeTab(BiblosOptions::STYLE);
  tabs->addTab(frm, BiblosOptions::codeOptionToString(BiblosOptions::STYLE));
  frm = initCodeTab(BiblosOptions::SCRIPT);
  tabs->addTab(frm, BiblosOptions::codeOptionToString(BiblosOptions::SCRIPT));

  return f;
}

void
OptionsDialog::setCodeFont()
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

void
OptionsDialog::setCodeColor(BiblosOptions::CodeOptions options)
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

void
OptionsDialog::setNormalColor()
{
  setCodeColor(BiblosOptions::NORMAL);
}

void
OptionsDialog::setCodeBackground(BiblosOptions::CodeOptions options)
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

void
OptionsDialog::setNormalBackground()
{
  setCodeBackground(BiblosOptions::NORMAL);
}

void
OptionsDialog::setNormalItalic(bool state)
{
  m_options->setItalic(BiblosOptions::NORMAL, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setNormalWeight(int index)
{
  m_options->setWeight(BiblosOptions::NORMAL, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setAttributeColor()
{
  setCodeColor(BiblosOptions::ATTRIBUTE);
}

void
OptionsDialog::setAttributeBackground()
{
  setCodeBackground(BiblosOptions::ATTRIBUTE);
}

void
OptionsDialog::setAttributeItalic(bool state)
{
  m_options->setItalic(BiblosOptions::ATTRIBUTE, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setAttributeWeight(int index)
{
  m_options->setWeight(BiblosOptions::ATTRIBUTE, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setTagColor()
{
  setCodeColor(BiblosOptions::TAG);
}

void
OptionsDialog::setTagBackground()
{
  setCodeBackground(BiblosOptions::TAG);
}

void
OptionsDialog::setTagItalic(bool state)
{
  m_options->setItalic(BiblosOptions::TAG, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setTagWeight(int index)
{
  m_options->setWeight(BiblosOptions::TAG, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setStringColor()
{
  setCodeColor(BiblosOptions::STRING);
}

void
OptionsDialog::setStringBackground()
{
  setCodeBackground(BiblosOptions::TAG);
}

void
OptionsDialog::setStringItalic(bool state)
{
  m_options->setItalic(BiblosOptions::STRING, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setStringWeight(int index)
{
  m_options->setWeight(BiblosOptions::STRING, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setErrorColor()
{
  setCodeColor(BiblosOptions::ERROR);
}

void
OptionsDialog::setErrorBackground()
{
  setCodeBackground(BiblosOptions::ERROR);
}

void
OptionsDialog::setErrorItalic(bool state)
{
  m_options->setItalic(BiblosOptions::ERROR, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setErrorWeight(int index)
{
  m_options->setWeight(BiblosOptions::ERROR, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setStyleColor()
{
  setCodeColor(BiblosOptions::STYLE);
}

void
OptionsDialog::setStyleBackground()
{
  setCodeBackground(BiblosOptions::STYLE);
}

void
OptionsDialog::setStyleItalic(bool state)
{
  m_options->setItalic(BiblosOptions::STYLE, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setStyleWeight(int index)
{
  m_options->setWeight(BiblosOptions::STYLE, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setScriptColor()
{
  setCodeColor(BiblosOptions::SCRIPT);
}

void
OptionsDialog::setScriptBackground()
{
  setCodeBackground(BiblosOptions::SCRIPT);
}

void
OptionsDialog::setScriptItalic(bool state)
{
  m_options->setItalic(BiblosOptions::SCRIPT, state);
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setScriptWeight(int index)
{
  m_options->setWeight(BiblosOptions::SCRIPT, QFont::Weight(index));
  emit codeChanged();
  m_modified = true;
}

void
OptionsDialog::setShowTocEdit(bool state)
{
  m_options->setTocVisible(state);
  emit codeChanged();
  emit showToc(state);
  m_modified = true;
}

void
OptionsDialog::setTocPosition()
{
  if (m_options->tocPosition() == BiblosOptions::LEFT) {
    m_options->setTocPosition(BiblosOptions::RIGHT);
    emit moveToc(BiblosOptions::RIGHT);
    m_modified = true;
  } else {
    m_options->setTocPosition(BiblosOptions::LEFT);
    emit moveToc(BiblosOptions::LEFT);
    m_modified = true;
  }
}

void
OptionsDialog::initGui()
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

void
OptionsDialog::resizeEvent(QResizeEvent* e)
{
  QSize size = e->size();
  m_options->setOptionsDlgSize(size);
  m_modified = true;
}

QColor
OptionsDialog::enactColorDialog(QColor old_color)
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
