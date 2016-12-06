#include <QApplication>
#include "logger.h"
#include "window.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Logger::initLogger("/sdcard/Download/");

    Window window;
    window.show();
    return app.exec();
}
