#ifndef LOADER_H
#define LOADER_H
#include <QGraphicsItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <memory>

#include "util/utility.h"

enum class ButtonsEnum {
  kNewGameButton,
  kExitButton,
  kBackground,
  kSimpleButton,
  kToMenuButton,
  kBackToGameButton,
  kMainBackground,
  kMenuBackground,
  kLoadingBackground,
  kAttackButton,
  kBeautifulAttackButton,
  kCancelButton,
  kMoveButton,
  kShopButton,
  kNextTurnButton,
  kUnactiveWidget,
  kActiveWidget,
  kUpgradeUnactiveButton,
  kUpgradActiveButton,
  kInfoButton,
  kNoNameUnit,
  kEscapeButton,
  kUnitsButton,
  kBuildingsButton,
  kBuyButton,
  kBeautifulMoveButton,
  kBatteriesIcon,
  kToolsIcon,
  kArmyPowerIcon,
  kSettingsButton,
  kLoser,
  kWinner
};

class Loader : public QGraphicsItem {
 public:
  Loader() = delete;
  static void LoadAll();
  static QPixmap* GetPlanetImage(int number);
  static QPixmap* GetBackgroundImage();
  static QPixmap* GetButtonImage(ButtonsEnum);
  static QPixmap* GetUnitImage(UnitType);
  static QPixmap* GetBuildingImage(BuildingType);
  static QBrush* GetBrush();
  static int32_t GetFont();
  static QMediaPlayer* GetClickSound();
  static QMediaPlayer* GetBackgroundSong();

 private:
  static QMediaPlayer* click_sound_;
  static QMediaPlayer* background_song_;

  static QVector<std::shared_ptr<QPixmap>> planet_pictures_;
  static std::shared_ptr<QPixmap> background_image_;
  static QMap<ButtonsEnum, std::shared_ptr<QPixmap>> button_images_;
  static QMap<UnitType, std::shared_ptr<QPixmap>> unit_images_;
  static QMap<BuildingType, std::shared_ptr<QPixmap>> building_images_;
  static QBrush* ibrush_;
  static int32_t font_;
};

#endif  // LOADER_H
