#include "data/loader.h"

#include <QFontDatabase>
#include <QVector>
#include <memory>

std::shared_ptr<QPixmap> Loader::background_image_ = nullptr;
QVector<std::shared_ptr<QPixmap>> Loader::planet_pictures_;
QMap<ButtonsEnum, std::shared_ptr<QPixmap>> Loader::button_images_;
QMap<UnitType, std::shared_ptr<QPixmap>> Loader::unit_images_;
int32_t Loader::font_;

void Loader::LoadAll() {
  // font
  font_ = QFontDatabase::addApplicationFont(":/Img/Fabulist.ttf");
  planet_pictures_.push_back(std::make_shared<QPixmap>(":/Img/1.png"));
  planet_pictures_.push_back(std::make_shared<QPixmap>(":/Img/2.png"));
  planet_pictures_.push_back(std::make_shared<QPixmap>(":/Img/3.png"));
  // button images
  {
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
    button_images_[ButtonsEnum::kMenuBackground] =
        std::make_shared<QPixmap>(":/Img/menu_background.jpg");
    button_images_[ButtonsEnum::kAttackButton] =
        std::make_shared<QPixmap>(":/Img/attack_button.png");
    button_images_[ButtonsEnum::kCancelButton] =
        std::make_shared<QPixmap>(":/Img/cancel_button.png");
    button_images_[ButtonsEnum::kMoveButton] =
        std::make_shared<QPixmap>(":/Img/move_button.png");
    button_images_[ButtonsEnum::kShopButton] =
        std::make_shared<QPixmap>(":/Img/shop_button.png");
    button_images_[ButtonsEnum::kBeautifulAttackButton] =
        std::make_shared<QPixmap>(":/Img/attack_beautiful_button.png");
    button_images_[ButtonsEnum::kNextTurnButton] =
        std::make_shared<QPixmap>(":/Img/next_turn_button.png");
    button_images_[ButtonsEnum::kActiveWidget] =
        std::make_shared<QPixmap>(":/Img/active_widget.png");
    button_images_[ButtonsEnum::kUnactiveWidget] =
        std::make_shared<QPixmap>(":/Img/unactive_widget.png");
  }
  // unit images
  {
    unit_images_[UnitType::kMarine] =
        std::make_shared<QPixmap>(":/Img/marine.png");
    unit_images_[UnitType::kRover] =
        std::make_shared<QPixmap>(":/Img/rover.png");
    unit_images_[UnitType::kRanger] =
        std::make_shared<QPixmap>(":/Img/ranger.png");
    unit_images_[UnitType::kDroid] =
        std::make_shared<QPixmap>(":/Img/droid.png");
    unit_images_[UnitType::kFalcon] =
        std::make_shared<QPixmap>(":/Img/falcon.png");
    unit_images_[UnitType::kNoUnit] =
        std::make_shared<QPixmap>(":/Img/EmptyUnit.png");
  }
}

QPixmap* Loader::GetPlanetImage(int number) {
  return planet_pictures_[number].get();
}

QPixmap* Loader::GetBackgroundImage() { return background_image_.get(); }

QPixmap* Loader::GetButtonImage(ButtonsEnum type) {
  return button_images_[type].get();
}

QPixmap* Loader::GetUnitImage(UnitType type) {
  return unit_images_[type].get();
}

int32_t Loader::GetFont() { return font_; }
