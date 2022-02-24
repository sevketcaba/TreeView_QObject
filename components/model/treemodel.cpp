#include "treemodel.h"

#include <QDebug>

TreeModel::TreeModel(QObject *pRoot, QObject *parent) :
    QAbstractItemModel(parent)
  , mRoot(pRoot)
{
    if(mRoot == nullptr)
    {
        mRoot = new QObject(this);
        mRoot->setObjectName("Hidden Root Object");
    }
}

TreeModel::~TreeModel()
{

}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    if(role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return tr("Object Name");
        }

        if(section == 1)
        {
            return tr("Class Name");
        }

        if(section == 2)
        {
            return tr("Super Class Name");
        }
    }
    return QVariant();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    auto *tItem = objectFromIndex(index);
    if(tItem == nullptr)
    {
        return QVariant();
    }

    if(index.column() == 0)
    {
        if(role == Qt::CheckStateRole)
        {
            return static_cast<Qt::CheckState>(mObjectCheckStateMap.value(tItem, Qt::Unchecked));
        }

        if(role == Qt::DisplayRole || role == Qt::EditRole)
        {
            return tItem->objectName();
        }
    }
    else if(index.column() == 1)
    {
        if(role == Qt::DisplayRole)
        {
            return tItem->metaObject()->className();
        }
    }
    else if(index.column() == 2)
    {
        if(role == Qt::DisplayRole)
        {
            return tItem->metaObject()->superClass() != nullptr ? tItem->metaObject()->superClass()->className() : "";
        }
    }

    return QVariant();
}



Qt::CheckState TreeModel::calculateParentState(const QModelIndex &parentIdx)
{
    auto *parentItem = objectFromIndex(parentIdx);
    int childCount = parentItem->children().size();
    int checkedCount = 0;
    bool hasPartial = false;
    for(int i=0;i<childCount;++i) {
        auto *childItem = parentItem->children().at(i);
        auto childValue = mObjectCheckStateMap.value(childItem, Qt::Unchecked);
        if(childValue == Qt::Checked)
        {
            ++checkedCount;
        }
        else if(childValue == Qt::PartiallyChecked)
        {
            hasPartial = true;
        }
    }

    if(checkedCount == 0 && !hasPartial)
    {
        return Qt::Unchecked;
    }

    if(checkedCount == childCount)
    {
        return Qt::Checked;
    }

    return Qt::PartiallyChecked;
}

Qt::CheckState TreeModel::getCheckState(const QModelIndex &idx)
{
    if(!idx.isValid())
    {
        return Qt::Unchecked;
    }
    auto *item = objectFromIndex(idx);
    return mObjectCheckStateMap.value(item, Qt::Unchecked);
}

void TreeModel::updateChildren(const QModelIndex &idx, Qt::CheckState state)
{
    if(!idx.isValid())
    {
        return;
    }

    if(state == Qt::PartiallyChecked)
    {
        return;
    }

    auto *item = objectFromIndex(idx);
    auto childrenCount = item->children().size();
    for(int i=0;i<childrenCount;++i)
    {
        auto *child = item->children().at(i);
        mObjectCheckStateMap[child] = state;
        updateChildren(index(i, 0, idx), state);
    }
    if(childrenCount > 0)
    {
        emit dataChanged(index(0, 0, idx), index(childrenCount-1, 0, idx));
    }
}

void TreeModel::updateParentCheckState(const QModelIndex &parentIdx)
{
    if(!parentIdx.isValid())
    {
        return;
    }

    auto oldState = getCheckState(parentIdx);
    auto parentState = calculateParentState(parentIdx);
    if(parentState == oldState)
    {
        return;
    }

    /// update the parent next cycle
    QMetaObject::invokeMethod(this,
                              "setData",
                              Qt::QueuedConnection,
                              Q_ARG(QModelIndex,parentIdx),
                              Q_ARG(QVariant,parentState),
                              Q_ARG(int, Qt::CheckStateRole));
}

