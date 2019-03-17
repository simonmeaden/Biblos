#include "options.h"

QString BiblosOptions::POSITION = "window";
QString BiblosOptions::DIALOG = "options dialog";
QString BiblosOptions::PREF_CURRENT_INDEX = "current book";
QString BiblosOptions::PREF_CURRENT_ITEMS = "current book item";
QString BiblosOptions::PREF_CURRENT_LINE_NOS = "current book line nos";
QString BiblosOptions::PREF_COUNT = "count";
QString BiblosOptions::PREF_BOOKLIST = "book list";
QString BiblosOptions::PREF_LIBRARY = "library list";
QString BiblosOptions::CODE_OPTIONS = "code editor";
QString BiblosOptions::CODE_FONT = "font";
QString BiblosOptions::CODE_NORMAL = "normal";
QString BiblosOptions::CODE_ATTRIBUTE = "attribute";
QString BiblosOptions::CODE_TAG = "tag";
QString BiblosOptions::CODE_ERROR = "error";
QString BiblosOptions::CODE_STRING = "string";
QString BiblosOptions::CODE_STYLE = "style";
QString BiblosOptions::CODE_SCRIPT = "script";
QString BiblosOptions::CODE_COLOR = "color";
QString BiblosOptions::CODE_BACK = "background";
QString BiblosOptions::CODE_WEIGHT = "weight";
QString BiblosOptions::CODE_ITALIC = "italic";
QString BiblosOptions::COPY_BOOKS_TO_STORE = "copy books to store";
QString BiblosOptions::DELETE_OLD_BOOK = "delete old book";
QString BiblosOptions::SHOW_TOC = "show toc";
QString BiblosOptions::TOC_POSITION = "toc position";
QString BiblosOptions::VIEW_STATE = "view state";

BiblosOptions::BiblosOptions()
  : m_code_font(QFont("Courier", 10))
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
{
  lib_key = QPixmapCache::insert(QPixmap(":/icons/library"));
  up_key = QPixmapCache::insert(QPixmap(":/icons/go-up"));
  down_key = QPixmapCache::insert(QPixmap(":/icons/go-down"));
  previous_key = QPixmapCache::insert(QPixmap(":/icons/go-previous"));
  next_key = QPixmapCache::insert(QPixmap(":/icons/go-next"));
  top_key = QPixmapCache::insert(QPixmap(":/icons/go-top"));
  bottom_key = QPixmapCache::insert(QPixmap(":/icons/go-bottom"));
  plus_key = QPixmapCache::insert(QPixmap(":/icons/add"));
  minus_key = QPixmapCache::insert(QPixmap(":/icons/remove"));
  new_key = QPixmapCache::insert(QPixmap(":/icons/new"));
  open_key = QPixmapCache::insert(QPixmap(":/icons/open"));
  save_key = QPixmapCache::insert(QPixmap(":/icons/save"));
  editor_key = QPixmapCache::insert(QPixmap(":/icons/editor"));
  code_key = QPixmapCache::insert(QPixmap(":/icons/code"));
  meta_key = QPixmapCache::insert(QPixmap(":/icons/metadata"));
  bookshelf_key = QPixmapCache::insert(QPixmap(":/icons/bookshelf"));
  tree_key = QPixmapCache::insert(QPixmap(":/icons/tree"));
  dialog_error_key = QPixmapCache::insert(QPixmap(":/icons/dialog-error"));
  view_refresh_key = QPixmapCache::insert(QPixmap(":/icons/view-refresh"));
  text_html_key = QPixmapCache::insert(QPixmap(":/icons/text-html"));
}

BiblosOptions::~BiblosOptions() {}

