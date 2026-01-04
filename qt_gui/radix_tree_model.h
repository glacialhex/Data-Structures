// qt_gui/radix_tree_model.h - El RadixTree Model Header
// El Qt wrapper lel RadixTree - bey7awel el C++ tree le Qt model
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include "../RadixTree.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

class RadixTreeModel : public QObject {
  Q_OBJECT

public:
  // Sort mode for suggestions
  enum SortMode { ByFrequency, ByRecent };

  struct Suggestion {
    QString word;
    int frequency;
    long long timestamp; // For recent searches feature

    // Default sort by frequency
    bool operator<(const Suggestion &other) const {
      return frequency > other.frequency; // Descending
    }
  };

  explicit RadixTreeModel(QObject *parent = nullptr);
  ~RadixTreeModel();

  void insert(const QString &word);
  bool search(const QString &word);
  void remove(const QString &word);
  QVector<Suggestion> getAutocompletions(const QString &prefix);
  void incrementFrequency(const QString &word);

  void loadFromFile(const QString &filePath);
  void saveToFile(const QString &filePath);
  void saveMetadata(const QString &filePath);
  void loadMetadata(const QString &filePath);

  QStringList getAllWords();
  int getWordCount();

signals:
  void suggestionsReady(const QVector<Suggestion> &suggestions);

private:
  RadixTree *tree;
  int wordCount;
  QString currentFile;
};
