#ifndef LOADER_H
#define LOADER_H
#include <QGraphicsItem>
#include <memory>

class Loader : public QGraphicsItem {
 public:
  Loader() = delete;
  static void LoadAll();
  static std::shared_ptr<QPixmap> GetPlanetImage(int number);
  static std::shared_ptr<QPixmap> GetBackgroundImage();

 private:
  static std::shared_ptr<QPixmap> planet_image_first_;
  static std::shared_ptr<QPixmap> planet_image_second_;
  static std::shared_ptr<QPixmap> planet_image_third_;
  static QVector<std::shared_ptr<QPixmap>> planet_pictures_;

  static std::shared_ptr<QPixmap> background_image_;
};

#endif  // LOADER_H
