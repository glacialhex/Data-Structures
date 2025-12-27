// qt_gui/main_window.cpp - El Main Window Implementation
// El Main window bta3 el app - fiha el UI wel logic
// © 2025 Ahead Team - Ain Shams / UEL

#include "main_window.h"
#include "ghost_line_edit.h"
#include "radix_tree_model.h"
#include "side_menu.h"
#include "snowflake_widget.h"
#include "status_manager.h"
#include "theme.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QResizeEvent>
#include <QStatusBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Ahead - Autocomplete Engine");
  setMinimumSize(1000, 700);

  // El stylesheet lel whole window - DARKER background
  setStyleSheet(QString("QMainWindow { background-color: #0a0e14; }"));

  // El model bta3 el RadixTree
  model = new RadixTreeModel(this);

  // El snowflakes effect - el talg
  snowflakes = new SnowflakeWidget(this);
  snowflakes->start();

  // Status manager
  statusManager = new StatusManager(this);

  setupUI();
  connectSignals();

  // Resize el snowflakes
  snowflakes->setGeometry(0, 0, width(), height());
  snowflakes->lower();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
  QWidget *centralWidget = new QWidget();
  setCentralWidget(centralWidget);

  // El main layout
  QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // El side menu
  sideMenu = new SideMenu();
  mainLayout->addWidget(sideMenu);

  // El content area
  QWidget *contentWidget = new QWidget();
  QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
  contentLayout->setContentsMargins(40, 30, 40, 30);
  contentLayout->setSpacing(20);

  // Header with logos
  QHBoxLayout *headerLayout = new QHBoxLayout();

  // Ain Shams Logo
  QLabel *ainShamsLogo = new QLabel();
  ainShamsLogo->setFixedSize(120, 120);
  ainShamsLogo->setScaledContents(true);
  QPixmap ainShamsPix(":/assets/logo_ain_shams.png");
  ainShamsLogo->setPixmap(ainShamsPix);
  ainShamsLogo->setStyleSheet("background: transparent;");

  // AHEAD Logo
  logoLabel = new QLabel();
  logoLabel->setFixedSize(350, 350);
  logoLabel->setScaledContents(true);
  QPixmap aheadPix(":/assets/logo_ahead.png");
  logoLabel->setPixmap(aheadPix);
  logoLabel->setAlignment(Qt::AlignCenter);
  logoLabel->setStyleSheet("background: transparent;");

  // UEL Logo
  QLabel *uelLogo = new QLabel();
  uelLogo->setFixedSize(220, 220);
  uelLogo->setScaledContents(true);
  QPixmap uelPix(":/assets/logo_uel.png");
  uelLogo->setPixmap(uelPix);
  uelLogo->setStyleSheet("background: transparent;");

  headerLayout->addWidget(ainShamsLogo, 0, Qt::AlignTop);
  headerLayout->addStretch();
  headerLayout->addWidget(logoLabel, 0, Qt::AlignTop);
  headerLayout->addStretch();
  headerLayout->addWidget(uelLogo, 0, Qt::AlignTop);

  contentLayout->addLayout(headerLayout);

  // Subtitle
  QLabel *subtitleLabel =
      new QLabel("Lightning-Fast Autocomplete • Powered by RadixTree");
  subtitleLabel->setAlignment(Qt::AlignCenter);
  subtitleLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 13px; letter-spacing: 2px; }")
          .arg(Colors::TEXT_SECONDARY));
  contentLayout->addWidget(subtitleLabel);

  // Search box - el search bar
  QHBoxLayout *searchLayout = new QHBoxLayout();
  searchLayout->addStretch();

  searchBox = new GhostLineEdit();
  searchBox->setFixedWidth(500);
  searchLayout->addWidget(searchBox);

  searchLayout->addStretch();
  contentLayout->addLayout(searchLayout);

  // Hint label
  QLabel *hintLabel = new QLabel("Press Space or → to accept suggestion\nPress "
                                 "Enter or ⏎ to confirm word choice");
  hintLabel->setAlignment(Qt::AlignCenter);
  hintLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 11px; font-style: italic; }")
          .arg(Colors::TEXT_SECONDARY));
  contentLayout->addWidget(hintLabel);

  // Suggestions label
  QLabel *suggestionsLabel = new QLabel("Suggestions");
  suggestionsLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 16px; font-weight: bold; }")
          .arg(Colors::TEXT_PRIMARY));
  contentLayout->addWidget(suggestionsLabel);

  // Suggestions list - el list bta3t el suggestions
  suggestionsList = new QListWidget();
  suggestionsList->setStyleSheet(
      QString(
          "QListWidget { background-color: %1; border: 1px solid %2; "
          "border-radius: 10px; padding: 10px; }"
          "QListWidget::item { color: %3; padding: 10px; border-radius: 5px; "
          "font-size: 15px; }"
          "QListWidget::item:hover { background-color: %4; }"
          "QListWidget::item:selected { background-color: %5; color: white; }")
          .arg(Colors::BG_MEDIUM, Colors::BORDER, Colors::TEXT_PRIMARY,
               Colors::BG_LIGHT, Colors::PRIMARY));
  suggestionsList->setMaximumHeight(250);
  contentLayout->addWidget(suggestionsList);

  contentLayout->addStretch();

  // ===== CREDITS BAR =====
  QFrame *creditsFrame = new QFrame();
  creditsFrame->setStyleSheet(
      QString("QFrame { background-color: %1; border-top: 1px solid %2; "
              "padding: 15px; }")
          .arg(Colors::BG_MEDIUM, Colors::BORDER));

  QVBoxLayout *creditsLayout = new QVBoxLayout(creditsFrame);
  creditsLayout->setContentsMargins(20, 15, 20, 15);
  creditsLayout->setSpacing(8);

  QLabel *teamLabel =
      new QLabel("👥 Team: Marwan Khaled (221002058) • Nour Hesham (221001657) "
                 "• Yousef Hesham (221001911)");
  teamLabel->setAlignment(Qt::AlignCenter);
  teamLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 13px; font-weight: bold; "
              "background: transparent; }")
          .arg(Colors::TEXT_PRIMARY));

  QLabel *uniLabel =
      new QLabel("🏛️ Ain Shams University / University of East London");
  uniLabel->setAlignment(Qt::AlignCenter);
  uniLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 12px; background: transparent; }")
          .arg(Colors::TEXT_SECONDARY));

  QLabel *licenseLabel = new QLabel("📄 MIT License • © 2025 Ahead Team");
  licenseLabel->setAlignment(Qt::AlignCenter);
  licenseLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 11px; background: transparent; }")
          .arg(Colors::TEXT_SECONDARY));

  creditsLayout->addWidget(teamLabel);
  creditsLayout->addWidget(uniLabel);
  creditsLayout->addWidget(licenseLabel);

  contentLayout->addWidget(creditsFrame);

  mainLayout->addWidget(contentWidget, 1);

  // ===== STATUS BAR =====
  statusBar()->setStyleSheet(
      QString("QStatusBar { background-color: %1; color: %2; border-top: 1px "
              "solid %3; padding: 5px; font-size: 12px; }")
          .arg(Colors::BG_DARK, Colors::TEXT_SECONDARY, Colors::BORDER));
  statusBar()->showMessage("Ready - Load a dictionary to begin");
}

