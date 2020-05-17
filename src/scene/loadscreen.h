#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include <QGraphicsItem>
#include <QProgressBar>
#include <QString>
#include <vector>

class ImageItem;
class QGraphicsProxyWidget;

class LoadScreen {
 public:
  LoadScreen() = delete;
  LoadScreen(int stages_cnt);
  ~LoadScreen();

  void LoadNext(const QString& msg);
  void StopLoad();

 private:
  QProgressBar* bar_;
  QGraphicsTextItem* stage_;
  ImageItem* background_;

  int loaded_ = 0;
  int stages_cnt_;

  QProgressBar* GetProgressBar(int w, int h);
};

#endif  // LOADSCREEN_H
