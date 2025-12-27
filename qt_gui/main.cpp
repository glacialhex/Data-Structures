// qt_gui/main.cpp - Entry Point
// El Application entry point - bey7ot el window we yeshaghalha
// © 2025 Ahead Team - Ain Shams University / UEL - MIT License

#include "main_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setApplicationName("Ahead");
  app.setOrganizationName("RadixTree");

  MainWindow window;
  window.show();

  return app.exec();
}