void MainWindow::connectSignals() {
  // Search box signals
  connect(searchBox, &GhostLineEdit::textChanged, this,
          &MainWindow::onTextChanged);
  connect(searchBox, &GhostLineEdit::returnPressed, this,
          &MainWindow::onSearchEnterPressed);

  // Suggestions list
  connect(suggestionsList, &QListWidget::itemDoubleClicked, this,
          &MainWindow::onSuggestionClicked);

  // Side menu signals
  connect(sideMenu, &SideMenu::addWordRequested, this, &MainWindow::onAddWord);
  connect(sideMenu, &SideMenu::removeWordRequested, this,
          &MainWindow::onRemoveWord);
  connect(sideMenu, &SideMenu::loadFileRequested, this,
          &MainWindow::onLoadFile);
  connect(sideMenu, &SideMenu::saveFileRequested, this,
          &MainWindow::onSaveFile);

  // Model signals - REAL-TIME updates
  connect(model, &RadixTreeModel::suggestionsReady,
          [this](const QVector<RadixTreeModel::Suggestion> &suggestions) {
            suggestionsList->clear();
            for (const auto &s : suggestions) {
              suggestionsList->addItem(
                  QString("%1 (%2)").arg(s.word).arg(s.frequency));
            }
            if (!suggestions.isEmpty()) {
              searchBox->setGhostText(suggestions.first().word);
            } else {
              searchBox->clearGhost();
            }
          });

  // Status manager
  connect(statusManager, &StatusManager::messageChanged,
          [this](const QString &msg) {
            if (msg.isEmpty()) {
              statusBar()->showMessage("Ready");
            } else {
              statusBar()->showMessage(msg);
            }
          });
}

