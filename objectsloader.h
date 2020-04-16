#ifndef OBJECTSLOADER_H
#define OBJECTSLOADER_H

class QString;
class QJsonDocument;
class QJsonObject;

class ObjectsLoader {
 public:
  ObjectsLoader() = delete;

  static void LoadDataFromFile(const QString& file_name);

 private:
  static void LoadDataFromJson(const QJsonDocument& document);
  static void LoadEconomicBuilding(const QJsonObject& building);
};

#endif  // OBJECTSLOADER_H
