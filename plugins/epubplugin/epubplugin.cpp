#include "epubplugin.h"

#include "epubcontainer.h"
#include "epubdocument.h"

const QString IPluginInterfaceClass::m_plugin_name = "Epub Reader";
const QString IPluginInterfaceClass::m_plugin_group = "Book Reader";
const QString IPluginInterfaceClass::m_vendor = "SM Electronic Components";
const int IPluginInterfaceClass::m_major_version = EPUB_VERSION_MAJOR;
const int IPluginInterfaceClass::m_minor_version = EPUB_VERSION_MINOR;
const int IPluginInterfaceClass::m_build_version = EPUB_VERSION_BUILD;
const QString IPluginInterfaceClass::m_version =
    QString("%1.%2.%3")
    .arg(IPluginInterfaceClass::m_major_version)
    .arg(IPluginInterfaceClass::m_minor_version)
    .arg(IPluginInterfaceClass::m_build_version);
bool IPluginInterfaceClass::m_loaded = false;

const QString IEBookInterfaceClass::m_file_filter = "*.epub";

EpubPlugin::EpubPlugin(Options *options, QObject *parent)
    : IEBookInterfaceClass(options, parent) {}

/*!
 * \brief Creates an EBookDocument from the supplied file path.
 *
 * \param path - the path to the required file.
 * \return a new EBookDocument;
 */
EBookDocument *EpubPlugin::createDocument(QString path)
{
    EPubDocument *document = new EPubDocument(this);
    document->openDocument(path);
    return document;
}

/*!
 * \brief Creates a code version of the EBookDocument.
 *
 * Creates a clone of the text document for use in a code editor.
 *
 * \param doc - the original EBookDocument.
 * \return a new EBookDocument;
 */
EBookDocument *EpubPlugin::createCodeDocument(EBookDocument *doc)
{
    if (doc) {
        EBookContents *contents = doc->cloneData();
        EPubDocument *document = new EPubDocument(doc->parent());
        document->setDocumentLayout(new QPlainTextDocumentLayout(document));
        document->setClonedData(contents);
        return document;
    }
}

void EpubPlugin::saveDocument(QString, EBookDocument *) {}
