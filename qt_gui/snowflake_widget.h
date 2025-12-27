// qt_gui/snowflake_widget.h - El Snowflake Effect Header
// El Talg effect - animated snowflakes
// © 2025 Ahead Team - Ain Shams / UEL
#pragma once

#include <QPainter>
#include <QTimer>
#include <QVector>
#include <QWidget>

class SnowflakeWidget : public QWidget {
  Q_OBJECT

public:
  explicit SnowflakeWidget(QWidget *parent = nullptr);
  void start();
  void stop();
  void setSnowflakeCount(int count);

protected:
  void paintEvent(QPaintEvent *event) override;

private slots:
  void updateSnowflakes();

private:
  struct Snowflake {
    float x, y, speed, size, opacity;
  };

  QVector<Snowflake> snowflakes;
  QTimer *timer;
  int snowflakeCount;
};
