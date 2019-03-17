#ifndef LIBRARYFRAME_H
#define LIBRARYFRAME_H

#include <QFrame>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QWidget>

#include "options.h"

class LibraryShelf;

class LibraryFrame : public QWidget
{
  Q_OBJECT
public:
  explicit LibraryFrame(Options options, QWidget* parent = nullptr);
  ~LibraryFrame();

  void setToShelf();
  void setToTree();
  bool isTree();

protected:
  Options m_options;
  QStackedWidget* m_stack;
  QTreeWidget* m_library_tree;
  LibraryShelf* m_library_shelf;
  int m_stack_tree, m_stack_shelf;

  void initGui();
  void initTree();
  void initShelf();
  void readLibrary();
};

#endif // LIBRARYFRAME_H
