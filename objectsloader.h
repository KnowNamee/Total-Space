#ifndef OBJECTSLOADER_H
#define OBJECTSLOADER_H

class QString;
class QJsonDocument;
class QJsonObject;

class ObjectsLoader {
public:
  ObjectsLoader() = delete;

<<<<<<< HEAD
  static void LoadBuildingsFromFile(const QString &file_name);

private:
  static void LoadBuildingsFromJson(const QJsonDocument &document);
  static void LoadEconomicBuilding(const QJsonObject &building);
  static void LoadUnit(const QJsonObject &unit);
=======
  static void LoadDataFromFile(const QString& file_name);

 private:
  static void LoadDataFromJson(const QJsonDocument& document);
  static void LoadEconomicBuilding(const QJsonObject& building);
>>>>>>> fa59a83c17d39e0028b1b720f26925ba89193ce2
};

#endif // OBJECTSLOADER_H
