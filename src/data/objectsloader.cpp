#include "data/objectsloader.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "data/objectsstorage.h"
#include "objects/building.h"
#include "objects/unit.h"

void ObjectsLoader::LoadDataFromFile(const QString& file_name) {
  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
  qDebug() << "GameObjectRepository: json parsed, error ="
           << error.errorString();
  Q_ASSERT(error.error == QJsonParseError::NoError);
  LoadDataFromJson(document);
}

void ObjectsLoader::LoadDataFromJson(const QJsonDocument& document) {
  Q_ASSERT(document.isObject());
  auto root_object = document.object();

  // TODO добавить другие функции обработки
  std::vector<std::function<void(const QJsonObject&)>> functions_to_apply = {
      LoadBuilding, LoadUnits};

  size_t function_type = 0;
  for (auto type_it = root_object.begin(); type_it != root_object.end();
       ++type_it) {
    Q_ASSERT(type_it->isObject());
    auto type_object = type_it->toObject();
    for (auto it = type_object.begin(); it != type_object.end(); ++it) {
      Q_ASSERT(it->isObject());
      auto building = it->toObject();
      functions_to_apply[function_type](building);
    }
    function_type++;
  }
}

void ObjectsLoader::LoadBuilding(const QJsonObject& building) {
  QString caption = building.value("caption").toString();
  QString unit_caption = building.value("unit").toString();
  QString type = building.value("type").toString();

  QVector<BuildingType> upgrades_vector;
  QJsonArray upgrades = building.value("upgrades").toArray();
  for (QJsonValue upgrade : upgrades) {
    upgrades_vector.push_back(
        ObjectsStorage::GetBuildingType(upgrade.toString()));
  }

  int32_t level = building.value("level").toInt();
  Resources cost(building.value("batteries_cost").toInt(),
                 building.value("tools_cost").toInt());
  Resources income(building.value("batteries_income").toInt(),
                   building.value("tools_income").toInt());

  Building* building_ptr =
      new Building(caption, type, upgrades_vector, level,
                   ObjectsStorage::GetUnitType(unit_caption), cost, income);

  ObjectsStorage::AddBuilding(building_ptr);
}

void ObjectsLoader::LoadUnits(const QJsonObject& unit) {
  QString caption = unit.value("caption").toString();
  int32_t power = unit.value("power").toInt();
  Resources cost(unit.value("batteries_cost").toInt(),
                 unit.value("tools_cost").toInt());
  Unit* unit_ptr = new Unit(caption, power, cost);
  ObjectsStorage::AddUnit(unit_ptr);
}
