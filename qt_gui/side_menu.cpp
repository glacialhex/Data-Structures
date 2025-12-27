// qt_gui/side_menu.cpp - El Side Menu Implementation
// El Side panel implementation - collapsible we fiha dark buttons
// © 2025 Ahead Team - Ain Shams / UEL

#include "side_menu.h"
#include <QHBoxLayout>

SideMenu::SideMenu(QWidget *parent)
    : QFrame(parent), isCollapsed(false), expandedWidth(280) {
  setFixedWidth(expandedWidth);
  setStyleSheet(
      QString("QFrame { background-color: %1; border-right: 1px solid %2; }")
          .arg(Colors::BG_MEDIUM, Colors::BORDER));
  setupUI();
}

void SideMenu::setupUI() {
  mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(16, 20, 16, 20);
  mainLayout->setSpacing(12);

  // Toggle button - el button bta3 el collapse
  toggleButton = new QPushButton("◀");
  toggleButton->setFixedSize(32, 32);
  toggleButton->setCursor(Qt::PointingHandCursor);
  toggleButton->setStyleSheet(
      QString("QPushButton { background-color: %1; color: %2; border: 1px "
              "solid %3; border-radius: 6px; font-size: 14px; }"
              "QPushButton:hover { background-color: %4; }")
          .arg(Colors::BG_LIGHT, Colors::TEXT_PRIMARY, Colors::BORDER,
               Colors::BG_DARK));
  connect(toggleButton, &QPushButton::clicked, this, &SideMenu::toggleCollapse);

  QHBoxLayout *toggleLayout = new QHBoxLayout();
  toggleLayout->addStretch();
  toggleLayout->addWidget(toggleButton);
  mainLayout->addLayout(toggleLayout);

  // Content widget - el content elly bey-collapse
  contentWidget = new QWidget();
  QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
  contentLayout->setContentsMargins(0, 0, 0, 0);
  contentLayout->setSpacing(12);

  // Title
  QLabel *title = new QLabel("Word Manager");
  title->setStyleSheet(
      QString("QLabel { color: %1; font-size: 18px; font-weight: bold; }")
          .arg(Colors::TEXT_PRIMARY));
  contentLayout->addWidget(title);

  // File section - el file indicator
  QFrame *fileFrame = new QFrame();
  fileFrame->setStyleSheet(
      QString(
          "QFrame { background-color: %1; border-radius: 8px; padding: 8px; }")
          .arg(Colors::BG_LIGHT));
  QVBoxLayout *fileLayout = new QVBoxLayout(fileFrame);
  fileLayout->setContentsMargins(10, 8, 10, 8);

  QLabel *fileTitle = new QLabel("📂 Current Source");
  fileTitle->setStyleSheet(
      QString("QLabel { color: %1; font-size: 12px; font-weight: bold; }")
          .arg(Colors::TEXT_SECONDARY));
  currentFileLabel = new QLabel("No file loaded");
  currentFileLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 13px; padding: 4px 0; }")
          .arg(Colors::TEXT_PRIMARY));
  currentFileLabel->setWordWrap(true);
  fileLayout->addWidget(fileTitle);
  fileLayout->addWidget(currentFileLabel);
  contentLayout->addWidget(fileFrame);

  // Separator
  QFrame *sep1 = new QFrame();
  sep1->setFrameShape(QFrame::HLine);
  sep1->setStyleSheet(QString("background-color: %1;").arg(Colors::BORDER));
  sep1->setFixedHeight(1);
  contentLayout->addWidget(sep1);

  // Word input
  wordInput = new QLineEdit();
  wordInput->setPlaceholderText("Enter word...");
  wordInput->setStyleSheet(
      QString("QLineEdit { background-color: %1; color: %2; border: 1px solid "
              "%3; border-radius: 8px; padding: 10px 12px; font-size: 14px; }"
              "QLineEdit:focus { border-color: %4; }")
          .arg(Colors::BG_LIGHT, Colors::TEXT_PRIMARY, Colors::BORDER,
               Colors::PRIMARY));
  contentLayout->addWidget(wordInput);

  // Add button - dark green
  addButton = createMenuButton("", "Add Word", "#1a7f37");
  connect(addButton, &QPushButton::clicked, this, &SideMenu::onAddClicked);
  connect(wordInput, &QLineEdit::returnPressed, this, &SideMenu::onAddClicked);
  contentLayout->addWidget(addButton);

  // Word list label
  QLabel *listLabel = new QLabel("Dictionary Words:");
  listLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 12px; margin-top: 8px; }")
          .arg(Colors::TEXT_SECONDARY));
  contentLayout->addWidget(listLabel);

  // Word list
  wordList = new QListWidget();
  wordList->setStyleSheet(
      QString(
          "QListWidget { background-color: %1; border: 1px solid %2; "
          "border-radius: 8px; font-size: 13px; }"
          "QListWidget::item { color: %3; padding: 8px 10px; border-radius: "
          "4px; }"
          "QListWidget::item:hover { background-color: %4; }"
          "QListWidget::item:selected { background-color: %5; color: white; }")
          .arg(Colors::BG_LIGHT, Colors::BORDER, Colors::TEXT_PRIMARY,
               Colors::BG_MEDIUM, Colors::PRIMARY));
  wordList->setMaximumHeight(180);
  connect(wordList, &QListWidget::itemClicked,
          [this](QListWidgetItem *item) { emit wordSelected(item->text()); });
  contentLayout->addWidget(wordList);

  // Word count
  wordCountLabel = new QLabel("0 words loaded");
  wordCountLabel->setStyleSheet(
      QString("QLabel { color: %1; font-size: 11px; }")
          .arg(Colors::TEXT_SECONDARY));
  contentLayout->addWidget(wordCountLabel);

  // Remove button - dark red
  removeButton = createMenuButton("", "Remove Selected", "#a12830");
  connect(removeButton, &QPushButton::clicked, this,
          &SideMenu::onRemoveClicked);
  contentLayout->addWidget(removeButton);

  // Separator
  QFrame *sep2 = new QFrame();
  sep2->setFrameShape(QFrame::HLine);
  sep2->setStyleSheet(QString("background-color: %1;").arg(Colors::BORDER));
  sep2->setFixedHeight(1);
  contentLayout->addWidget(sep2);

  // File operations label
  QLabel *fileLabel = new QLabel("File Operations");
  fileLabel->setStyleSheet(QString("QLabel { color: %1; font-size: 14px; "
                                   "font-weight: bold; margin-top: 4px; }")
                               .arg(Colors::TEXT_PRIMARY));
  contentLayout->addWidget(fileLabel);

  // Load button
  loadButton = createMenuButton("📂", "Load Dictionary", "#3d5a80");
  connect(loadButton, &QPushButton::clicked, this, &SideMenu::onLoadClicked);
  contentLayout->addWidget(loadButton);

  // Save button
  saveButton = createMenuButton("💾", "Save Dictionary", "#293241");
  connect(saveButton, &QPushButton::clicked, this, &SideMenu::onSaveClicked);
  contentLayout->addWidget(saveButton);

  contentLayout->addStretch();
  mainLayout->addWidget(contentWidget);
}