void MainWindow::onTextChanged(const QString &text) {
  if (text.isEmpty()) {
    suggestionsList->clear();
    searchBox->clearGhost();
    return;
  }
  model->getAutocompletions(text);
}

void MainWindow::onSuggestionClicked(QListWidgetItem *item) {
  QString word = item->text().split(" (").first();
  searchBox->setText(word);
  model->incrementFrequency(word);
  // Refresh suggestions to show updated frequency
  model->getAutocompletions(searchBox->text());
  statusManager->showMessage(QString("Selected: %1").arg(word));
}

void MainWindow::onSearchEnterPressed() {
  QString word = searchBox->text().trimmed().toLower();
  if (!word.isEmpty()) {
    if (model->search(word)) {
      model->incrementFrequency(word);
      statusManager->showMessage(
          QString("Frequency increased for: %1").arg(word));
    } else {
      model->insert(word);
      sideMenu->updateWordList(model->getAllWords());
      statusManager->showMessage(QString("Added new word: %1").arg(word));
    }
    sideMenu->setWordCount(model->getWordCount());
    // REAL-TIME: Refresh suggestions to show updated frequency
    model->getAutocompletions(word);
  }
}

void MainWindow::onAddWord(const QString &word) {
  model->insert(word);
  sideMenu->updateWordList(model->getAllWords());
  sideMenu->setWordCount(model->getWordCount());
  statusManager->showMessage(QString("Added: %1").arg(word));
}

void MainWindow::onRemoveWord(const QString &word) {
  model->remove(word);
  // REAL-TIME: Update the word list immediately
  sideMenu->updateWordList(model->getAllWords());
  sideMenu->setWordCount(model->getWordCount());
  // Refresh suggestions if currently searching
  if (!searchBox->text().isEmpty()) {
    model->getAutocompletions(searchBox->text());
  }
  statusManager->showMessage(QString("Removed: %1").arg(word));
}

void MainWindow::onLoadFile(const QString &filePath) {
  model->loadFromFile(filePath);
  currentFile = filePath;
  sideMenu->updateWordList(model->getAllWords());
  sideMenu->setWordCount(model->getWordCount());
  sideMenu->setCurrentFile(QFileInfo(filePath).fileName());
  statusManager->showMessage(QString("Loaded %1 words from %2")
                                 .arg(model->getWordCount())
                                 .arg(QFileInfo(filePath).fileName()));
}

void MainWindow::onSaveFile(const QString &filePath) {
  model->saveToFile(filePath);
  statusManager->showMessage(
      QString("Saved to %1").arg(QFileInfo(filePath).fileName()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  if (snowflakes) {
    snowflakes->setGeometry(0, 0, width(), height());
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (!currentFile.isEmpty()) {
    model->saveMetadata(currentFile);
  }
  event->accept();
}
