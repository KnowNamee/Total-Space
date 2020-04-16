#include "objectsloader.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

#include "building.h"
#include "objectsstorage.h"

void ObjectsLoader::LoadDataFromFile(const QString &file_name) {
  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
  qDebug() << "GameObjectRepository: json parsed, error ="
           << error.errorString();
  Q_ASSERT(error.error == QJsonParseError::NoError);
  LoadDataFromJson(document);
}

void ObjectsLoader::LoadDataFromJson(const QJsonDocument &document) {
  Q_ASSERT(document.isObject());
  auto root_object = document.object();

  // TODO добавить другие функции обработки
  std::vector<std::function<void(const QJsonObject &)>> functions_to_apply = {
      LoadEconomicBuilding};

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

void ObjectsLoader::LoadEconomicBuilding(const QJsonObject &building) {
  QString caption = building.value("caption").toString();
  QString type = building.value("type").toString();
  Resources cost(building.value("batteries_cost").toInt(),
                 building.value("tools_cost").toInt());
  Resources income(building.value("batteries_income").toInt(),
                   building.value("tools_income").toInt());
  Resources keeping(building.value("keeping_batteries").toInt(),
                    building.value("keeping_tools").toInt());
  EconomicBuilding *building_ptr =
      new EconomicBuilding(caption, type, cost, income, keeping);
  ObjectsStorage::AddBuilding(building_ptr);
}
