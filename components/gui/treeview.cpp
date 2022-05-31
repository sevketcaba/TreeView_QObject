#include "treeview.h"
#include <treemodel.h>
#include <QFont>
#include <QToolBar>
#include <QVBoxLayout>

TreeView::TreeView(QWidget *parent)
    : QWidget(parent)
    , mTreeView(new QTreeView)
    , mTreeModel(nullptr)
{
    auto *toolBar = new QToolBar;
    mAddParent = toolBar->addAction(style()->standardIcon(QStyle::SP_DirLinkIcon), "Add Parent");
    mAddParent->setEnabled(false);
    mAddChild = toolBar->addAction(style()->standardIcon(QStyle::SP_FileLinkIcon), "Add Child");
    mAddChild->setEnabled(false);
    mRemove = toolBar->addAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Add Child");
    mRemove->setEnabled(false);

    mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    mTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto *vb = new QVBoxLayout;
    {
        vb->addWidget(toolBar);
        vb->addWidget(mTreeView);
    }
    setLayout(vb);

    connect(mAddParent, &QAction::triggered, this, [=](){
        auto *object = new QObject;
        object->setObjectName(QString("%1").arg(mTreeModel->rowCount()));
        mTreeModel->addObject(object);
    });
    connect(mAddChild, &QAction::triggered, this, [=](){
        auto parentIndex = mTreeView->selectionModel()->currentIndex();
        auto parentObjectName = mTreeModel->data(parentIndex, Qt::DisplayRole).toString();
        auto *object = new QObject;
        object->setObjectName(QString("%1_%2").arg(parentObjectName).arg(mTreeModel->rowCount(parentIndex)));
        mTreeModel->addObjectAt(object, parentIndex);
        mTreeView->expand(parentIndex);
    });
    connect(mRemove, &QAction::triggered, this, [=](){
        auto currentIndex = mTreeView->selectionModel()->currentIndex();
        mTreeModel->removeObjectAt(currentIndex);
    });
}

TreeView::~TreeView()
{
}

const TreeModel *TreeView::model() const
{
    return mTreeModel;
}

void TreeView::setModel(TreeModel *model)
{
    mTreeModel = model;
    mTreeView->setModel(model);
    if(model == nullptr)
    {
        mAddParent->setEnabled(false);
        return;
    }
    mAddParent->setEnabled(true);
    auto *selectionModel = mTreeView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::currentRowChanged, this, [=](const QModelIndex &current, const QModelIndex &){
        mAddChild->setEnabled(current.isValid());
        mRemove->setEnabled(current.isValid());
    });
    /// NOTE: WORKAROUND : selectionModel has the first row selected, but it's not visible in the view
    if(model->rowCount() > 0)
    {
        selectionModel->setCurrentIndex(model->index(0,0), QItemSelectionModel::ClearAndSelect);
    }
    else
    {
        selectionModel->setCurrentIndex(QModelIndex(), QItemSelectionModel::ClearAndSelect);
    }
    mTreeView->expandToDepth(1);
}

