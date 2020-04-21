#include "data/loader.h"

#include <QVector>
#include <memory>

std::shared_ptr<QPixmap> Loader::background_image_ = nullptr;
QVector<std::shared_ptr<QPixmap>> Loader::planet_pictures_;
QMap<ButtonsEnum, std::shared_ptr<QPixmap>> Loader::button_images_;

void Loader::LoadAll() {
  planet_pictures_.push_back(std::make_shared<QPixmap>(":/Img/1.png"));
  planet_pictures_.push_back(std::make_shared<QPixmap>(":/Img/2.png"));
  planet_pictures_.push_back(std::make_shared<QPixmap>(":/Img/3.png"));

  button_images_[ButtonsEnum::kNewGameButton] =
      std::make_shared<QPixmap>(":/Img/new_game_button.png");
  button_images_[ButtonsEnum::kExitButton] =
      std::make_shared<QPixmap>(":/Img/exit_button.png");
  button_images_[ButtonsEnum::kBackground] =
      std::make_shared<QPixmap>(":/Img/start_background.jpg");
  button_images_[ButtonsEnum::kSimpleButton] =
      std::make_shared<QPixmap>(":/Img/simple_button.png");
  button_images_[ButtonsEnum::kBackToGameButton] =
      std::make_shared<QPixmap>(":/Img/back_button.png");
  button_images_[ButtonsEnum::kToMenuButton] =
      std::make_shared<QPixmap>(":/Img/to_main_menu_button.png");
  button_images_[ButtonsEnum::kMainBackground] =
      std::make_shared<QPixmap>(":/Img/main_background.jpg");
}

QPixmap *Loader::GetPlanetImage(int number) {
  return planet_pictures_[number].get();
}

QPixmap *Loader::GetBackgroundImage() { return background_image_.get(); }

QPixmap *Loader::GetButtonImage(ButtonsEnum type) {
  return button_images_[type].get();
}
