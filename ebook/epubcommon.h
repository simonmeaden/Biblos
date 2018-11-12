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

class QEPubContainer;

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

  Options()
    : codeFont(QFont("Courier", 10))
    , normalColor(Qt::black)
    , normalBack(Qt::white)
    , normalItalic(false)
    , normalWeight(QFont::Normal)
    , attributeColor(Qt::cyan)
    , attributeBack(Qt::white)
    , attributeItalic(false)
    , attributeWeight(QFont::Normal)
    , tagColor(Qt::blue)
    , tagBack(Qt::white)
    , tagItalic(false)
    , tagWeight(QFont::Normal)
    , stringColor(Qt::green)
    , stringBack(Qt::white)
    , stringItalic(false)
    , stringWeight(QFont::Normal)
    , errorColor(Qt::yellow)
    , errorBack(Qt::red)
    , errorItalic(false)
    , errorWeight(QFont::Normal)
    , scriptColor("mediumorchid")
    , scriptBack(Qt::white)
    , scriptItalic(false)
    , scriptWeight(QFont::Normal)
    , styleColor("silver")
    , styleBack(Qt::white)
    , styleItalic(false)
    , styleWeight(QFont::Normal)
  {}

  static QString codeOptionToString(CodeOptions options)
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

  static QString weightToString(QFont::Weight weight)
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

  QColor color(CodeOptions options)
  {
    switch (options) {
      case Options::NORMAL:
        return normalColor;
      case Options::TAG:
        return tagColor;
      case Options::ATTRIBUTE:
        return attributeColor;
      case Options::ERROR:
        return errorColor;
      case Options::STRING:
        return stringColor;
      case Options::SCRIPT:
        return scriptColor;
      case Options::STYLE:
        return styleColor;
    }
    return QColor();
  }

  void setColor(CodeOptions options, QColor color)
  {
    switch (options) {
      case Options::NORMAL:
        normalColor = color;
        break;
      case Options::TAG:
        tagColor = color;
        break;
      case Options::ATTRIBUTE:
        attributeColor = color;
        break;
      case Options::ERROR:
        errorColor = color;
        break;
      case Options::STRING:
        stringColor = color;
        break;
      case Options::SCRIPT:
        scriptColor = color;
        break;
      case Options::STYLE:
        styleColor = color;
        break;
    }
  }

  QColor background(CodeOptions options)
  {
    switch (options) {
      case Options::NORMAL:
        return normalBack;
      case Options::TAG:
        return tagBack;
      case Options::ATTRIBUTE:
        return attributeBack;
      case Options::ERROR:
        return errorBack;
      case Options::STRING:
        return stringBack;
      case Options::SCRIPT:
        return scriptBack;
      case Options::STYLE:
        return styleBack;
    }
    return QColor();
  }

  void setBackground(CodeOptions options, QColor color)
  {
    switch (options) {
      case Options::NORMAL:
        normalBack = color;
        break;
      case Options::TAG:
        tagBack = color;
        break;
      case Options::ATTRIBUTE:
        attributeBack = color;
        break;
      case Options::ERROR:
        errorBack = color;
        break;
      case Options::STRING:
        stringBack = color;
        break;
      case Options::SCRIPT:
        scriptBack = color;
        break;
      case Options::STYLE:
        styleBack = color;
        break;
    }
  }

  bool italic(CodeOptions options)
  {
    switch (options) {
      case Options::NORMAL:
        return normalItalic;
      case Options::TAG:
        return tagItalic;
      case Options::ATTRIBUTE:
        return attributeItalic;
      case Options::ERROR:
        return errorItalic;
      case Options::STRING:
        return stringItalic;
      case Options::SCRIPT:
        return scriptItalic;
      case Options::STYLE:
        return styleItalic;
    }
    return false;
  }

  void setItalic(CodeOptions options, bool italic)
  {
    switch (options) {
      case Options::NORMAL:
        normalItalic = italic;
        break;
      case Options::TAG:
        tagItalic = italic;
        break;
      case Options::ATTRIBUTE:
        attributeItalic = italic;
        break;
      case Options::ERROR:
        errorItalic = italic;
        break;
      case Options::STRING:
        stringItalic = italic;
        break;
      case Options::SCRIPT:
        scriptItalic = italic;
        break;
      case Options::STYLE:
        styleItalic = italic;
        break;
    }
  }

  QFont::Weight weight(CodeOptions options)
  {
    switch (options) {
      case Options::NORMAL:
        return normalWeight;
      case Options::TAG:
        return tagWeight;
      case Options::ATTRIBUTE:
        return attributeWeight;
      case Options::ERROR:
        return errorWeight;
      case Options::STRING:
        return stringWeight;
      case Options::SCRIPT:
        return scriptWeight;
      case Options::STYLE:
        return styleWeight;
    }
    return QFont::Normal;
  }

  void setWeight(CodeOptions options, QFont::Weight weight)
  {
    switch (options) {
      case Options::NORMAL:
        normalWeight = weight;
        break;
      case Options::TAG:
        tagWeight = weight;
        break;
      case Options::ATTRIBUTE:
        attributeWeight = weight;
        break;
      case Options::ERROR:
        errorWeight = weight;
        break;
      case Options::STRING:
        stringWeight = weight;
        break;
      case Options::SCRIPT:
        scriptWeight = weight;
        break;
      case Options::STYLE:
        styleWeight = weight;
        break;
    }
  }

  static QColor contrastingColor(QColor color)
  {
    int v = (color.red() + color.green() + color.blue()) / 3 > 127 ? 0 : 255;
    return QColor(v, v, v);
  }

  QRect rect;
  QSize options_dlg;
//  int x, y, width, height;
//  int prefx, prefy, prefwidth, prefheight;
  int popuptimeout;
  int currentindex;
  int bookcount;
  QStringList currentfiles;
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
typedef QSharedPointer<EPubToc> epubtoc_t;

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
  epubtoc_t toc;
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
  QEPubContainer* m_container;
  EPubItem m_currentItem;
  bool m_loaded;
};

#endif // COMMON_H
