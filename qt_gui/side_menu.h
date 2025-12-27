// qt_gui/side_menu.h - El Side Menu Header
// El Side panel bta3 el app - fiha el buttons wel word list
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include "theme.h"
#include <QFileDialog>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

class SideMenu : public QFrame {
  Q_OBJECT

public:
  explicit SideMenu(QWidget *parent = nullptr);

signals:
  void addWordRequested(const QString &word);
  void removeWordRequested(const QString &word);
  void loadFileRequested(const QString &filePath);
  void saveFileRequested(const QString &filePath);
  void wordSelected(const QString &word);

public slots:
  void updateWordList(const QStringList &words);
  void setWordCount(int count);
  void setCurrentFile(const QString &filename);
  void toggleCollapse();

private slots:
  void onAddClicked();
  void onRemoveClicked();
  void onLoadClicked();
  void onSaveClicked();

private:
  void setupUI();
  QPushButton *createMenuButton(const QString &icon, const QString &text,
                                const QString &color);

  QVBoxLayout *mainLayout;
  QWidget *contentWidget;
  QPushButton *toggleButton;
  QLineEdit *wordInput;
  QPushButton *addButton;
  QPushButton *removeButton;
  QPushButton *loadButton;
  QPushButton *saveButton;
  QListWidget *wordList;
  QLabel *wordCountLabel;
  QLabel *currentFileLabel;
  QString currentFilePath;
  bool isCollapsed;
  int expandedWidth;
};
