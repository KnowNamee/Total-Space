#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>

class Resources {
 public:
  Resources() : batteries_(0), tools_(0) {}
  Resources(int32_t batteries, int32_t tools)
      : batteries_(batteries), tools_(tools) {}

  int32_t GetBatteries() const { return batteries_; }
  int32_t GetTools() const { return tools_; }

  const Resources operator+(const Resources& rhs) const {
    return Resources(batteries_ + rhs.batteries_, tools_ + rhs.tools_);
  }

  Resources& operator+=(const Resources& rhs) {
    *this = *this + rhs;
    return *this;
  }

 private:
  int32_t batteries_;
  int32_t tools_;
};

class UnitCharacteristics {
 public:
  UnitCharacteristics() : attack_(0), armor_(0), health_(0), stamina_(0) {}
  UnitCharacteristics(double attack, double armor, double health,
                      double stamina)
      : attack_(attack), armor_(armor), health_(health), stamina_(stamina) {}

  double GetAttack() const { return attack_; }

  double GetArmor() const { return armor_; }

  double GetHealth() const { return health_; }

  double GetStamina() const { return stamina_; }

  const UnitCharacteristics operator*(double rhs) const {
    return UnitCharacteristics(attack_ * rhs, armor_ * rhs,
                               health_ * rhs, stamina_ * rhs);
  }

  UnitCharacteristics& operator*=(double rhs) {
    *this = *this * rhs;
    return *this;
  }

  const UnitCharacteristics operator+(const UnitCharacteristics& rhs) const {
    return UnitCharacteristics(attack_ + rhs.attack_, armor_ + rhs.armor_,
                               health_ + rhs.health_, stamina_ + rhs.stamina_);
  }

  UnitCharacteristics& operator+=(const UnitCharacteristics& rhs) {
    *this = *this + rhs;
    return *this;
  }

 private:
  double attack_;
  double armor_;
  double health_;
  double stamina_;
};

enum class BuildingType {
  kBatteryFactory,
  kForge,
  kWorkshop,
  kAssemblyShop,
  kElectronics,
  kRobotsLine
};

enum class UnitType {
  kNoUnit,
  kMarine,
  kDroid,
  kRanger,
  kFalcon,
  kRover,
};

enum class UnitRole {
  kMelee,
  kRange,
  kHealer,
};

#endif  // UTILITY_H
