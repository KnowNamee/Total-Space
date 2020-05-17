#ifndef TYPEOFFSET_H
#define TYPEOFFSET_H

namespace TypeOffset {

enum {
  Planet,
  Button,
  kButtonItem,
};

}  // namespace TypeOffset

namespace ZValues {

enum {
  kEdge = 0,
  kPlanet = 1,
  kGameMenu = 2,
  kPauseMenu = 3,
  kPlanetMenu = 3,
  kMainMenu = 3,
  kInteractionMenu = 4,
  kPlanetInfo = 4,
  kLoadScreen = 5,
  kLoadScreenStage = 6,
  kLoadScreenBar = 6,
  kSettingsMenuDown = 7,
  kSettingsMenuUp = 8,
};

}  // namespace ZValues

#endif  // TYPEOFFSET_H
