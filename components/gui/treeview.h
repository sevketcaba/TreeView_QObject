#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QAction>
#include <QTreeView>
#include <QWidget>

class TreeModel;

class TreeView : public QWidget
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = nullptr);
    virtual ~TreeView() override;

    Q_INVOKABLE void setModel(TreeModel *model);
private:
    QTreeView *mTreeView;
    TreeModel *mTreeModel;

    QAction *mAddParent;
    QAction *mAddChild;
    QAction *mRemove;
};
#endif // TREEVIEW_H
