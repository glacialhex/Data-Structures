// qt_gui/snowflake_widget.cpp - El Snowflake Effect Implementation
// El Talg bey-animate - kol snowflake 3andaha speed we size
// © 2025 Ahead Team - Ain Shams / UEL

#include "snowflake_widget.h"
#include <QRandomGenerator>

SnowflakeWidget::SnowflakeWidget(QWidget *parent)
    : QWidget(parent), snowflakeCount(80) {
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setAttribute(Qt::WA_TranslucentBackground);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &SnowflakeWidget::updateSnowflakes);
}

void SnowflakeWidget::start() {
  snowflakes.clear();
  for (int i = 0; i < snowflakeCount; ++i) {
    Snowflake s;
    s.x = QRandomGenerator::global()->bounded(2000);
    s.y = QRandomGenerator::global()->bounded(1500);
    s.speed = 0.3f + QRandomGenerator::global()->bounded(100) / 100.0f;
    s.size = 2.0f + QRandomGenerator::global()->bounded(40) / 10.0f;
    s.opacity = 0.3f + QRandomGenerator::global()->bounded(70) / 100.0f;
    snowflakes.append(s);
  }
  timer->start(33); // ~30 FPS
}

void SnowflakeWidget::stop() { timer->stop(); }

void SnowflakeWidget::setSnowflakeCount(int count) { snowflakeCount = count; }

void SnowflakeWidget::updateSnowflakes() {
  for (auto &s : snowflakes) {
    s.y += s.speed;
    s.x += (QRandomGenerator::global()->bounded(10) - 5) * 0.1f;

    if (s.y > height()) {
      s.y = -10;
      s.x = QRandomGenerator::global()->bounded(width());
    }
    if (s.x < 0)
      s.x = width();
    if (s.x > width())
      s.x = 0;
  }
  update();
}

void SnowflakeWidget::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  for (const auto &s : snowflakes) {
    QColor color(255, 255, 255, int(s.opacity * 255));
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(s.x, s.y), s.size, s.size);
  }
}
