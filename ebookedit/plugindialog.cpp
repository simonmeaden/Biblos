#include "plugindialog.h"

#include "basespellclass.h"

PluginDialog::PluginDialog(QWidget* parent)
  : QDialog(parent)
{
  setWindowTitle(tr("Installed Plugins"));

  QGridLayout* layout = new QGridLayout;
  setLayout(layout);
  m_filter_edit = new QLineEdit(tr("Filter"), this);
  layout->addWidget(m_filter_edit, 0, 0);

  m_model = new TreeModel(this);
  m_plugin_view = new QTreeView(this);
  m_plugin_view->setModel(m_model);
  layout->addWidget(m_plugin_view, 1, 0);
}

void
PluginDialog::addPlugin(IEBookInterface* plugin)
{
  QString plugin_group = plugin->pluginGroup();
  QString name = plugin->name();
  QString vendor = plugin->vendor();
  QString version = plugin->version();
  m_model->addModelData(plugin_group, name, version, vendor);
}

//= TreeItem ==================================================================

TreeItem::TreeItem(const QList<QVariant>& data, TreeItem* parent_item)
{
  m_parent_item = parent_item;
  m_item_data = data;
}

TreeItem::~TreeItem()
{
  qDeleteAll(m_child_items);
}

void
TreeItem::appendChild(TreeItem* item)
{
  m_child_items.append(item);
}

TreeItem*
TreeItem::child(int row)
{
  return m_child_items.value(row);
}

int
TreeItem::childCount()
{
  return m_child_items.count();
}

int
TreeItem::columnCount()
{
  return m_item_data.count();
}

QVariant
TreeItem::data(int column) const
{
  return m_item_data.value(column);
}

int
TreeItem::row() const
{
  if (m_parent_item)
    return m_parent_item->m_child_items.indexOf(const_cast<TreeItem*>(this));

  return 0;
}

TreeItem*
TreeItem::parentItem()
{
  return m_parent_item;
}

//= TreeModel =================================================================
TreeModel::TreeModel(QObject* parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> root_data;
  root_data << tr("Plugin Group") << tr("Name") << tr("Version")
            << tr("Vendor");
  root_item = new TreeItem(root_data);
}

TreeModel::~TreeModel()
{
  delete root_item;
}

QModelIndex
TreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  TreeItem* parentItem;

  if (!parent.isValid())
    parentItem = root_item;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  TreeItem* childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex
TreeModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem* parentItem = childItem->parentItem();

  if (parentItem == root_item)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int
TreeModel::rowCount(const QModelIndex& parent) const
{
  TreeItem* parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = root_item;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

int
TreeModel::columnCount(const QModelIndex& parent) const
{
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return root_item->columnCount();
}

QVariant
TreeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags
TreeModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

QVariant
TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return root_item->data(section);

  return QVariant();
}

void
TreeModel::addModelData(const QString& plugin_group_name,
                        const QString& plugin_name,
                        const QString& version,
                        const QString& vendor/*,
                        TreeItem* parent*/)
{
  QVariantList column_data;
  column_data << plugin_name << version << vendor;

  TreeItem* group = groupItem(plugin_group_name);
  TreeItem* item = new TreeItem(column_data, group);
  group->appendChild(item);
}

/*
 * Find existiong TreeIten with this name, otherwise create a new TreeItem with
 * the same name.
 */
TreeItem*
TreeModel::groupItem(QString plugin_group_name)
{
  for (int i = 0; i < root_item->childCount(); i++) {
    TreeItem* child = root_item->child(i);
    QString group_name = child->data(0).value<QString>();
    if (group_name == plugin_group_name) {
      return child;
    }
  }

  QVariantList list;
  list << plugin_group_name;
  TreeItem* group = new TreeItem(list, root_item);
  return group;
}
