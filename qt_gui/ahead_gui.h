// Da el GUI kolo fe file wa7ed
// © 2025 Ahead Team - Ain Shams University / UEL - MIT License
#pragma once

#include <QColor>
#include <QFileDialog>
#include <QFrame>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QString>
#include <QStyleOptionFrame>
#include <QTimer>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../RadixTree.h"

// ============================================================
// THEME - El Alwan wel Design (Colors and Styling)
// ============================================================
namespace Colors {
inline const QString PRIMARY = "#6366f1"; // Indigo - el lawn el asasi
inline const QString PRIMARY_DARK = "#4f46e5";
inline const QString SECONDARY = "#ec4899"; // Pink
inline const QString SUCCESS = "#22c55e";   // Green - lel na7ga
inline const QString WARNING = "#f59e0b";   // Orange - lel ta7zir
inline const QString ERROR = "#ef4444";     // Red - lel akhtaa
inline const QString BG_DARK = "#0f172a";   // Asmar - background
inline const QString BG_MEDIUM = "#1e293b";
inline const QString BG_LIGHT = "#334155";
inline const QString TEXT_PRIMARY = "#f8fafc"; // Abyad - el nos el asasi
inline const QString TEXT_SECONDARY = "#94a3b8";
inline const QString BORDER = "#475569";
inline const QString ACCENT = "#38bdf8"; // Cyan - accent
} // namespace Colors

// ============================================================
// SUGGESTION STRUCT - Lel Autocomplete
// ============================================================
struct Suggestion {
  QString word;  // El kalema
  int frequency; // Kam mara etkarar
  bool operator<(const Suggestion &other) const {
    return frequency > other.frequency; // Tartib tanazoly (aktar awel)
  }
};

// ============================================================
// GHOST LINE EDIT - Input box w 7rkat
// ============================================================
class GhostLineEdit : public QLineEdit {
  Q_OBJECT
public:
  explicit GhostLineEdit(QWidget *parent = nullptr);
  void setGhostText(const QString &suggestion);
  void clearGhost();

signals:
  void textUpdated(const QString &text);
  void suggestionAccepted(const QString &word);

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  QString m_ghostText;      // El nos el ghost
  QString m_fullSuggestion; // El suggestion kamel
};

// ============================================================
// SNOWFLAKE WIDGET - Merry Christmas HoHoHO!
// ============================================================
class SnowflakeWidget : public QWidget {
  Q_OBJECT
public:
  explicit SnowflakeWidget(QWidget *parent = nullptr);
  void start();
  void setSnowflakeCount(int count);

protected:
  void paintEvent(QPaintEvent *event) override;
  void timerEvent(QTimerEvent *event) override;

private:
  struct Snowflake {
    float x, y, speed, size, opacity;
  };
  QVector<Snowflake> snowflakes; // Kol 7abbet talg
  int timerId = 0;
};

// ============================================================
// STATUS MANAGER - Yedeer el 7aga el hala
// ============================================================
class StatusManager : public QObject {
  Q_OBJECT
public:
  explicit StatusManager(QObject *parent = nullptr);
  void showSuccess(const QString &message);
  void showError(const QString &message);
  void showInfo(const QString &message);
  void clear();

signals:
  void statusChanged(const QString &message, const QString &color);

private:
  QTimer *clearTimer;
};

// ============================================================
// RADIX TREE MODEL - El Wrapper lel RadixTree
// ============================================================
class RadixTreeModel : public QObject {
  Q_OBJECT
public:
  explicit RadixTreeModel(QObject *parent = nullptr);
  ~RadixTreeModel();

  void insert(const QString &word);
  bool search(const QString &word) const;
  bool remove(const QString &word);
  void clear();
  bool isEmpty() const;
  QVector<Suggestion> getAutocompletions(const QString &prefix);
  bool loadFromFile(const QString &filePath);
  bool saveToFile(const QString &filePath);
  void incrementFrequency(const QString &word);
  void saveMetadata(const QString &filePath);
  void loadMetadata(const QString &filePath);

signals:
  void wordInserted(const QString &word);
  void wordRemoved(const QString &word);
  void treeCleared();
  void fileLoaded(int wordCount);
  void suggestionsReady(const QVector<Suggestion> &suggestions);

private:
  RadixTree *tree; // El shagara nafsaha
  QString currentFile;
  int wordCount = 0;
  void collectSuggestions(Node *node, const QString &current,
                          QVector<Suggestion> &results);
};

// ============================================================
// SIDE MENU - (collapsible w bta3)
// ============================================================
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
  QPushButton *addButton, *removeButton, *loadButton, *saveButton;
  QListWidget *wordList;
  QLabel *wordCountLabel, *currentFileLabel;
  QString currentFilePath;
  bool isCollapsed;
  int expandedWidth;
};

// ============================================================
// MAIN WINDOW - El Nafeza el Ra2isiya
// ============================================================
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
  void onSuggestionAccepted(const QString &word);
  void updateSuggestionsList(const QVector<Suggestion> &suggestions);
  void onSuggestionDoubleClicked(QListWidgetItem *item);
  void onAddWord(const QString &word);
  void onRemoveWord(const QString &word);
  void onLoadFile(const QString &filePath);
  void onSaveFile(const QString &filePath);
  void updateStatus(const QString &message, const QString &color);

private:
  void setupUI();
  void setupConnections();

  RadixTreeModel *model;
  StatusManager *statusManager;
  SnowflakeWidget *snowflakes;
  GhostLineEdit *searchBox;
  QListWidget *suggestionsList;
  QLabel *statusLabel, *logoLabel;
  SideMenu *sideMenu;
  QString currentFile;
  int wordCount = 0;
};
