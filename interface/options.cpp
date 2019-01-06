#include "options.h"

QString Options::POSITION = "window";
QString Options::DIALOG = "options dialog";
QString Options::PREF_CURRENT_INDEX = "current book";
QString Options::PREF_COUNT = "count";
QString Options::PREF_BOOKLIST = "book list";
QString Options::PREF_LIBRARY = "library list";
QString Options::CODE_OPTIONS = "code editor";
QString Options::CODE_FONT = "font";
QString Options::CODE_NORMAL = "normal";
QString Options::CODE_ATTRIBUTE = "attribute";
QString Options::CODE_TAG = "tag";
QString Options::CODE_ERROR = "error";
QString Options::CODE_STRING = "string";
QString Options::CODE_STYLE = "style";
QString Options::CODE_SCRIPT = "script";
QString Options::CODE_COLOR = "color";
QString Options::CODE_BACK = "background";
QString Options::CODE_WEIGHT = "weight";
QString Options::CODE_ITALIC = "italic";
QString Options::COPY_BOOKS_TO_STORE = "copy books to store";
QString Options::DELETE_OLD_BOOK = "delete old book";
QString Options::SHOW_TOC = "show toc";
QString Options::TOC_POSITION = "toc position";
QString Options::VIEW_STATE = "view state";

Options::Options(QObject* parent)
  : QObject(parent)
  , m_code_font(QFont("Courier", 10))
  , m_normal_color(Qt::black)
  , m_normal_back(Qt::white)
  , m_normal_italic(false)
  , m_normal_weight(QFont::Normal)
  , m_attribute_color(Qt::cyan)
  , m_attribute_back(Qt::white)
  , m_attribute_italic(false)
  , m_attribute_weight(QFont::Normal)
  , m_tag_color(Qt::blue)
  , m_tag_back(Qt::white)
  , m_tag_italic(false)
  , m_tag_weight(QFont::Normal)
  , m_string_color(Qt::green)
  , m_string_back(Qt::white)
  , m_string_italic(false)
  , m_string_weight(QFont::Normal)
  , m_error_color(Qt::yellow)
  , m_error_back(Qt::red)
  , m_error_italic(false)
  , m_error_weight(QFont::Normal)
  , m_script_color("mediumorchid")
  , m_script_back(Qt::white)
  , m_script_italic(false)
  , m_script_weight(QFont::Normal)
  , m_style_color("silver")
  , m_style_back(Qt::white)
  , m_style_italic(false)
  , m_style_weight(QFont::Normal)
  , m_copy_books_to_store(true)
  , m_delete_old_book(false)
  , m_never_confirm_delete(false)
{
}

Options::~Options()
{
}

