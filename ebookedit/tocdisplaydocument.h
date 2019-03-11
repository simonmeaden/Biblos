#ifndef TOCDISPLAYDOCUMENT_H
#define TOCDISPLAYDOCUMENT_H

#include <QSharedPointer>
#include <QTextCursor>
#include <QTextDocument>

class TocDisplayDocument : public QTextDocument
{
  Q_OBJECT
public:
  TocDisplayDocument(QObject* parent);
  ~TocDisplayDocument();

  void addLinePosition(int line_index, int start_position, int length);
  void updateLinePosition(int line_index,
                          QPair<int, int> data,
                          int old_line_offset);
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

#endif // TOCDISPLAYDOCUMENT_H
