#ifndef LOADER_H
#define LOADER_H
#include <QGraphicsItem>
#include <memory>

enum class ButtonsEnum {
  kNewGameButton,
  kExitButton,
  kBackground,
  kSimpleButton,
  kToMenuButton,
  kBackToGameButton,
  kMainBackground
};

class Loader : public QGraphicsItem {
 public:
  Loader() = delete;
  static void LoadAll();
  static QPixmap* GetPlanetImage(int number);
  static QPixmap* GetBackgroundImage();
  static QPixmap* GetButtonImage(ButtonsEnum);

 private:
  static QVector<std::shared_ptr<QPixmap>> planet_pictures_;
  static std::shared_ptr<QPixmap> background_image_;
  static QMap<ButtonsEnum, std::shared_ptr<QPixmap>> button_images_;
};

#endif  // LOADER_H