void Options::save(const QString filename)
{
  QFile file(filename);
  if (m_prefchanged) {
    if (file.open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << POSITION;
        emitter << YAML::Value << m_rect;
        emitter << YAML::Key << DIALOG;
        emitter << YAML::Value << m_options_dlg_size;
        emitter << YAML::Key << PREF_CURRENT_INDEX;
        emitter << YAML::Value << m_currentindex;
        emitter << YAML::Key << COPY_BOOKS_TO_STORE;
        emitter << YAML::Value << m_copy_books_to_store;
        emitter << YAML::Key << DELETE_OLD_BOOK;
        emitter << YAML::Value << m_delete_old_book;
        emitter << YAML::Key << SHOW_TOC;
        emitter << YAML::Value << m_toc_visible;
        emitter << YAML::Key << TOC_POSITION;
        emitter << YAML::Value << (m_toc_position == Options::LEFT ? "LEFT" : "RIGHT");
        emitter << YAML::Key << PREF_BOOKLIST;
        {
          // Start of PREF_BOOKLIST
          emitter << YAML::BeginSeq;
          foreach (QString book, m_current_files) {
            emitter << book;
          }
          emitter << YAML::EndSeq;
        } // End of PREF_BOOKLIST
        //        emitter << YAML::Key << PREF_LIBRARY;
        //        {
        //          // Start of PREF_LIBRARY
        //          emitter << YAML::BeginSeq;
        //          foreach (QString book, m_current_lib_files) {
        //            emitter << book;
        //          }
        //          emitter << YAML::EndSeq;
        //        } // End of PREF_LIBRARY
        emitter << YAML::Key << CODE_OPTIONS;
        {
          // Start of CODE_OPTIONS
          emitter << YAML::BeginMap;
          emitter << YAML::Key << CODE_FONT;
          emitter << YAML::Value << m_code_font;
          emitter << YAML::Key << CODE_NORMAL;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_normal_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_normal_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_normal_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_normal_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_ATTRIBUTE;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_attribute_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_attribute_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_attribute_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_attribute_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_TAG;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_tag_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_tag_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_tag_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_tag_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_STRING;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_string_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_string_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << int(m_string_weight);
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_string_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_ERROR;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_error_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_error_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_error_weight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_error_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_STYLE;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_style_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_style_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_style_weight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_style_italic;
            emitter << YAML::EndMap;
          }
          emitter << YAML::Key << CODE_SCRIPT;
          emitter << YAML::Value;
          {
            emitter << YAML::BeginMap;
            emitter << YAML::Key << CODE_COLOR;
            emitter << YAML::Value << m_script_color;
            emitter << YAML::Key << CODE_BACK;
            emitter << YAML::Value << m_script_back;
            emitter << YAML::Key << CODE_WEIGHT;
            emitter << YAML::Value << m_script_weight;
            emitter << YAML::Key << CODE_ITALIC;
            emitter << YAML::Value << m_script_italic;
            emitter << YAML::EndMap;
          }
        } // End of CODE_OPTIONS
        emitter << YAML::EndMap;
      }
      QTextStream out(&file);
      out << emitter.c_str();
    }
  }
}

