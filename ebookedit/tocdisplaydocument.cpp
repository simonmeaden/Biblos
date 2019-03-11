#include "tocdisplaydocument.h"

QTextCursor
TocDisplayDocument::endOfListItems()
{
  return m_end_of_listitems;
}

int
TocDisplayDocument::lineCount()
{
  return m_toc_positioning.size();
}

void
TocDisplayDocument::setTocString(QString toc_string)
{
  m_toc_string = toc_string;
  setHtml(toc_string);
}

QString
TocDisplayDocument::tocString()
{
  return m_toc_string;
}

void
TocDisplayDocument::updateLinePosition(int line_index,
                                       QPair<int, int> data,
                                       int old_line_offset)
{
  m_toc_positioning[line_index] = data;
  // update remaining start positions for change in modified text length.
  if (line_index < m_toc_positioning.size() - 1) {
    for (int i = line_index + 1; i < m_toc_positioning.size(); i++) {
      QPair<int, int> position = m_toc_positioning.value(i);
      position.first += old_line_offset; // this could be a negative value.
    }
  }
}
