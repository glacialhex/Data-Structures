// qt_gui/status_manager.h - El Status Manager Header
// El Status bar manager - bey-handle el status messages
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

class StatusManager : public QObject {
  Q_OBJECT

public:
  explicit StatusManager(QObject *parent = nullptr);
  void showMessage(const QString &message, int duration = 3000);

signals:
  void messageChanged(const QString &message);

private:
  QTimer *clearTimer;
};