void
BiblosOptions::save(const QString filename)
{
  QFile* file;
  if (filename.isEmpty())
    file = new QFile(configFile());
  else
    file = new QFile(filename);

  if (m_pref_changed) {
    if (file->open((QFile::ReadWrite | QFile::Truncate))) {
      YAML::Emitter emitter;
      {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << POSITION;
        emitter << YAML::Value << m_rect;
        emitter << YAML::Key << DIALOG;
        emitter << YAML::Value << m_options_dlg_size;
        emitter << YAML::Key << PREF_CURRENT_INDEX;
        emitter << YAML::Value << m_currentindex;
        emitter << YAML::Key << SHOW_TOC;
        emitter << YAML::Value << m_toc_visible;
        emitter << YAML::Key << TOC_POSITION;
        emitter << YAML::Value
                << (m_toc_position == BiblosOptions::LEFT ? "LEFT" : "RIGHT");
        emitter << YAML::Key << PREF_BOOKLIST;
        {
          // Start of PREF_BOOKLIST
          emitter << YAML::BeginSeq;
          foreach (QString book, m_current_files) {
            emitter << book;
          }
          emitter << YAML::EndSeq;
        } // End of PREF_BOOKLIST
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
      QTextStream out(file);
      out << emitter.c_str();
      file->close();
    }
  }
}

void
BiblosOptions::load(const QString filename)
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
    if (m_preferences[SHOW_TOC]) {
      m_toc_visible = m_preferences[SHOW_TOC].as<bool>();
    } else {
      m_toc_visible = true;
    }
    if (m_preferences[TOC_POSITION]) {
      QString pos = m_preferences[TOC_POSITION].as<QString>();
      m_toc_position =
        (pos == "LEFT" ? BiblosOptions::LEFT : BiblosOptions::RIGHT);
    } else {
      m_toc_position = BiblosOptions::LEFT;
    }
    // Last books loaded in library.
    YAML::Node books = m_preferences[PREF_BOOKLIST];
    if (books && books.IsSequence()) {
      m_current_files.clear(); // Empty list just in case.
      for (uint i = 0; i < books.size(); i++) {
        m_current_files.append(books[i].as<QString>());
      }
    }

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
    m_pref_changed = true;
  }
}

BiblosOptions::TocPosition
BiblosOptions::tocPosition() const
{
  return m_toc_position;
}

void
BiblosOptions::setTocPosition(const TocPosition position)
{
  m_toc_position = position;
  m_pref_changed = true;
}

bool
BiblosOptions::tocVisible() const
{
  return m_toc_visible;
}

void
BiblosOptions::setTocVisible(const bool visible)
{
  m_toc_visible = visible;
  m_pref_changed = true;
}

QString
BiblosOptions::codeOptionToString(const CodeOptions options)
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

QString
BiblosOptions::weightToString(const QFont::Weight weight)
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

QColor
BiblosOptions::normalColor() const
{
  return m_normal_color;
}

