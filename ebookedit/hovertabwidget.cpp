#include "hovertabwidget.h"

HoverTabWidget::HoverTabWidget(QWidget* parent)
  : QTabWidget(parent)
{
  HoverTabBar* tabbar = new HoverTabBar(this);
  tabbar->setMouseTracking(true);
  setTabBar(tabbar);
  connect(
    tabbar, &HoverTabBar::tabEntered, this, &HoverTabWidget::tabWasEntered);
  connect(tabbar, &HoverTabBar::tabExited, this, &HoverTabWidget::tabExited);
}

HoverTabWidget::~HoverTabWidget() {}

/*!
 * \brief HoverTabWidget::addTab
 * \param widget
 * \param text
 * \return
 */
int
HoverTabWidget::addTab(QWidget* widget, const QString& text)
{
  int index = QTabWidget::addTab(widget, text);
  return index;
}

/*!
 * \brief HoverTabWidget::addTab
 * \param widget
 * \param text
 * \param data
 * \return
 */
int
HoverTabWidget::addTab(QWidget* widget, const QString& text, QVariant data)
{
  int index = QTabWidget::addTab(widget, text);
  tabBar()->setTabData(index, data);
  return index;
}

/*!
 * \brief Takes the tabEntered signal from the HoverTabBar and adds the Qvariant
 * data object from the tabBar if it exists, otherwise sends an empty QVariant.
 *
 * \param index the integer tab index.
 */
void
HoverTabWidget::tabWasEntered(int index, QPoint pos)
{
  QVariant v = tabBar()->tabData(index);
  if (v.isValid()) {
    emit tabEntered(index, pos, v);
  } else {
    emit tabEntered(index, pos, QVariant());
  }
}

HoverTabBar::HoverTabBar(QWidget* parent)
  : QTabBar(parent)
{
  setAttribute(Qt::WA_Hover);
}

HoverTabBar::~HoverTabBar() {}

void
HoverTabBar::hoverEnter(QHoverEvent* event)
{
  m_hoverposition = event->pos();
  m_hoverindex = tabAt(m_hoverposition);
  QVariant v = tabData(m_hoverindex);
  emit tabEntered(m_hoverindex, m_hoverposition);
}

void
HoverTabBar::hoverLeave(QHoverEvent* /*event*/)
{
  emit tabExited(m_hoverindex);
  m_hoverindex = -1;
}

void
HoverTabBar::hoverMove(QHoverEvent* event)
{
  QPoint pos = event->pos();
  int index = tabAt(pos);
  if (index == -1) {
    emit tabExited(m_hoverindex);
    m_hoverindex = -1;
    return;
  }
  if (index != m_hoverindex) {
    emit tabExited(m_hoverindex);
    m_hoverindex = index;
    m_hoverposition = pos;
    emit tabEntered(m_hoverindex, m_hoverposition);
  }
}

bool
HoverTabBar::event(QEvent* e)
{
  
  switch (e->type()) {
    case QEvent::HoverEnter:
      hoverEnter(static_cast<QHoverEvent*>(e));
      return true;
    case QEvent::HoverLeave:
      hoverLeave(static_cast<QHoverEvent*>(e));
      return true;
    case QEvent::HoverMove:
      hoverMove(static_cast<QHoverEvent*>(e));
      return true;
    default:
      break;
  }
  return QWidget::event(e);
}
