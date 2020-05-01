#ifndef RESOURCES_H
#define RESOURCES_H

#include <cstdint>

class Resources {
 public:
  Resources() : batteries_(0), tools_(0) {}
  Resources(int32_t batteries, int32_t tools)
      : batteries_(batteries), tools_(tools) {}

  int32_t GetBatteries() const { return batteries_; }
  int32_t GetTools() const { return tools_; }

  const Resources operator+(const Resources& rhs) {
    return Resources(batteries_ + rhs.batteries_, tools_ + rhs.tools_);
  }
  const Resources operator-(const Resources& rhs) {
    return Resources(batteries_ - rhs.batteries_, tools_ - rhs.tools_);
  }

  Resources& operator+=(const Resources& rhs) {
    *this = *this + rhs;
    return *this;
  }
  Resources& operator-=(const Resources& rhs) {
    *this = *this - rhs;
    return *this;
  }

 private:
  int32_t batteries_;
  int32_t tools_;
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

enum class ShopType {
    kUnit,
    kBuilding,
};

#endif  // RESOURCES_H
