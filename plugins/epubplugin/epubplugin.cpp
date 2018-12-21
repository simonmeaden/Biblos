#include "epubplugin.h"

#include "ebookcommon.h"
#include "epubcontainer.h"
#include "epubdocument.h"

const QString EPubPlugin::m_plugin_name = "Epub Reader";
const QString EPubPlugin::m_plugin_group = "Book Reader";
const QString EPubPlugin::m_vendor = "SM Electronic Components";
const int EPubPlugin::m_major_version = EPUB_VERSION_MAJOR;
const int EPubPlugin::m_minor_version = EPUB_VERSION_MINOR;
const int EPubPlugin::m_build_version = EPUB_VERSION_BUILD;
const QString EPubPlugin::m_version = QString("%1.%2.%3")
                                          .arg(EPubPlugin::m_major_version)
                                          .arg(EPubPlugin::m_minor_version)
                                          .arg(EPubPlugin::m_build_version);
bool EPubPlugin::m_loaded = false;

const QString EPubPlugin::m_file_filter = "*.epub";

EPubPlugin::EPubPlugin(QObject *parent) : QObject(parent) {}

EPubPlugin::EPubPlugin(Options *options, QObject *parent)
    : QObject(parent), m_options(options) {}

/*!
 * \brief Creates an EBookDocument from the supplied file path.
 *
 * \param path - the path to the required file.
 * \return a new EBookDocument;
 */
IEBookDocument *EPubPlugin::createDocument(QString path) {
  m_document = new EPubDocument(this);
  m_document->openDocument(path);
  return m_document;
}

/*!
 * \brief Creates a code version of the EBookDocument.
 *
 * Creates a clone of the text document for use in a code editor.
 *
 * \param doc - the original EBookDocument.
 * \return a new EBookDocument;
 */
IEBookDocument *EPubPlugin::createCodeDocument() {
  if (m_document) {
    //    EBookContents *contents = doc->cloneData();
    EPubDocument *document = new EPubDocument(parent());
    document->setDocumentLayout(new QPlainTextDocumentLayout(document));
    //    document->setClonedData(contents);
    return document;
  }
  return Q_NULLPTR;
}

void EPubPlugin::saveDocument(IEBookDocument *document) {
  QString filename = document->filename();
  // TODO actual save epub file.
}

QString EPubPlugin::fileFilter() { return m_file_filter; }

QString EPubPlugin::pluginGroup() const { return m_plugin_group; }

QString EPubPlugin::pluginName() const { return m_plugin_name; }

QString EPubPlugin::vendor() const { return m_vendor; }

bool EPubPlugin::loaded() const { return m_loaded; }

void EPubPlugin::setLoaded(bool loaded) { m_loaded = loaded; }

QString EPubPlugin::version() const { return m_version; }

int EPubPlugin::majorVersion() const { return m_major_version; }

int EPubPlugin::minorVersion() const { return m_minor_version; }

int EPubPlugin::buildVersion() const { return m_build_version; }

void EPubPlugin::buildMenu() {
  // TODO.
}
