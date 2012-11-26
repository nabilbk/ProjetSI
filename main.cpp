/** @mainpage */


#include <QtGui/QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.processEvents(QEventLoop::ExcludeUserInputEvents & QEventLoop::ExcludeSocketNotifiers);
    Controller control(app.applicationDirPath());

    return app.exec();

    return 0;
}
