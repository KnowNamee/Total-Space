#ifndef LOADER_H
#define LOADER_H
#include <QGraphicsItem>
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
  kNoNameUnit
};

class Loader : public QGraphicsItem {
 public:
  Loader() = delete;
  static void LoadAll();
  static QPixmap* GetPlanetImage(int number);
  static QPixmap* GetBackgroundImage();
  static QPixmap* GetButtonImage(ButtonsEnum);
  static QPixmap* GetUnitImage(UnitType);
  static QBrush* GetBrush();
  static int32_t GetFont();

 private:
  static QVector<std::shared_ptr<QPixmap>> planet_pictures_;
  static std::shared_ptr<QPixmap> background_image_;
  static QMap<ButtonsEnum, std::shared_ptr<QPixmap>> button_images_;
  static QMap<UnitType, std::shared_ptr<QPixmap>> unit_images_;
  static QBrush* ibrush_;
  static int32_t font_;
};

#endif  // LOADER_H
