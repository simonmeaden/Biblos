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
#include <QTextCursor>
#include <QTextDocument>
#include <QtPlugin>



class TocDisplayDocument : public QTextDocument
{
  Q_OBJECT
public:
  TocDisplayDocument(QObject* parent);
  ~TocDisplayDocument();

  void addLinePosition(int line_index, int start_position,
                       int length);
  void updateLinePosition(int line_index, QPair<int, int> data, int old_line_offset);
  QPair<int, int> linePosition(int index);
//  void setLinePosition(int line_index, TocLinePosition position);
//  void setEndOfListItems(QTextCursor& line_position);
  QTextCursor endOfListItems();
  int lineCount();
  void setTocString(QString toc_string);
  QString tocString();

protected:
  QString m_toc_string;
  QMap<int, QPair<int, int>> m_toc_positioning;
  QTextCursor m_end_of_listitems;
};

struct EPubNavPoint {
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

enum DocumentType { PLAINTEXT, HTML };
enum EBookDocumentType {
  UNSUPPORTED_TYPE,
  EPUB,
  MOBI,
  AZW,
  PDF,
};


#endif // COMMON_H
