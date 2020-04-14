#include "loader.h"

#include <QVector>
#include <memory>

#include "random"
std::shared_ptr<QPixmap> Loader::planet_image_first_ = nullptr;
std::shared_ptr<QPixmap> Loader::planet_image_second_ = nullptr;
std::shared_ptr<QPixmap> Loader::planet_image_third_ = nullptr;
std::shared_ptr<QPixmap> Loader::background_image_ = nullptr;
QVector<std::shared_ptr<QPixmap>> Loader::planet_pictures_;

void Loader::LoadAll() {
  planet_image_first_ = std::make_shared<QPixmap>(":/Img/1.png");
  planet_image_second_ = std::make_shared<QPixmap>(":/Img/2.png");
  planet_image_third_ = std::make_shared<QPixmap>(":/Img/3.png");
  //  background_image_ =
  //  std::make_shared<QPixmap>(":/Sprites/start_screen.jpg");

  planet_pictures_.push_back(planet_image_first_);
  planet_pictures_.push_back(planet_image_second_);
  planet_pictures_.push_back(planet_image_third_);
}

std::shared_ptr<QPixmap> Loader::GetPlanetImage(int number) {
  return planet_pictures_[number];
}

std::shared_ptr<QPixmap> Loader::GetBackgroundImage() {
  return background_image_;
}
