// qt_gui/radix_tree_model.cpp - El RadixTree Model Implementation
// El Qt model bey-wrap el RadixTree - kol el operations hena
// © 2025 Ahead Team - Ain Shams / UEL

#include "radix_tree_model.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <algorithm>

RadixTreeModel::RadixTreeModel(QObject *parent)
    : QObject(parent), wordCount(0) {
  tree = new RadixTree();
}

RadixTreeModel::~RadixTreeModel() { delete tree; }

void RadixTreeModel::insert(const QString &word) {
  tree->insert(word.toStdString().c_str());
  wordCount++;
}

bool RadixTreeModel::search(const QString &word) {
  return tree->search(word.toStdString().c_str());
}

void RadixTreeModel::remove(const QString &word) {
  tree->deleteWord(word.toStdString().c_str());
  wordCount--;
}

void RadixTreeModel::incrementFrequency(const QString &word) {
  if (search(word)) {
    tree->insert(word.toStdString().c_str()); // re-insert bezawed el frequency
  }
}

QVector<RadixTreeModel::Suggestion>
RadixTreeModel::getAutocompletions(const QString &prefix) {
  QVector<Suggestion> results;

  if (prefix.isEmpty() || tree->empty()) {
    emit suggestionsReady(results);
    return results;
  }

  // na2el el call lel RadixTree getSuggestions
  std::vector<RadixTree::Suggestion> treeSuggestions =
      tree->getSuggestions(prefix.toStdString().c_str());

  // 7awel le Qt types
  for (const auto &s : treeSuggestions) {
    results.append({QString::fromStdString(s.word), s.frequency, s.timestamp});
  }

  std::sort(results.begin(), results.end());
  emit suggestionsReady(results);
  return results;
}

void RadixTreeModel::loadFromFile(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  delete tree;
  tree = new RadixTree();
  wordCount = 0;
  currentFile = filePath;

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine().trimmed().toLower();
    if (!line.isEmpty()) {
      insert(line);
    }
  }
  file.close();

  loadMetadata(filePath);
}

void RadixTreeModel::saveToFile(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  QStringList words = getAllWords();
  for (const QString &word : words) {
    out << word << "\n";
  }
  file.close();
}

void RadixTreeModel::saveMetadata(const QString &filePath) {
  QString metaPath = filePath + ".meta";
  QFile file(metaPath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  // besta5dem getAllSuggestions men el RadixTree
  std::vector<RadixTree::Suggestion> allSuggestions = tree->getAllSuggestions();

  for (const auto &s : allSuggestions) {
    out << QString::fromStdString(s.word) << ":" << s.frequency << "\n";
  }
  file.close();
}

void RadixTreeModel::loadMetadata(const QString &filePath) {
  QString metaPath = filePath + ".meta";
  QFile file(metaPath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine().trimmed();
    if (line.contains(':')) {
      QStringList parts = line.split(':');
      if (parts.size() >= 2) {
        QString word = parts[0];
        int freq = parts[1].toInt();
        // zawd el frequency b re-insert (freq-1) mara
        for (int i = 1; i < freq; ++i) {
          tree->insert(word.toStdString().c_str());
        }
      }
    }
  }
  file.close();
}

QStringList RadixTreeModel::getAllWords() {
  QStringList words;

  // besta5dem getAllSuggestions men el RadixTree
  std::vector<RadixTree::Suggestion> suggestions = tree->getAllSuggestions();

  for (const auto &s : suggestions) {
    words.append(QString::fromStdString(s.word));
  }
  return words;
}

int RadixTreeModel::getWordCount() { return wordCount; }
