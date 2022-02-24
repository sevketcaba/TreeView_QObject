#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <common.h>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *pRoot = nullptr, QObject *parent = nullptr);
    virtual ~TreeModel() override;

    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &idx, int role) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QModelIndex parent(const QModelIndex &child) const override;
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void addObject(QObject *item, QObject *parent = nullptr);
    Q_INVOKABLE void addObjectAt(QObject *item, const QModelIndex &parent);
    Q_INVOKABLE void removeObject(QObject *item);
    Q_INVOKABLE void removeObjectAt(const QModelIndex &index);

    Q_INVOKABLE void setObject(QObject *root);
protected:
    QObject *objectFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromObject(QObject *item) const;

private:
    QObject *mRoot;
    QMap<QObject*,Qt::CheckState> mObjectCheckStateMap;
    QMap<QObject*,QPersistentModelIndex *> mObjectIndexMap;

    void updateParentCheckState(const QModelIndex &parentIdx);
    Qt::CheckState calculateParentState(const QModelIndex &parentIdx);
    Qt::CheckState getCheckState(const QModelIndex &idx);
    void updateChildren(const QModelIndex &idx, Qt::CheckState state);

    void removeObjectImpl(QObject *item, const QModelIndex &idx);
};

#endif // TREEMODEL_H

