#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QAbstractItemModel>
#include <QDialog>
#include <QtWidgets>

class IPluginInterface;
class PluginModel;

class PluginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PluginDialog(QWidget *parent = nullptr);

    void addPlugin(IPluginInterface *);

signals:
    void loadedWasChanged(QString name, bool state);

protected:
    QTreeWidget *m_plugin_widget;
    QLineEdit *m_filter_edit;
    //    PluginModel *m_model;

    QTreeWidgetItem *addTreeRoot(QString group);
    void addTreeChild(QTreeWidgetItem *parent, QString name, bool loaded,
                      QString verion, QString vendor);
    void itemWasClicked(QTreeWidgetItem *item, int column);
};

#endif // PLUGINDIALOG_H
