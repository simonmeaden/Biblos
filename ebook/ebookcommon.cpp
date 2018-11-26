#include "ebookcommon.h"

QString
Options::codeOptionToString(Options::CodeOptions options)
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
Options::weightToString(QFont::Weight weight)
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

Options::Options()
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
  , copy_books_to_store(true)
  , delete_old_book(false)
  , never_confirm_delete(false)
{}

Options::~Options() {}

QColor
Options::color(Options::CodeOptions options)
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

void
Options::setColor(Options::CodeOptions options, QColor color)
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

QColor
Options::background(Options::CodeOptions options)
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

void
Options::setBackground(Options::CodeOptions options, QColor color)
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

bool
Options::italic(Options::CodeOptions options)
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

void
Options::setItalic(Options::CodeOptions options, bool italic)
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

QFont::Weight
Options::weight(Options::CodeOptions options)
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

void
Options::setWeight(Options::CodeOptions options, QFont::Weight weight)
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

QColor
Options::contrastingColor(QColor color)
{
  int v = (color.red() + color.green() + color.blue()) / 3 > 127 ? 0 : 255;
  return QColor(v, v, v);
}

