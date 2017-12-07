#include "mainwindow.h"
#include "mainmobile.h"

#include <QApplication>

#include <ctime>

int main(int argc, char *argv[])
{
#if !defined(Q_OS_ANDROID) and !defined(Q_OS_IOS)
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QApplication a(argc, argv);
  std::srand(std::time(0));

#if 0//!defined(Q_OS_ANDROID) and !defined(Q_OS_IOS)
  MainWindow w;
  w.show();
#else
  MainMobile w;
  w.show();
#endif

  return a.exec();
}
