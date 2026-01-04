// qt_gui/ghost_line_edit.cpp - El Ghost Text Input Implementation
// El Search bar implementation - bey3mel ghost text suggestion
// © 2025 Ahead Team - Ain Shams / UEL

#include "ghost_line_edit.h"
#include "theme.h"

GhostLineEdit::GhostLineEdit(QWidget *parent) : QLineEdit(parent) {
  setStyleSheet(
      QString("QLineEdit { background-color: %1; color: %2; border: 2px solid "
              "%3; border-radius: 25px; padding: 15px 25px; font-size: 20px; "
              "font-weight: 500; }"
              "QLineEdit:focus { border-color: %4; background-color: %5; }")
          .arg(Colors::BG_MEDIUM, Colors::TEXT_PRIMARY, Colors::BORDER,
               Colors::PRIMARY, Colors::BG_LIGHT));

  setPlaceholderText("Start typing to search...");
  setMinimumHeight(60);

  connect(this, &QLineEdit::textChanged, this, &GhostLineEdit::textUpdated);
}

void GhostLineEdit::setGhostText(const QString &suggestion) {
  QString currentText = text();
  if (suggestion.isEmpty() ||
      !suggestion.startsWith(currentText, Qt::CaseInsensitive)) {
    m_ghostText = "";
    m_fullSuggestion = "";
  } else {
    m_ghostText = suggestion.mid(currentText.length());
    m_fullSuggestion = suggestion;
  }
  update();
}

void GhostLineEdit::clearGhost() {
  m_ghostText = "";
  m_fullSuggestion = "";
  update();
}

void GhostLineEdit::paintEvent(QPaintEvent *event) {
  QLineEdit::paintEvent(event);

  if (!m_ghostText.isEmpty() && hasFocus()) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QStyleOptionFrame option;
    initStyleOption(&option);
    QRect textRect =
        style()->subElementRect(QStyle::SE_LineEditContents, &option, this);
    // textRect.adjust(25, 0, 0, 0); // Removed double padding adjustment

    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(text());

    painter.setPen(QColor(Colors::TEXT_SECONDARY));
    painter.setFont(font());

    QRect ghostRect = textRect;
    ghostRect.setLeft(textRect.left() + textWidth);
    painter.drawText(ghostRect, Qt::AlignLeft | Qt::AlignVCenter, m_ghostText);
  }
}

void GhostLineEdit::keyPressEvent(QKeyEvent *event) {
  // Tab or Right Arrow accepts suggestion
  if ((event->key() == Qt::Key_Tab || event->key() == Qt::Key_Right) &&
      !m_fullSuggestion.isEmpty() && cursorPosition() == text().length()) {
    setText(m_fullSuggestion);
    emit suggestionAccepted(m_fullSuggestion);
    clearGhost();
    event->accept();
    return;
  }

  // Space at end also accepts
  if (event->key() == Qt::Key_Space && !m_fullSuggestion.isEmpty() &&
      cursorPosition() == text().length()) {
    setText(m_fullSuggestion + " ");
    emit suggestionAccepted(m_fullSuggestion);
    clearGhost();
    event->accept();
    return;
  }

  QLineEdit::keyPressEvent(event);
}
