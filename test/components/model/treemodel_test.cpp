#include <QTest>
#include <treemodel.h>
#include <QAbstractItemModelTester>
#include <QDebug>
#include <QSignalSpy>

class TreeModelTest : public QObject
{
    Q_OBJECT
public:
    explicit TreeModelTest(QObject *parent = nullptr) :
        QObject(parent)
      , mData(nullptr)
    {

    }

private:
    void resetData()
    {
        mData = new QObject;
        {
            QObject *c_0 = new QObject(mData);
            {
                new QObject(c_0);
                new QObject(c_0);
            }
            QObject *c_1 = new QObject(mData);
            {
                new QObject(c_1);
                new QObject(c_1);
            }
        }
    }
    QObject *mData;
private slots:
    void initTestCase() {
        resetData();
    }
    void cleanupTestCase() {
        mData->deleteLater();
        mData = nullptr;
    }

    void aimTestEmptyModel()
    {
        TreeModel model;
        QAbstractItemModelTester tester(&model);
    }
    void aimTestSingleObjectModel()
    {
        QObject object;
        TreeModel model(&object);
        QAbstractItemModelTester tester(&model);
    }
    void aimTestTreeModel()
    {
        TreeModel model(mData);
        QAbstractItemModelTester tester(&model);
    }
    void aimTestTreeModelAddRemoveInSequence()
    {
        TreeModel model(mData); // mdata already has 2 children
        QSignalSpy spy(&model, SIGNAL(itemRemovalDone()));
        QVERIFY(spy.isValid());
        QPointer<QObject> child = new QObject;
        model.addObject(child, mData);
        QVERIFY(mData->children().size() == 3);
        {
            QAbstractItemModelTester tester(&model);
        }
        QPointer<QObject> grandChild = new QObject;
        model.addObject(grandChild, child);
        QVERIFY(mData->children().size() == 3); // root item count should be the same
        QVERIFY(child->children().size() == 1); // child has one child now
        {
            QAbstractItemModelTester tester(&model); // sanity check
        }
        model.removeObject(grandChild);
        QVERIFY(spy.wait(1000));
        QCOMPARE(spy.count(), 1);
        QVERIFY(grandChild.isNull()); // grandchild needs to be deleted
        QVERIFY(mData->children().size() == 3); // root item count should be the same
        QVERIFY(child->children().size() == 0); // child has no children now
        {
            QAbstractItemModelTester tester(&model); // sanity check
        }
        model.removeObject(child);
        QVERIFY(spy.wait(1000));
        QCOMPARE(spy.count(), 2);
        QVERIFY(mData->children().size() == 2); // root item count should be 2
        QVERIFY(child.isNull()); // child needs to be deleted
        {
            QAbstractItemModelTester tester(&model); // sanity check
        }
    }

    void aimTestTreeModelRemoveChildWithChildren()
    {
        TreeModel model(mData); // mdata already has 2 children
        QSignalSpy spy(&model, SIGNAL(itemRemovalDone()));
        {
            QPointer<QObject> child = new QObject;
            model.addObject(child, mData);
            QPointer<QObject> grandChild = new QObject;
            model.addObject(grandChild, child);
            model.removeObject(child);
            QVERIFY(spy.wait(1000));
            QCOMPARE(spy.count(), 1);
            QVERIFY(mData->children().size() == 2); // root item count should be 2
            QVERIFY(grandChild.isNull()); // grandchild needs to be deleted
            QVERIFY(child.isNull()); // child needs to be deleted
            {
                QAbstractItemModelTester tester(&model); // sanity check
            }
        }
    }
};

QTEST_MAIN(TreeModelTest)
#include "treemodel_test.moc"
