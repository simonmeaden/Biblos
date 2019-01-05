#ifndef METADATAEDITOR_H
#define METADATAEDITOR_H

#include <QtWidgets>

#include "ebookcommon.h"
#include "options.h"
#include "library.h"

class MetadataEditor : public QWidget
{
  Q_OBJECT
public:
  explicit MetadataEditor(Options *options, QWidget *parent = nullptr);

  void addAuthors(SharedAuthorList authorlist);
signals:

public slots:

protected:
  Options *m_options;
  QLineEdit *m_title;

  QList<QLineEdit*> m_authors;
  QVBoxLayout *authorLayout;

  void initGui();
};

#endif // METADATAEDITOR_H
