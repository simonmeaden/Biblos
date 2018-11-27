#ifndef LIBMOBI_H
#define LIBMOBI_H

#include "ebook_global.h"

#include <QTextDocument>

class EBOOKSHARED_EXPORT MobiDocument : public QTextDocument {
  Q_OBJECT

public:
  MobiDocument(QObject *parent = Q_NULLPTR);
  ~MobiDocument();

  bool loaded() { return m_loaded; }
  void openDocument(const QString &path);

private slots:
  void loadDocument();

protected:
  QString m_documentPath;
  bool m_loaded;
};

#endif // LIBMOBI_H
