#include "objectsloader.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

void ObjectsLoader::LoadBuildingsFromFile(const QString &file_name) {
  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QJsonParseError error;
  QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
  qDebug() << "GameObjectRepository: json parsed, error ="
           << error.errorString();
  Q_ASSERT(error.error == QJsonParseError::NoError);
  LoadBuildingsFromJson(document);
}

void ObjectsLoader::LoadBuildingsFromJson(const QJsonDocument &document) {}

void ObjectsLoader::LoadBuilding(const QJsonObject &building) {}
