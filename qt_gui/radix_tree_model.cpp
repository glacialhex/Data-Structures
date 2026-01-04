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
    tree->insert(word.toStdString().c_str()); // Re-insert increments frequency
  }
}

QVector<RadixTreeModel::Suggestion>
RadixTreeModel::getAutocompletions(const QString &prefix) {
  QVector<Suggestion> results;

  if (prefix.isEmpty() || tree->empty()) {
    return results;
  }

  Node *current = tree->getRoot();
  QString builtString = "";
  int keyLen = prefix.length();
  int keyIndex = 0;

  while (keyIndex < keyLen) {
    QChar edgeChar = prefix[keyIndex];
    child *ch = current->children;
    child *found = nullptr;

    while (ch) {
      if (ch->firstChar == edgeChar.toLatin1()) {
        found = ch;
        break;
      }
      ch = ch->next;
    }

    if (!found) {
      emit suggestionsReady(results); // Clear ghost text
      return results;
    }

    QString nodeData = QString::fromStdString(found->node->data);
    int matched = 0;
    int nodeLen = nodeData.length();

    while (matched < nodeLen && keyIndex + matched < keyLen &&
           nodeData[matched] == prefix[keyIndex + matched]) {
      matched++;
    }

    if (keyIndex + matched < keyLen && matched < nodeLen) {
      emit suggestionsReady(results); // Clear ghost text
      return results;
    }

    builtString += nodeData;
    current = found->node;
    keyIndex += matched;
  }

  collectSuggestions(current, builtString, results);
  std::sort(results.begin(), results.end());

  emit suggestionsReady(results);
  return results;
}

void RadixTreeModel::collectSuggestions(Node *node, const QString &currentWord,
                                        QVector<Suggestion> &results) {
  if (!node)
    return;

  if (node->ended) {
    results.append({currentWord, node->frequency, node->timestamp});
  }

  child *ch = node->children;
  while (ch) {
    QString nextWord = currentWord + QString::fromStdString(ch->node->data);
    collectSuggestions(ch->node, nextWord, results);
    ch = ch->next;
  }
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
  // Save metadata format: word:frequency
  QVector<Suggestion> allSuggestions;
  Node *root = tree->getRoot();
  if (root) {
    child *ch = root->children;
    while (ch) {
      QString word = QString::fromStdString(ch->node->data);
      collectSuggestions(ch->node, word, allSuggestions);
      ch = ch->next;
    }
  }

  for (const auto &s : allSuggestions) {
    out << s.word << ":" << s.frequency << "\n";
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
        // Set frequency by re-inserting (freq-1) times
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
  QVector<Suggestion> suggestions;

  Node *root = tree->getRoot();
  if (root) {
    child *ch = root->children;
    while (ch) {
      QString word = QString::fromStdString(ch->node->data);
      collectSuggestions(ch->node, word, suggestions);
      ch = ch->next;
    }
  }

  for (const auto &s : suggestions) {
    words.append(s.word);
  }
  return words;
}

int RadixTreeModel::getWordCount() { return wordCount; }
