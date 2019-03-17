#include "libraryframe.h"

#include <ebookcommon.h>

#include "libraryshelf.h"

LibraryFrame::LibraryFrame(Options options, QWidget* parent)
  : QWidget(parent)
  , m_options(options)
{
  initGui();
}

LibraryFrame::~LibraryFrame() {}

void LibraryFrame::initGui()
{
  QHBoxLayout* main_layout = new QHBoxLayout;
  setLayout(main_layout);

  m_stack = new QStackedWidget(this);
  main_layout->addWidget(m_stack);

  m_library_tree = new QTreeWidget(this);
  m_stack_tree = m_stack->addWidget(m_library_tree);

  m_library_shelf = new LibraryShelf(this);
  m_stack_shelf = m_stack->addWidget(m_library_shelf);

  setToTree();
}

void LibraryFrame::initTree()
{
}

void LibraryFrame::initShelf()
{
}

void LibraryFrame::readLibrary()
{
}

void LibraryFrame::setToShelf()
{
  m_stack->setCurrentIndex(m_stack_shelf);
}

void LibraryFrame::setToTree()
{
  m_stack->setCurrentIndex(m_stack_tree);
}

bool LibraryFrame::isTree()
{
  return (m_stack->currentIndex() == m_stack_tree);
}
