#ifndef COMMON_H
#define COMMON_H

#include <QDateTime>
#include <QFont>
#include <QHash>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QPoint>
#include <QSharedPointer>
#include <QtPlugin>

class EPubContainer;

class Options
{
public:
  enum CodeOptions
  {
    NORMAL,
    TAG,
    STRING,
    ATTRIBUTE,
    ERROR,
    STYLE,
    SCRIPT,
  };

  Options();
  ~Options();

  QColor color(CodeOptions options);
  void setColor(CodeOptions options, QColor color);

  QColor background(CodeOptions options);
  void setBackground(CodeOptions options, QColor color);

  bool italic(CodeOptions options);
  void setItalic(CodeOptions options, bool italic);

  QFont::Weight weight(CodeOptions options);
  void setWeight(CodeOptions options, QFont::Weight weight);

  static QColor contrastingColor(QColor color);
  static QString codeOptionToString(CodeOptions options);
  static QString weightToString(QFont::Weight weight);

  QRect rect;
  QSize options_dlg;
  //  int x, y, width, height;
  //  int prefx, prefy, prefwidth, prefheight;
  int popuptimeout;
  int currentindex;
  int bookcount;
  QStringList current_files;
  QStringList current_lib_files;
  bool enablepopup;

  // Code Editor options
  QFont codeFont;
  QColor normalColor;
  QColor normalBack;
  bool normalItalic;
  QFont::Weight normalWeight;
  QColor attributeColor;
  QColor attributeBack;
  bool attributeItalic;
  QFont::Weight attributeWeight;
  QColor tagColor;
  QColor tagBack;
  bool tagItalic;
  QFont::Weight tagWeight;
  QColor stringColor;
  QColor stringBack;
  bool stringItalic;
  QFont::Weight stringWeight;
  QColor errorColor;
  QColor errorBack;
  bool errorItalic;
  QFont::Weight errorWeight;
  QColor scriptColor;
  QColor scriptBack;
  bool scriptItalic;
  QFont::Weight scriptWeight;
  QColor styleColor;
  QColor styleBack;
  bool styleItalic;
  QFont::Weight styleWeight;

  bool copy_books_to_store;
  bool use_calibre_library;
  bool delete_old_book;
  bool never_confirm_delete;
};

struct EPubItem
{
  QString path;
  QByteArray mimetype;
};

struct EPubPageReference
{
  enum StandardType
  {
    CoverPage,
    TitlePage,
    TableOfContents,
    Index,
    Glossary,
    Acknowledgements,
    Bibliography,
    Colophon,
    CopyrightPage,
    Dedication,
    Epigraph,
    Foreword,
    ListOfIllustrations,
    ListOfTables,
    Notes,
    Preface,
    Text,
    Other
  };

  static StandardType typeFromString(const QString& name);

  QString target;
  QString title;
};

struct EPubNavPoint
{
  EPubNavPoint(QString classname, QString id, QString label, QString src)
  {
    this->classname = classname;
    this->id = id;
    this->label = label;
    this->src = src;
  }
  QString classname;
  QString id;
  QString label;
  QString src;
};
typedef QSharedPointer<EPubNavPoint> navpoint_t;

struct EPubToc
{
  QString version;
  QString xml_lang;
  QString xmlns;
  QString title;
  QMap<QString, QString> metadata;
  QMap<int, navpoint_t> navmap;
};
typedef QSharedPointer<EPubToc> ebooktoc_t;

struct EBookData
{
  EBookData()
    : toc(Q_NULLPTR)
  {
    title = "";
    subject = "";
    language = "";
    author_names = "";
    authors.clear();
    date = QDateTime::currentDateTime();
    publisher = "";
  }
  QString title;
  QString subject;
  QString language;
  QString author_names;
  QStringList authors;
  QDateTime date;
  QString publisher;
  ebooktoc_t toc;
  QString dict_path;
  QString country;
};
Q_DECLARE_METATYPE(EBookData);

enum DocumentType
{
  PLAINTEXT,
  HTML
};

struct EBookContents
{
  QHash<QString, QByteArray> m_svgs;
  QHash<QString, QImage> m_renderedSvgs;
  EPubContainer *m_container;
  EPubItem m_currentItem;
  bool m_loaded;
};

#endif // COMMON_H
