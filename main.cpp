#include <QtGui/QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.processEvents(QEventLoop::ExcludeUserInputEvents & QEventLoop::ExcludeSocketNotifiers);
    Controller c(a.applicationDirPath());

    return a.exec();

    return 0;
}
