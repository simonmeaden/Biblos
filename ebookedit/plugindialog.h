#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QAbstractItemModel>


class IEBookInterface;

typedef QList<QVariant> QVariantList;

class TreeItem
{
public:
  explicit TreeItem(const QList<QVariant>& data,
                    TreeItem* parent_item = Q_NULLPTR);
  ~TreeItem();

  void appendChild(TreeItem* item);
  TreeItem* child(int row);
  int childCount();
  int columnCount();
  QVariant data(int column) const;
  int row() const;
  TreeItem* parentItem();

protected:
  QList<TreeItem*> m_child_items;
  QList<QVariant> m_item_data;
  TreeItem* m_parent_item;
};

class TreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit TreeModel(QObject* parent = Q_NULLPTR);
  ~TreeModel() override;

  QVariant data(const QModelIndex& index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row,
                    int column,
                    const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  void addModelData(const QString &plugin_group,
                    const QString &name,
                    const QString &version,
                    const QString &vendor);

private:
  TreeItem* root_item;

  TreeItem* groupItem(QString group);
};


class PluginDialog : public QDialog
{
  Q_OBJECT
public:
  explicit PluginDialog(QWidget *parent = nullptr);

  void addPlugin(IEBookInterface*);

protected:
  QTreeView *m_plugin_view;
  QLineEdit *m_filter_edit;
  TreeModel *m_model;

};

#endif // PLUGINDIALOG_H