void Options::load(const QString filename)
{
  QFile file(filename);
  if (file.exists()) {
    m_preferences = YAML::LoadFile(file);
    // Last window position.
    if (m_preferences[POSITION]) {
      m_rect = m_preferences[POSITION].as<QRect>();
    } else {
      m_rect = QRect(0, 0, DEF_WIDTH, DEF_HEIGHT);
    }
    if (m_preferences[DIALOG]) {
      m_options_dlg_size = m_preferences[DIALOG].as<QSize>();
    } else {
      m_options_dlg_size = QSize(DEF_DLG_WIDTH, DEF_DLG_HEIGHT);
    }
    // Current book being read/edited.
    if (m_preferences[PREF_CURRENT_INDEX]) {
      m_currentindex = m_preferences[PREF_CURRENT_INDEX].as<int>();
    } else {
      m_currentindex = 0;
    }
    // Copy edited books to a book store.
    if (m_preferences[COPY_BOOKS_TO_STORE]) {
      m_copy_books_to_store = m_preferences[COPY_BOOKS_TO_STORE].as<bool>();
    } else {
      m_copy_books_to_store = true;
    }
    // Copy edited books to a book store.
    if (m_preferences[DELETE_OLD_BOOK]) {
      m_delete_old_book = m_preferences[DELETE_OLD_BOOK].as<bool>();
    } else {
      m_delete_old_book = false;
    }
    if (m_preferences[SHOW_TOC]) {
      m_toc_visible = m_preferences[SHOW_TOC].as<bool>();
    } else {
      m_toc_visible = true;
    }
    if (m_preferences[TOC_POSITION]) {
      QString pos = m_preferences[TOC_POSITION].as<QString>();
      m_toc_position = (pos == "LEFT" ? Options::LEFT : Options::RIGHT);
    } else {
      m_toc_position = Options::LEFT;
    }
    // Last books loaded in library.
    //    YAML::Node library = m_preferences[PREF_LIBRARY];
    //    if (library && library.IsSequence()) {
    //      m_current_lib_files.clear(); // Empty list just in case.
    //      for (uint i = 0; i < library.size(); i++) {
    //        m_current_lib_files.append(library[i].as<QString>());
    //      }
    //    }
    //    emit loadLibraryFiles(m_current_lib_files, m_currentindex);

    // Last books loaded NOT in library.
    YAML::Node books = m_preferences[PREF_BOOKLIST];
    if (books && books.IsSequence()) {
      m_current_files.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_current_files.append(books[i].as<QString>());
      }
    }
    emit loadLibraryFiles(m_current_files, m_currentindex);

    //    currentfiles = m_preferences["book
    //    list"].as<QStringList>();
    YAML::Node codeoptions = m_preferences[CODE_OPTIONS];
    if (codeoptions && codeoptions.IsMap()) {
      YAML::Node codefont = codeoptions[CODE_FONT];
      if (codefont) {
        m_code_font = codefont.as<QFont>();
      }
      YAML::Node codenormal = codeoptions[CODE_NORMAL];
      if (codenormal && codenormal.IsMap()) {
        m_normal_color = codenormal[CODE_COLOR].as<QColor>();
        m_normal_back = codenormal[CODE_BACK].as<QColor>();
        m_normal_weight = QFont::Weight(codenormal[CODE_WEIGHT].as<int>());
        m_normal_italic = codenormal[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeatt = codeoptions[CODE_ATTRIBUTE];
      if (codeatt && codeatt.IsMap()) {
        m_attribute_color = codeatt[CODE_COLOR].as<QColor>();
        m_attribute_back = codeatt[CODE_BACK].as<QColor>();
        m_attribute_weight = QFont::Weight(codeatt[CODE_WEIGHT].as<int>());
        m_attribute_italic = codeatt[CODE_ITALIC].as<bool>();
      }
      YAML::Node codetag = codeoptions[CODE_TAG];
      if (codetag && codetag.IsMap()) {
        m_tag_color = codetag[CODE_COLOR].as<QColor>();
        m_tag_back = codetag[CODE_BACK].as<QColor>();
        m_tag_weight = QFont::Weight(codetag[CODE_WEIGHT].as<int>());
        m_tag_italic = codeatt[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeattstr = codeoptions[CODE_STRING];
      if (codeattstr && codeattstr.IsMap()) {
        m_string_color = codeattstr[CODE_COLOR].as<QColor>();
        m_string_back = codeattstr[CODE_BACK].as<QColor>();
        m_string_weight = QFont::Weight(codeattstr[CODE_WEIGHT].as<int>());
        m_string_italic = codeattstr[CODE_ITALIC].as<bool>();
      }
      YAML::Node codeerr = codeoptions[CODE_ERROR];
      if (codeerr && codeerr.IsMap()) {
        m_error_color = codeerr[CODE_COLOR].as<QColor>();
        m_error_back = codeerr[CODE_BACK].as<QColor>();
        m_error_weight = QFont::Weight(codeerr[CODE_WEIGHT].as<int>());
        m_error_italic = codeerr[CODE_ITALIC].as<bool>();
      }
      YAML::Node codestyle = codeoptions[CODE_STYLE];
      if (codestyle && codestyle.IsMap()) {
        m_style_color = codestyle[CODE_COLOR].as<QColor>();
        m_style_back = codestyle[CODE_BACK].as<QColor>();
        m_style_weight = QFont::Weight(codestyle[CODE_WEIGHT].as<int>());
        m_style_italic = codestyle[CODE_ITALIC].as<bool>();
      }
      YAML::Node codescript = codeoptions[CODE_SCRIPT];
      if (codescript && codescript.IsMap()) {
        m_script_color = codescript[CODE_COLOR].as<QColor>();
        m_script_back = codescript[CODE_BACK].as<QColor>();
        m_script_weight = QFont::Weight(codescript[CODE_WEIGHT].as<int>());
        m_script_italic = codescript[CODE_ITALIC].as<bool>();
      }
    }
  } else {
    m_rect = QRect(DEF_X, DEF_Y, DEF_WIDTH, DEF_HEIGHT);
    m_currentindex = -1;
    m_current_files = QStringList();
    m_prefchanged = true;
  }
}

Options::TocPosition Options::tocPosition() const
{
  return m_toc_position;
}

void Options::setTocPosition(const TocPosition position)
{
  m_toc_position = position;
  m_prefchanged = true;
}

bool Options::tocVisible() const
{
  return m_toc_visible;
}

void Options::setTocVisible(const bool visible)
{
  m_toc_visible = visible;
  m_prefchanged = true;
}

QString Options::codeOptionToString(const CodeOptions options)
{
  switch (options) {
  case NORMAL:
    return QObject::tr("Normal");
  case TAG:
    return QObject::tr("Tag");
  case STRING:
    return QObject::tr("String");
  case ATTRIBUTE:
    return QObject::tr("Attribute");
  case ERROR:
    return QObject::tr("Error");
  case STYLE:
    return QObject::tr("Style");
  case SCRIPT:
    return QObject::tr("Script");
  }
  return QString();
}

QString Options::weightToString(const QFont::Weight weight)
{
  switch (weight) {
  case QFont::Thin:
    return "Thin";
  case QFont::ExtraLight:
    return "ExtraLight";
  case QFont::Light:
    return "Light";
  case QFont::Normal:
    return "Normal";
  case QFont::Medium:
    return "Medium";
  case QFont::DemiBold:
    return "DemiBold";
  case QFont::Bold:
    return "Bold";
  case QFont::ExtraBold:
    return "ExtraBold";
  case QFont::Black:
    return "Black";
  }
  return QString();
}

QColor Options::normalColor() const
{
  return m_normal_color;
}

void Options::setNormalColor(const QColor& normal_color)
{
  m_normal_color = normal_color;
  m_prefchanged = true;
}

QColor Options::normalBack() const
{
  return m_normal_back;
}

void Options::setNormalBack(const QColor& normal_back)
{
  m_normal_back = normal_back;
  m_prefchanged = true;
}

bool Options::normalItalic() const
{
  return m_normal_italic;
}

void Options::setNormalItalic(bool normal_italic)
{
  m_normal_italic = normal_italic;
  m_prefchanged = true;
}

QFont Options::codeFont() const
{
  return m_code_font;
}

void Options::setCodeFont(const QFont& code_font)
{
  m_code_font = code_font;
  m_prefchanged = true;
}

QFont::Weight Options::normalWeight() const
{
  return m_normal_weight;
}

void Options::setNormalWeight(const QFont::Weight& normal_weight)
{
  m_normal_weight = normal_weight;
  m_prefchanged = true;
}

QColor Options::attributeColor() const
{
  return m_attribute_color;
}

void Options::setAttributeColor(const QColor& attribute_color)
{
  m_attribute_color = attribute_color;
  m_prefchanged = true;
}

QColor Options::attributeBack() const
{
  return m_attribute_back;
}

void Options::setAttributeBack(const QColor& attribute_back)
{
  m_attribute_back = attribute_back;
  m_prefchanged = true;
}

bool Options::attributeItalic() const
{
  return m_attribute_italic;
}

void Options::setAttribute_italic(bool attribute_italic)
{
  m_attribute_italic = attribute_italic;
  m_prefchanged = true;
}

QFont::Weight Options::attributeWeight() const
{
  return m_attribute_weight;
}

void Options::setAttributeWeight(const QFont::Weight& attribute_weight)
{
  m_attribute_weight = attribute_weight;
  m_prefchanged = true;
}

QColor Options::tagColor() const
{
  return m_tag_color;
}

void Options::setTagColor(const QColor& tag_color)
{
  m_tag_color = tag_color;
  m_prefchanged = true;
}

QColor Options::tagBack() const
{
  return m_tag_back;
}

void Options::setTagBack(const QColor& tag_back)
{
  m_tag_back = tag_back;
  m_prefchanged = true;
}

bool Options::tagItalic() const
{
  return m_tag_italic;
}

void Options::setTagItalic(bool tag_italic)
{
  m_tag_italic = tag_italic;
  m_prefchanged = true;
}

QFont::Weight Options::tagWeight() const
{
  return m_tag_weight;
}

void Options::setTagWeight(const QFont::Weight& tag_weight)
{
  m_tag_weight = tag_weight;
  m_prefchanged = true;
}

QColor Options::stringColor() const
{
  return m_string_color;
}

void Options::setStringColor(const QColor& string_color)
{
  m_string_color = string_color;
  m_prefchanged = true;
}

QColor Options::stringBack() const
{
  return m_string_back;
}

void Options::setStringBack(const QColor& string_back)
{
  m_string_back = string_back;
  m_prefchanged = true;
}

bool Options::stringItalic() const
{
  return m_string_italic;
}

void Options::setStringItalic(bool string_italic)
{
  m_string_italic = string_italic;
  m_prefchanged = true;
}

QFont::Weight Options::stringWeight() const
{
  return m_string_weight;
}

void Options::setStringWeight(const QFont::Weight& string_weight)
{
  m_string_weight = string_weight;
  m_prefchanged = true;
}

QColor Options::errorColor() const
{
  return m_error_color;
}

void Options::setErrorColor(const QColor& error_color)
{
  m_error_color = error_color;
  m_prefchanged = true;
}

QColor Options::errorBack() const
{
  return m_error_back;
}

void Options::setErrorBack(const QColor& error_back)
{
  m_error_back = error_back;
  m_prefchanged = true;
}

bool Options::errorItalic() const
{
  return m_error_italic;
}

void Options::setErrorItalic(bool error_italic)
{
  m_error_italic = error_italic;
  m_prefchanged = true;
}

QFont::Weight Options::errorWeight() const
{
  return m_error_weight;
}

void Options::setErrorWeight(const QFont::Weight& error_weight)
{
  m_error_weight = error_weight;
  m_prefchanged = true;
}

QColor Options::scriptColor() const
{
  return m_script_color;
}

void Options::setScriptColor(const QColor& script_color)
{
  m_script_color = script_color;
  m_prefchanged = true;
}

QColor Options::scriptBack() const
{
  return m_script_back;
}

void Options::setScriptBack(const QColor& script_back)
{
  m_script_back = script_back;
  m_prefchanged = true;
}

bool Options::scriptItalic() const
{
  return m_script_italic;
}

void Options::setScriptItalic(bool script_italic)
{
  m_script_italic = script_italic;
  m_prefchanged = true;
}

QFont::Weight Options::scriptWeight() const
{
  return m_script_weight;
}

void Options::setScriptWeight(const QFont::Weight& script_weight)
{
  m_script_weight = script_weight;
  m_prefchanged = true;
}

QColor Options::styleColor() const
{
  return m_style_color;
}

void Options::setStyleColor(const QColor& style_color)
{
  m_style_color = style_color;
  m_prefchanged = true;
}

QColor Options::styleBack() const
{
  return m_style_back;
}

void Options::setStyleBack(const QColor& style_back)
{
  m_style_back = style_back;
  m_prefchanged = true;
}

bool Options::styleItalic() const
{
  return m_style_italic;
}

void Options::setStyleItalic(bool style_italic)
{
  m_style_italic = style_italic;
  m_prefchanged = true;
}

QFont::Weight Options::styleWeight() const
{
  return m_style_weight;
}

void Options::setStyleWeight(const QFont::Weight& style_weight)
{
  m_style_weight = style_weight;
  m_prefchanged = true;
}

QRect Options::rect() const
{
  return m_rect;
}

void Options::setRect(const QRect& rect)
{
  m_rect = rect;
  m_prefchanged = true;
}

QSize Options::optionsDlgSize() const
{
  return m_options_dlg_size;
}

void Options::setOptionsDlgSize(const QSize& options_dlg)
{
  m_options_dlg_size = options_dlg;
  m_prefchanged = true;
}

int Options::currentIndex() const
{
  return m_currentindex;
}

void Options::setCurrentIndex(const int index)
{
  m_currentindex = index;
  m_prefchanged = true;
}

bool Options::currentFilesContains(const QString filename)
{
  return m_current_files.contains(filename);
}

void Options::appendCurrentFile(const QString filename)
{
  if (!m_current_files.contains(filename)) {
    m_current_files.append(filename);
    m_prefchanged = true;
  }
}

//void Options::moveToLibFile(const QString filename)
//{
//  if (m_current_files.contains(filename)) {
//    m_current_files.removeOne(filename);
//    m_prefchanged = true;
//  }
//  if (!m_current_lib_files.contains(filename)) {
//    m_current_lib_files.append(filename);
//    m_prefchanged = true;
//  }
//}

QStringList Options::currentfiles() const
{
  return m_current_files;
}

int Options::bookCount() const
{
  return m_current_files.size();
}

bool Options::deleteOldBook()
{
  return m_delete_old_book;
}

void Options::setDeleteOldBook(bool delete_book)
{
  m_delete_old_book = delete_book;
  m_prefchanged = true;
}

bool Options::alwaysConfirmDelete()
{
  return !m_never_confirm_delete;
}

bool Options::neverConfirmDelete()
{
  return m_never_confirm_delete;
}

void Options::setNeverConfirmDelete(bool never_confirm)
{
  m_never_confirm_delete = never_confirm;
  m_prefchanged = true;
}

bool Options::copyBooksToStore()
{
  return m_copy_books_to_store;
}

void Options::setCopyBooksToStore(bool copy_books)
{
  m_copy_books_to_store = copy_books;
  m_prefchanged = true;
}

bool Options::useCalibreLibrary()
{
  return m_use_calibre_library;
}

void Options::setUseCalibreLibrary(bool use_calibre)
{
  m_use_calibre_library = use_calibre;
  m_prefchanged = true;
}

Options::ViewState Options::viewState() const
{
  return m_view_state;
}

void Options::setViewState(const Options::ViewState& view_state)
{
  m_view_state = view_state;
  m_prefchanged = true;
}

QColor Options::color(const CodeOptions options) const
{
  switch (options) {
  case Options::NORMAL:
    return m_normal_color;
  case Options::TAG:
    return m_tag_color;
  case Options::ATTRIBUTE:
    return m_attribute_color;
  case Options::ERROR:
    return m_error_color;
  case Options::STRING:
    return m_string_color;
  case Options::SCRIPT:
    return m_script_color;
  case Options::STYLE:
    return m_style_color;
  }
  return QColor();
}

void Options::setColor(const CodeOptions options, const QColor color)
{
  switch (options) {
  case Options::NORMAL:
    m_normal_color = color;
    m_prefchanged = true;
    break;
  case Options::TAG:
    m_tag_color = color;
    m_prefchanged = true;
    break;
  case Options::ATTRIBUTE:
    m_attribute_color = color;
    m_prefchanged = true;
    break;
  case Options::ERROR:
    m_error_color = color;
    m_prefchanged = true;
    break;
  case Options::STRING:
    m_string_color = color;
    m_prefchanged = true;
    break;
  case Options::SCRIPT:
    m_script_color = color;
    m_prefchanged = true;
    break;
  case Options::STYLE:
    m_style_color = color;
    m_prefchanged = true;
    break;
  }
}

QColor Options::background(const CodeOptions options) const
{
  switch (options) {
  case Options::NORMAL:
    return m_normal_back;
  case Options::TAG:
    return m_tag_back;
  case Options::ATTRIBUTE:
    return m_attribute_back;
  case Options::ERROR:
    return m_error_back;
  case Options::STRING:
    return m_string_back;
  case Options::SCRIPT:
    return m_script_back;
  case Options::STYLE:
    return m_style_back;
  }
  return QColor();
}

void Options::setBackground(const CodeOptions options, const QColor color)
{
  switch (options) {
  case Options::NORMAL:
    m_normal_back = color;
    m_prefchanged = true;
    break;
  case Options::TAG:
    m_tag_back = color;
    m_prefchanged = true;
    break;
  case Options::ATTRIBUTE:
    m_attribute_back = color;
    m_prefchanged = true;
    break;
  case Options::ERROR:
    m_error_back = color;
    m_prefchanged = true;
    break;
  case Options::STRING:
    m_string_back = color;
    m_prefchanged = true;
    break;
  case Options::SCRIPT:
    m_script_back = color;
    m_prefchanged = true;
    break;
  case Options::STYLE:
    m_style_back = color;
    m_prefchanged = true;
    break;
  }
}

bool Options::italic(const CodeOptions options) const
{
  switch (options) {
  case Options::NORMAL:
    return m_normal_italic;
  case Options::TAG:
    return m_tag_italic;
  case Options::ATTRIBUTE:
    return m_attribute_italic;
  case Options::ERROR:
    return m_error_italic;
  case Options::STRING:
    return m_string_italic;
  case Options::SCRIPT:
    return m_script_italic;
  case Options::STYLE:
    return m_style_italic;
  }
  return false;
}

void Options::setItalic(const CodeOptions options, const bool italic)
{
  switch (options) {
  case Options::NORMAL:
    m_normal_italic = italic;
    m_prefchanged = true;
    break;
  case Options::TAG:
    m_tag_italic = italic;
    m_prefchanged = true;
    break;
  case Options::ATTRIBUTE:
    m_attribute_italic = italic;
    m_prefchanged = true;
    break;
  case Options::ERROR:
    m_error_italic = italic;
    m_prefchanged = true;
    break;
  case Options::STRING:
    m_string_italic = italic;
    m_prefchanged = true;
    break;
  case Options::SCRIPT:
    m_script_italic = italic;
    m_prefchanged = true;
    break;
  case Options::STYLE:
    m_style_italic = italic;
    m_prefchanged = true;
    break;
  }
}

QFont::Weight Options::weight(const CodeOptions options) const
{
  switch (options) {
  case Options::NORMAL:
    return m_normal_weight;
  case Options::TAG:
    return m_tag_weight;
  case Options::ATTRIBUTE:
    return m_attribute_weight;
  case Options::ERROR:
    return m_error_weight;
  case Options::STRING:
    return m_string_weight;
  case Options::SCRIPT:
    return m_script_weight;
  case Options::STYLE:
    return m_style_weight;
  }
  return QFont::Normal;
}

void Options::setWeight(const CodeOptions options, const QFont::Weight weight)
{
  switch (options) {
  case Options::NORMAL:
    m_normal_weight = weight;
    m_prefchanged = true;
    break;
  case Options::TAG:
    m_tag_weight = weight;
    m_prefchanged = true;
    break;
  case Options::ATTRIBUTE:
    m_prefchanged = true;
    m_attribute_weight = weight;
    break;
  case Options::ERROR:
    m_error_weight = weight;
    m_prefchanged = true;
    break;
  case Options::STRING:
    m_string_weight = weight;
    m_prefchanged = true;
    break;
  case Options::SCRIPT:
    m_script_weight = weight;
    m_prefchanged = true;
    break;
  case Options::STYLE:
    m_style_weight = weight;
    m_prefchanged = true;
    break;
  }
}

QColor Options::contrastingColor(const QColor color)
{
  int v = (color.red() + color.green() + color.blue()) / 3 > 127 ? 0 : 255;
  return QColor(v, v, v);
}
