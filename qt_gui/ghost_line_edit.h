// qt_gui/ghost_line_edit.h - El Ghost Text Input Header
// El Search bar bel ghost text suggestion
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include <QKeyEvent>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionFrame>

class GhostLineEdit : public QLineEdit {
  Q_OBJECT

public:
  explicit GhostLineEdit(QWidget *parent = nullptr);
  void setGhostText(const QString &suggestion);
  void clearGhost();

signals:
  void suggestionAccepted(const QString &suggestion);
  void textUpdated(const QString &text);

protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

private:
  QString m_ghostText;
  QString m_fullSuggestion;
};
