#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // MainWindow 생성 및 표시
  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}
