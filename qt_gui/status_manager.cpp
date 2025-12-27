// qt_gui/status_manager.cpp - El Status Manager Implementation
// El Status bar logic - bey-show messages lel user
// © 2025 Ahead Team - Ain Shams / UEL

#include "status_manager.h"

StatusManager::StatusManager(QObject *parent) : QObject(parent) {
  clearTimer = new QTimer(this);
  clearTimer->setSingleShot(true);
  connect(clearTimer, &QTimer::timeout, [this]() { emit messageChanged(""); });
}

void StatusManager::showMessage(const QString &message, int duration) {
  emit messageChanged(message);
  clearTimer->start(duration);
}