QPushButton *SideMenu::createMenuButton(const QString &icon,
                                        const QString &text,
                                        const QString &color) {
  QString label = icon.isEmpty() ? text : QString("%1  %2").arg(icon, text);
  QPushButton *btn = new QPushButton(label);
  btn->setCursor(Qt::PointingHandCursor);
  btn->setStyleSheet(
      QString("QPushButton { background-color: %1; color: white; border: none; "
              "border-radius: 8px; padding: 12px 16px; font-size: 14px; "
              "text-align: left; }"
              "QPushButton:hover { background-color: %2; }"
              "QPushButton:pressed { background-color: %3; }")
          .arg(color, QColor(color).lighter(115).name(),
               QColor(color).darker(110).name()));
  return btn;
}

void SideMenu::toggleCollapse() {
  isCollapsed = !isCollapsed;
  if (isCollapsed) {
    contentWidget->hide();
    setFixedWidth(60);
    toggleButton->setText("▶");
  } else {
    contentWidget->show();
    setFixedWidth(expandedWidth);
    toggleButton->setText("◀");
  }
}

void SideMenu::onAddClicked() {
  QString word = wordInput->text().trimmed().toLower();
  if (!word.isEmpty()) {
    emit addWordRequested(word);
    wordInput->clear();
  }
}

void SideMenu::onRemoveClicked() {
  if (wordList->currentItem()) {
    emit removeWordRequested(wordList->currentItem()->text());
  }
}

void SideMenu::onLoadClicked() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Load Dictionary", "", "Text Files (*.txt);;All Files (*)");
  if (!filePath.isEmpty()) {
    currentFilePath = filePath;
    currentFileLabel->setText(QFileInfo(filePath).fileName());
    emit loadFileRequested(filePath);
  }
}

void SideMenu::onSaveClicked() {
  QString filePath = QFileDialog::getSaveFileName(
      this, "Save Dictionary", "output_dictionary.txt", "Text Files (*.txt)");
  if (!filePath.isEmpty()) {
    emit saveFileRequested(filePath);
  }
}

void SideMenu::updateWordList(const QStringList &words) {
  wordList->clear();
  for (const QString &word : words) {
    wordList->addItem(word);
  }
}

void SideMenu::setWordCount(int count) {
  wordCountLabel->setText(QString("%1 words loaded").arg(count));
}

void SideMenu::setCurrentFile(const QString &filename) {
  currentFileLabel->setText(filename);
}
