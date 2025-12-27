// qt_gui/main_window.h - El Main Window Header
// El Main window class declaration - fiha kol el UI components
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>

class RadixTreeModel;
class SideMenu;
class SnowflakeWidget;
class GhostLineEdit;
class StatusManager;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void resizeEvent(QResizeEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

private slots:
  void onTextChanged(const QString &text);
  void onSuggestionClicked(QListWidgetItem *item);
  void onAddWord(const QString &word);
  void onRemoveWord(const QString &word);
  void onLoadFile(const QString &filePath);
  void onSaveFile(const QString &filePath);
  void onSearchEnterPressed();

private:
  void setupUI();
  void connectSignals();

  RadixTreeModel *model;
  SideMenu *sideMenu;
  SnowflakeWidget *snowflakes;
  GhostLineEdit *searchBox;
  StatusManager *statusManager;
  QListWidget *suggestionsList;
  QLabel *logoLabel;
  QString currentFile;
};
