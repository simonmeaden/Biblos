#include "ebookcommon.h"

EBookManifest::EBookManifest()
  : css(CSSMap(new QMap<QString, QString>()))
{}

EBookManifest::~EBookManifest() {}
