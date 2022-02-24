#include <treeview.h>
#include <sample/sample.h>
#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include <treemodel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TreeView view;
    view.setMinimumSize(480,640);
    TreeModel model(nullptr, &view);
    Sample sample(&model);
    model.setObject(&sample);
    view.setModel(&model);
    view.show();

    return a.exec();
}
