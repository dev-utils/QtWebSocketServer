#include <QCoreApplication>
#include "WebServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebServer server(1234);
    return a.exec();
}
