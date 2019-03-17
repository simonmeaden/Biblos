#include "plugindialog.h"

//#include <qlogger/qlogger.h>
// using namespace qlogger;

#include "iplugininterface.h"

PluginDialog::PluginDialog(QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(tr("Installed Plugins"));

  QGridLayout* layout = new QGridLayout;
  setLayout(layout);
  m_filter_edit = new QLineEdit(tr("Filter"), this);
  layout->addWidget(m_filter_edit, 0, 0);

  QStringList headers;
  headers << tr("Group") << tr("Name") << tr("Loaded") << tr("Version")
          << tr("Vendor");

  m_plugin_widget = new QTreeWidget(this);
  m_plugin_widget->setColumnCount(5);
  m_plugin_widget->setHeaderLabels(headers);
  m_plugin_widget->header()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  connect(m_plugin_widget,
          &QTreeWidget::itemClicked,
          this,
          &PluginDialog::itemWasClicked);

  layout->addWidget(m_plugin_widget, 1, 0);

  setGeometry(geometry().x(), geometry().y(), 600, 300);
}

QTreeWidgetItem*
PluginDialog::addTreeRoot(QString group)
{
  QTreeWidgetItem* tree_item = new QTreeWidgetItem(m_plugin_widget);
  tree_item->setText(0, group);
  tree_item->setExpanded(true);
  return tree_item;
}

void
PluginDialog::addTreeChild(QTreeWidgetItem* parent,
                           QString name,
                           bool loaded,
                           QString version,
                           QString vendor)
{
  QTreeWidgetItem* tree_item = new QTreeWidgetItem();
  tree_item->setText(1, name);
  if (loaded)
    tree_item->setCheckState(2, Qt::Checked);
  else
    tree_item->setCheckState(2, Qt::Unchecked);
  tree_item->setText(3, version);
  tree_item->setText(4, vendor);
  parent->addChild(tree_item);
}

void
PluginDialog::itemWasClicked(QTreeWidgetItem* item, int column)
{
  QLoggingCategory category("biblos.plugin.dialog");
  // TODO - handle loaded changes
  if (column == 2) {
    qCDebug(category)
      << QString("plugin item clicked : value %2 : column %1 : state %3")
           .arg(column)
           .arg(item->text(1))
           .arg(item->checkState(2) == Qt::Checked);
    emit loadedWasChanged(item->text(1), item->checkState(2) == Qt::Checked);
  }
}

void
PluginDialog::addPlugin(IPluginInterface* plugin)
{
  QString plugin_group = plugin->pluginGroup();
  QString name = plugin->pluginName();
  QString vendor = plugin->vendor();
  QString version = plugin->version();
  bool loaded = plugin->loaded();

  QTreeWidgetItem* parent_item;
  QList<QTreeWidgetItem*> top_items =
    m_plugin_widget->findItems(plugin_group, Qt::MatchFixedString);
  if (top_items.isEmpty()) {
    parent_item = addTreeRoot(plugin_group);
    addTreeChild(parent_item, name, loaded, version, vendor);
  } else {
    parent_item = top_items.at(0);
    addTreeChild(parent_item, name, loaded, version, vendor);
  }
}