bool TreeModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
    auto *tItem = objectFromIndex(idx);
    if(tItem == nullptr)
    {
        return false;
    }

    if(idx.column() == 0)
    {
        if(role == Qt::CheckStateRole)
        {
            auto oldState = getCheckState(idx);
            if(value.toInt() == Qt::Checked)
            {
                if(oldState != Qt::Checked)
                {
                    mObjectCheckStateMap[tItem] = Qt::Checked;
                    updateChildren(idx, Qt::Checked);
                    updateParentCheckState(idx.parent());
                }
            }
            else if(value.toInt() == Qt::Unchecked)
            {
                if(oldState != Qt::Unchecked)
                {
                    mObjectCheckStateMap[tItem] = Qt::Unchecked;
                    updateChildren(idx, Qt::Unchecked);
                    updateParentCheckState(idx.parent());
                }
            }
            else
            {
                if(oldState != Qt::PartiallyChecked)
                {
                    mObjectCheckStateMap[tItem] = Qt::PartiallyChecked;
                    updateParentCheckState(idx.parent());
                }
            }
        }
        else if(role == Qt::DisplayRole || role == Qt::EditRole)
        {
            tItem->setObjectName(value.toString());
        }
    }
    else
    {
        // readonly
        return false;
    }

    emit dataChanged(idx, idx);
    return true;
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    auto *parentItem = objectFromIndex(parent);

    if(row < 0) {
        qCritical() << "[index] requested a negative row";
        return QModelIndex();
    }
    if(row >= parentItem->children().size())
    {
        qCritical() << "[index] requested a row that's beyond parent's children" << row << column << parent;
        return QModelIndex();
    }

    return createIndex(row, column, parentItem->children().at(row));
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
    {
        qCritical() << "[parent] requested a parent index of an invalid index";
        return QModelIndex();
    }

    auto *tItem = objectFromIndex(child);
    if(tItem == nullptr)
    {
        qCritical() << "[parent] requested a parent index of a null item of an index";
        return QModelIndex();
    }
    auto *tParentItem = tItem->parent();
    if(tParentItem == nullptr)
    {
        qCritical() << "[parent] requested a parent index of an index of which item has no parent";
        return QModelIndex();
    }
    auto *tGrandParent = tParentItem->parent();
    if(tGrandParent == nullptr) // this means this is a child of root
    {
        return QModelIndex();
    }

    return createIndex( static_cast<int>(tGrandParent->children().indexOf(tParentItem)), 0, tParentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    auto *item = objectFromIndex(parent);
    if(item == nullptr)
        return 0;

    return static_cast<int>(item->children().size());
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsTristate;
}


QObject *TreeModel::objectFromIndex(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return mRoot;
    }
    return reinterpret_cast<QObject*>(index.internalPointer());
}

QModelIndex TreeModel::indexFromObject(QObject *item) const
{
    if(item == nullptr)
    {
        return QModelIndex();
    }
    auto *pmi = mObjectIndexMap.value(item, nullptr);
    if(pmi == nullptr)
    {
        return QModelIndex();
    }
    if(!pmi->isValid())
    {
        return QModelIndex();
    }
    return index(pmi->row(), pmi->column(), pmi->parent());
}

void TreeModel::addObject(QObject *item, QObject *parent)
{
    // if parent is null, attach the item to the root object
    auto *tParent = parent == nullptr
            ? mRoot
            : parent;

    // get the index of the parent
    QModelIndex parentIndex = indexFromObject(tParent);

    addObjectAt(item, parentIndex);
}

void TreeModel::addObjectAt(QObject *item, const QModelIndex &parent)
{
    // get the next row; item does not know its parent yet
    int row = static_cast<int>(rowCount(parent));

    beginInsertRows(parent, row, row);

    auto *parentObj = reinterpret_cast<QObject*>(parent.internalPointer());
    if(parentObj == nullptr)
    {
        parentObj = mRoot;
    }
    item->setParent(parentObj);

    // add to the map
    mObjectIndexMap[item] = new QPersistentModelIndex(index(row, 0, parent));

    updateParentCheckState(parent);

    endInsertRows();
}

void TreeModel::removeObject(QObject *item)
{
    if(item == nullptr)
    {
        return;
    }
    auto idx = indexFromObject(item);
    removeObjectImpl(item, idx);
}

void TreeModel::removeObjectAt(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    auto *item = objectFromIndex(index);
    removeObjectImpl(item, index);
}

void TreeModel::setObject(QObject *root)
{
    beginResetModel();

    if(mRoot)
    {
        mRoot->setParent(nullptr);
        mRoot->deleteLater();
        mRoot = nullptr;
    }

    mRoot = root;

    endResetModel();
}

void TreeModel::removeObjectImpl(QObject *item, const QModelIndex &idx)
{
    connect(item, &QObject::destroyed, this, [=](){
        endRemoveRows();
    });

    auto parentIdx = idx.parent();
    beginRemoveRows(parentIdx, idx.row(), idx.row());

    // remove from check state map
    mObjectCheckStateMap.remove(item);

    // remove from map
    auto *pmi = mObjectIndexMap.take(item);
    delete pmi;

    // delete item
    item->setParent(nullptr);
    item->deleteLater();

    updateParentCheckState(parentIdx);
}
