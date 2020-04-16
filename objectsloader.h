#ifndef OBJECTSLOADER_H
#define OBJECTSLOADER_H

class QString;
class QJsonDocument;
class QJsonObject;

class ObjectsLoader {
 public:
  ObjectsLoader() = delete;

  static void LoadBuildingsFromFile(const QString& file_name);

 private:
  static void LoadBuildingsFromJson(const QJsonDocument& document);
  static void LoadEconomicBuilding(const QJsonObject& building);
};

#endif  // OBJECTSLOADER_H
