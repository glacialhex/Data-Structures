// qt_gui/main_window.h - El Main Window Header
// El Main window class declaration - fiha kol el UI components
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include "radix_tree_model.h"
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QRadioButton>
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

  // Sort mode for suggestions
  RadixTreeModel::SortMode sortMode;
  QRadioButton *sortByFrequency;
  QRadioButton *sortByRecent;
};
