#include "mobiplugin.h"

#include "mobidocument.h"

const QString MobiPlugin::m_plugin_name = "Mobi Reader";
const QString MobiPlugin::m_plugin_group = "Book Reader";
const QString MobiPlugin::m_vendor = "SM Electronic Components";
const int MobiPlugin::m_major_version = MOBI_VERSION_MAJOR;
const int MobiPlugin::m_minor_version = MOBI_VERSION_MINOR;
const int MobiPlugin::m_build_version = MOBI_VERSION_BUILD;
const QString MobiPlugin::m_version = QString("%1.%2.%3")
                                          .arg(MobiPlugin::m_major_version)
                                          .arg(MobiPlugin::m_minor_version)
                                          .arg(MobiPlugin::m_build_version);
bool MobiPlugin::m_loaded = false;
const QString MobiPlugin::m_file_filter = "*.mobi";
const QString MobiPlugin::m_file_description = "Mobi Document";

MobiPlugin::MobiPlugin(QObject* parent) : QObject(parent) {}

MobiPlugin::MobiPlugin(Options* options, QObject* parent) :
    QObject(parent), m_options(options)
{
}

QString MobiPlugin::pluginGroup() const { return m_plugin_group; }

QString MobiPlugin::pluginName() const { return m_plugin_name; }

QString MobiPlugin::vendor() const { return m_vendor; }

bool MobiPlugin::loaded() const { return m_loaded; }

void MobiPlugin::setLoaded(bool loaded) { m_loaded = loaded; }

QString MobiPlugin::version() const { return m_version; }

int MobiPlugin::majorVersion() const { return m_major_version; }

int MobiPlugin::minorVersion() const { return m_minor_version; }

int MobiPlugin::buildVersion() const { return m_build_version; }

void MobiPlugin::buildMenu() {}

IEBookDocument* MobiPlugin::createDocument(QString path)
{
  m_document = new MobiDocument(this);
  m_document->openDocument(path);
  return m_document;
}

IEBookDocument* MobiPlugin::createCodeDocument() {}

QString MobiPlugin::fileFilter() { return m_file_filter; }

QString MobiPlugin::fileDescription() { return m_file_description; }