void
BiblosOptions::setNormalColor(const QColor& normal_color)
{
  m_normal_color = normal_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::normalBack() const
{
  return m_normal_back;
}

void
BiblosOptions::setNormalBack(const QColor& normal_back)
{
  m_normal_back = normal_back;
  m_pref_changed = true;
}

bool
BiblosOptions::normalItalic() const
{
  return m_normal_italic;
}

void
BiblosOptions::setNormalItalic(bool normal_italic)
{
  m_normal_italic = normal_italic;
  m_pref_changed = true;
}

QFont
BiblosOptions::codeFont() const
{
  return m_code_font;
}

void
BiblosOptions::setCodeFont(const QFont& code_font)
{
  m_code_font = code_font;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::normalWeight() const
{
  return m_normal_weight;
}

void
BiblosOptions::setNormalWeight(const QFont::Weight& normal_weight)
{
  m_normal_weight = normal_weight;
  m_pref_changed = true;
}

QColor
BiblosOptions::attributeColor() const
{
  return m_attribute_color;
}

void
BiblosOptions::setAttributeColor(const QColor& attribute_color)
{
  m_attribute_color = attribute_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::attributeBack() const
{
  return m_attribute_back;
}

void
BiblosOptions::setAttributeBack(const QColor& attribute_back)
{
  m_attribute_back = attribute_back;
  m_pref_changed = true;
}

bool
BiblosOptions::attributeItalic() const
{
  return m_attribute_italic;
}

void
BiblosOptions::setAttribute_italic(bool attribute_italic)
{
  m_attribute_italic = attribute_italic;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::attributeWeight() const
{
  return m_attribute_weight;
}

void
BiblosOptions::setAttributeWeight(const QFont::Weight& attribute_weight)
{
  m_attribute_weight = attribute_weight;
  m_pref_changed = true;
}

QColor
BiblosOptions::tagColor() const
{
  return m_tag_color;
}

void
BiblosOptions::setTagColor(const QColor& tag_color)
{
  m_tag_color = tag_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::tagBack() const
{
  return m_tag_back;
}

void
BiblosOptions::setTagBack(const QColor& tag_back)
{
  m_tag_back = tag_back;
  m_pref_changed = true;
}

bool
BiblosOptions::tagItalic() const
{
  return m_tag_italic;
}

void
BiblosOptions::setTagItalic(bool tag_italic)
{
  m_tag_italic = tag_italic;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::tagWeight() const
{
  return m_tag_weight;
}

void
BiblosOptions::setTagWeight(const QFont::Weight& tag_weight)
{
  m_tag_weight = tag_weight;
  m_pref_changed = true;
}

QColor
BiblosOptions::stringColor() const
{
  return m_string_color;
}

void
BiblosOptions::setStringColor(const QColor& string_color)
{
  m_string_color = string_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::stringBack() const
{
  return m_string_back;
}

void
BiblosOptions::setStringBack(const QColor& string_back)
{
  m_string_back = string_back;
  m_pref_changed = true;
}

bool
BiblosOptions::stringItalic() const
{
  return m_string_italic;
}

void
BiblosOptions::setStringItalic(bool string_italic)
{
  m_string_italic = string_italic;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::stringWeight() const
{
  return m_string_weight;
}

void
BiblosOptions::setStringWeight(const QFont::Weight& string_weight)
{
  m_string_weight = string_weight;
  m_pref_changed = true;
}

QColor
BiblosOptions::errorColor() const
{
  return m_error_color;
}

void
BiblosOptions::setErrorColor(const QColor& error_color)
{
  m_error_color = error_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::errorBack() const
{
  return m_error_back;
}

void
BiblosOptions::setErrorBack(const QColor& error_back)
{
  m_error_back = error_back;
  m_pref_changed = true;
}

bool
BiblosOptions::errorItalic() const
{
  return m_error_italic;
}

void
BiblosOptions::setErrorItalic(bool error_italic)
{
  m_error_italic = error_italic;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::errorWeight() const
{
  return m_error_weight;
}

void
BiblosOptions::setErrorWeight(const QFont::Weight& error_weight)
{
  m_error_weight = error_weight;
  m_pref_changed = true;
}

QColor
BiblosOptions::scriptColor() const
{
  return m_script_color;
}

void
BiblosOptions::setScriptColor(const QColor& script_color)
{
  m_script_color = script_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::scriptBack() const
{
  return m_script_back;
}

void
BiblosOptions::setScriptBack(const QColor& script_back)
{
  m_script_back = script_back;
  m_pref_changed = true;
}

bool
BiblosOptions::scriptItalic() const
{
  return m_script_italic;
}

void
BiblosOptions::setScriptItalic(bool script_italic)
{
  m_script_italic = script_italic;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::scriptWeight() const
{
  return m_script_weight;
}

void
BiblosOptions::setScriptWeight(const QFont::Weight& script_weight)
{
  m_script_weight = script_weight;
  m_pref_changed = true;
}

QColor
BiblosOptions::styleColor() const
{
  return m_style_color;
}

void
BiblosOptions::setStyleColor(const QColor& style_color)
{
  m_style_color = style_color;
  m_pref_changed = true;
}

QColor
BiblosOptions::styleBack() const
{
  return m_style_back;
}

void
BiblosOptions::setStyleBack(const QColor& style_back)
{
  m_style_back = style_back;
  m_pref_changed = true;
}

bool
BiblosOptions::styleItalic() const
{
  return m_style_italic;
}

void
BiblosOptions::setStyleItalic(bool style_italic)
{
  m_style_italic = style_italic;
  m_pref_changed = true;
}

QFont::Weight
BiblosOptions::styleWeight() const
{
  return m_style_weight;
}

void
BiblosOptions::setStyleWeight(const QFont::Weight& style_weight)
{
  m_style_weight = style_weight;
  m_pref_changed = true;
}

QString
BiblosOptions::homeDir() const
{
  return m_home_directory;
}

void
BiblosOptions::setHomeDir(const QString& home_directiory)
{
  m_home_directory = home_directiory;
}

QString
BiblosOptions::libraryDir() const
{
  return m_library_directory;
}

void
BiblosOptions::setLibraryDir(const QString& library_directory)
{
  m_library_directory = library_directory;
}

QString
BiblosOptions::configDir() const
{
  return m_config_directory;
}

void
BiblosOptions::setConfigDir(const QString& config_directory)
{
  m_config_directory = config_directory;
}

QString
BiblosOptions::configFile() const
{
  return m_config_file;
}

void
BiblosOptions::setConfigFile(const QString& config_file)
{
  m_config_file = config_file;
}

QString
BiblosOptions::libraryFile() const
{
  return m_lib_file;
}

void
BiblosOptions::setLibraryFile(const QString& lib_file)
{
  m_lib_file = lib_file;
}

QString
BiblosOptions::authorsFile() const
{
  return m_authors_file;
}

void
BiblosOptions::setAuthorsFile(const QString& authors_file)
{
  m_authors_file = authors_file;
}

QString
BiblosOptions::dicDir() const
{
  return m_dic_directory;
}

void
BiblosOptions::setDicDir(const QString& dic_dir)
{
  m_dic_directory = dic_dir;
}

QString
BiblosOptions::bdicDir() const
{
  return m_bdic_directory;
}

void
BiblosOptions::setBdicDir(const QString& dic_dir)
{
  m_bdic_directory = dic_dir;
}

QString
BiblosOptions::seriesFile() const
{
  return m_series_file;
}

void
BiblosOptions::setSeriesFile(const QString& seriesFile)
{
  m_series_file = seriesFile;
}

QRect
BiblosOptions::rect() const
{
  return m_rect;
}

void
BiblosOptions::setRect(const QRect& rect)
{
  m_rect = rect;
  m_pref_changed = true;
}

QSize
BiblosOptions::optionsDlgSize() const
{
  return m_options_dlg_size;
}

void
BiblosOptions::setOptionsDlgSize(const QSize& options_dlg)
{
  m_options_dlg_size = options_dlg;
  m_pref_changed = true;
}

int
BiblosOptions::currentIndex() const
{
  return m_currentindex;
}

void
BiblosOptions::setCurrentIndex(const int index)
{
  m_currentindex = index;
  m_pref_changed = true;
}

bool
BiblosOptions::currentFilesContains(const QString filename)
{
  return m_current_files.contains(filename);
}

void
BiblosOptions::appendCurrentFile(const QString filename)
{
  if (!m_current_files.contains(filename)) {
    m_current_files.append(filename);
    m_pref_changed = true;
  }
}

void
BiblosOptions::replaceCurrentFile(const QString filename)
{
  m_current_files.replace(m_currentindex, filename);
  m_pref_changed = true;
}

QStringList
BiblosOptions::currentfiles() const
{
  return m_current_files;
}

int
BiblosOptions::bookCount() const
{
  return m_current_files.size();
}

BiblosOptions::ViewState
BiblosOptions::viewState() const
{
  return m_view_state;
}

void
BiblosOptions::setViewState(const BiblosOptions::ViewState& view_state)
{
  m_view_state = view_state;
  m_pref_changed = true;
}

QColor
BiblosOptions::color(const CodeOptions options) const
{
  switch (options) {
    case BiblosOptions::NORMAL:
      return m_normal_color;
    case BiblosOptions::TAG:
      return m_tag_color;
    case BiblosOptions::ATTRIBUTE:
      return m_attribute_color;
    case BiblosOptions::ERROR:
      return m_error_color;
    case BiblosOptions::STRING:
      return m_string_color;
    case BiblosOptions::SCRIPT:
      return m_script_color;
    case BiblosOptions::STYLE:
      return m_style_color;
  }
  return QColor();
}

void
BiblosOptions::setColor(const CodeOptions options, const QColor color)
{
  switch (options) {
    case BiblosOptions::NORMAL:
      m_normal_color = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::TAG:
      m_tag_color = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::ATTRIBUTE:
      m_attribute_color = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::ERROR:
      m_error_color = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::STRING:
      m_string_color = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::SCRIPT:
      m_script_color = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::STYLE:
      m_style_color = color;
      m_pref_changed = true;
      break;
  }
}

QColor
BiblosOptions::background(const CodeOptions options) const
{
  switch (options) {
    case BiblosOptions::NORMAL:
      return m_normal_back;
    case BiblosOptions::TAG:
      return m_tag_back;
    case BiblosOptions::ATTRIBUTE:
      return m_attribute_back;
    case BiblosOptions::ERROR:
      return m_error_back;
    case BiblosOptions::STRING:
      return m_string_back;
    case BiblosOptions::SCRIPT:
      return m_script_back;
    case BiblosOptions::STYLE:
      return m_style_back;
  }
  return QColor();
}

void
BiblosOptions::setBackground(const CodeOptions options, const QColor color)
{
  switch (options) {
    case BiblosOptions::NORMAL:
      m_normal_back = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::TAG:
      m_tag_back = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::ATTRIBUTE:
      m_attribute_back = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::ERROR:
      m_error_back = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::STRING:
      m_string_back = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::SCRIPT:
      m_script_back = color;
      m_pref_changed = true;
      break;
    case BiblosOptions::STYLE:
      m_style_back = color;
      m_pref_changed = true;
      break;
  }
}

bool
BiblosOptions::italic(const CodeOptions options) const
{
  switch (options) {
    case BiblosOptions::NORMAL:
      return m_normal_italic;
    case BiblosOptions::TAG:
      return m_tag_italic;
    case BiblosOptions::ATTRIBUTE:
      return m_attribute_italic;
    case BiblosOptions::ERROR:
      return m_error_italic;
    case BiblosOptions::STRING:
      return m_string_italic;
    case BiblosOptions::SCRIPT:
      return m_script_italic;
    case BiblosOptions::STYLE:
      return m_style_italic;
  }
  return false;
}

void
BiblosOptions::setItalic(const CodeOptions options, const bool italic)
{
  switch (options) {
    case BiblosOptions::NORMAL:
      m_normal_italic = italic;
      m_pref_changed = true;
      break;
    case BiblosOptions::TAG:
      m_tag_italic = italic;
      m_pref_changed = true;
      break;
    case BiblosOptions::ATTRIBUTE:
      m_attribute_italic = italic;
      m_pref_changed = true;
      break;
    case BiblosOptions::ERROR:
      m_error_italic = italic;
      m_pref_changed = true;
      break;
    case BiblosOptions::STRING:
      m_string_italic = italic;
      m_pref_changed = true;
      break;
    case BiblosOptions::SCRIPT:
      m_script_italic = italic;
      m_pref_changed = true;
      break;
    case BiblosOptions::STYLE:
      m_style_italic = italic;
      m_pref_changed = true;
      break;
  }
}

QFont::Weight
BiblosOptions::weight(const CodeOptions options) const
{
  switch (options) {
    case BiblosOptions::NORMAL:
      return m_normal_weight;
    case BiblosOptions::TAG:
      return m_tag_weight;
    case BiblosOptions::ATTRIBUTE:
      return m_attribute_weight;
    case BiblosOptions::ERROR:
      return m_error_weight;
    case BiblosOptions::STRING:
      return m_string_weight;
    case BiblosOptions::SCRIPT:
      return m_script_weight;
    case BiblosOptions::STYLE:
      return m_style_weight;
  }
  return QFont::Normal;
}

void
BiblosOptions::setWeight(const CodeOptions options, const QFont::Weight weight)
{
  switch (options) {
    case BiblosOptions::NORMAL:
      m_normal_weight = weight;
      m_pref_changed = true;
      break;
    case BiblosOptions::TAG:
      m_tag_weight = weight;
      m_pref_changed = true;
      break;
    case BiblosOptions::ATTRIBUTE:
      m_pref_changed = true;
      m_attribute_weight = weight;
      break;
    case BiblosOptions::ERROR:
      m_error_weight = weight;
      m_pref_changed = true;
      break;
    case BiblosOptions::STRING:
      m_string_weight = weight;
      m_pref_changed = true;
      break;
    case BiblosOptions::SCRIPT:
      m_script_weight = weight;
      m_pref_changed = true;
      break;
    case BiblosOptions::STYLE:
      m_style_weight = weight;
      m_pref_changed = true;
      break;
  }
}

QColor
BiblosOptions::contrastingColor(const QColor color)
{
  int v = (color.red() + color.green() + color.blue()) / 3 > 127 ? 0 : 255;
  return QColor(v, v, v);
}
