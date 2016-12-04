#include <QApplication>
#include "logger.h"
#include "window.h"
#if(G_TEST == 1)
#include <gtest/gtest.h>
#endif

int main(int argc, char *argv[]){
#if(G_TEST == 1)
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
    QApplication app(argc, argv);
    Logger::initLogger("/sdcard/Download/");

    Window window;
    window.show();
    return app.exec();
}
