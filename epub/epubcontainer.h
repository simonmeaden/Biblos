#ifndef EPUBCONTAINER_H
#define EPUBCONTAINER_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QSet>
#include <QVector>
#include <QDomNode>
#include <QMimeDatabase>

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

//class KZip;
//class KArchiveDirectory;
//class KArchiveFile;
class QXmlStreamReader;

struct EpubItem {
    QString path;
    QByteArray mimetype;
};

struct EpubPageReference {
    enum StandardType {
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

    static StandardType typeFromString(const QString &name);

    QString target;
    QString title;
};

class EPubContainer : public QObject
{
    Q_OBJECT
public:
    explicit EPubContainer(QObject *parent = Q_NULLPTR);
    ~EPubContainer();

    bool openFile(const QString path);
    EpubItem epubItem(const QString &id) const { return m_items.value(id); }
    QSharedPointer<QuaZipFile> zipFile(const QString &path);
    QImage image(const QString &id);
    QString metadata(const QString &key);
    void setMetadata(const QString &key, const QString &value);
    QStringList items() { return m_orderedItems; }
    QString standardPage(EpubPageReference::StandardType type) { return m_standardReferences.value(type).target; }

signals:
    void errorHappened(const QString &error);

public slots:

protected:

    bool parseMimetype(QStringList &files);
    bool parseContainer(QStringList &files);
    bool parseContentFile(const QString filepath);
    bool parseMetadataItem(const QDomNode &metadataNode);
    bool parseManifestItem(const QDomNode &manifestNodes, const QString currentFolder);
    bool parseSpineItem(const QDomNode &spineNode);
    bool parseGuideItem(const QDomNode &guideItem);

    static const QString MIMETYPE_FILE, METADATA_FOLDER, CONTAINER_FILE;

    const QuaZip *getFile(const QString &path);

    QuaZip *m_archive = Q_NULLPTR;

    QHash<QString, QString> m_metadata;

    QHash<QString, EpubItem> m_items;
    QStringList m_orderedItems;
    QSet<QString> m_unorderedItems;

    QHash<EpubPageReference::StandardType, EpubPageReference> m_standardReferences;
    QHash<QString, EpubPageReference> m_otherReferences;
    QMimeDatabase m_mimeDatabase;
};

#endif // EPUBCONTAINER_H
