#include "data/loader.h"

#include <QBrush>
#include <QFontDatabase>
#include <QVector>
#include <memory>

std::shared_ptr<QPixmap> Loader::background_image_ = nullptr;
QVector<std::shared_ptr<QPixmap>> Loader::planet_pictures_;
QMap<ButtonsEnum, std::shared_ptr<QPixmap>> Loader::button_images_;
QMap<UnitType, std::shared_ptr<QPixmap>> Loader::unit_images_;
QMap<BuildingType, std::shared_ptr<QPixmap>> Loader::building_images_;
int32_t Loader::font_;
QBrush* Loader::ibrush_ = nullptr;

void Loader::LoadAll() {
  // font
  font_ = QFontDatabase::addApplicationFont(":/Img/Fabulist.ttf");
  // brush
  ibrush_ = new QBrush;
  ibrush_->setTextureImage(QImage(":/Img/transparent_bg.png"));
  // pictures
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
    button_images_[ButtonsEnum::kUnactiveWidget] =
        std::make_shared<QPixmap>(":/Img/unactive_widget.png");
    button_images_[ButtonsEnum::kLoadingBackground] =
        std::make_shared<QPixmap>(":/Img/loading_background.jpg");
    button_images_[ButtonsEnum::kUpgradActiveButton] =
        std::make_shared<QPixmap>(":/Img/upgrade_active_button.png");
    button_images_[ButtonsEnum::kUpgradeUnactiveButton] =
        std::make_shared<QPixmap>(":/Img/upgrade_unactive_button.png");
    button_images_[ButtonsEnum::kInfoButton] =
        std::make_shared<QPixmap>(":/Img/info_button.png");
    button_images_[ButtonsEnum::kNoNameUnit] =
        std::make_shared<QPixmap>(":/Img/no_name.png");
    button_images_[ButtonsEnum::kEscapeButton] =
        std::make_shared<QPixmap>(":/Img/exit_shop.png");
    button_images_[ButtonsEnum::kUnitsButton] =
        std::make_shared<QPixmap>(":/Img/unit_shop.png");
    button_images_[ButtonsEnum::kBuildingsButton] =
        std::make_shared<QPixmap>(":/Img/factory_shop.png");
    button_images_[ButtonsEnum::kBuyButton] =
        std::make_shared<QPixmap>(":/Img/buy_button.png");
    button_images_[ButtonsEnum::kBeautifulMoveButton] =
        std::make_shared<QPixmap>(":/Img/move_beautiful_button.png");
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
  {
    building_images_[BuildingType::kForge] =
        std::make_shared<QPixmap>(":/Img/forge.png");
    building_images_[BuildingType::kWorkshop] =
        std::make_shared<QPixmap>(":/Img/workshop.png");
    building_images_[BuildingType::kRobotsLine] =
        std::make_shared<QPixmap>(":/Img/robot_line.png");
    building_images_[BuildingType::kElectronics] =
        std::make_shared<QPixmap>(":/Img/electronics.png");
    building_images_[BuildingType::kAssemblyShop] =
        std::make_shared<QPixmap>(":/Img/assembly_shop.png");
    building_images_[BuildingType::kBatteryFactory] =
        std::make_shared<QPixmap>(":/Img/battery_factory.png");
    building_images_[BuildingType::kBarracks] =
        std::make_shared<QPixmap>(":/Img/barrack.png");
    building_images_[BuildingType::kTrainingBase] =
        std::make_shared<QPixmap>(":/Img/Training Base.png");
    building_images_[BuildingType::kSpaceport] =
        std::make_shared<QPixmap>(":/Img/Spaceport.png");
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

QPixmap* Loader::GetBuildingImage(BuildingType type) {
  return building_images_[type].get();
}

QBrush* Loader::GetBrush() { return ibrush_; }

int32_t Loader::GetFont() { return font_; }
