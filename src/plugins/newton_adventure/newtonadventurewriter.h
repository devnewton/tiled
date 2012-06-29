#ifndef NEWTONADVENTUREWRITER_H
#define NEWTONADVENTUREWRITER_H

#include "map.h"
#include "objectgroup.h"
#include "protobuf/newton_adventure_level.pb.h"
#include "protobuf/nanim.pb.h"

class NewtonAdventureWriter
{
public:
    NewtonAdventureWriter(const Tiled::Map *map, const QString &fileName);
    bool write();

private:
    void convertMap();
    void convertObjectGroup(const Tiled::ObjectGroup& objectGroup, int zorder);
    const im::bci::newtonadv::EntityType& getOrCreateEntityType(const Tiled::MapObject& object);
    QString getEntityTypeName(const Tiled::MapObject& object);
    const im::bci::newtonadv::EntityType* createEntityType(QString name, const Tiled::MapObject& object);
    bool hasAnimation(std::string name);
    const Tiled::MapObject* findObjectByName(QString name);
    void convertShape(const Tiled::MapObject& object, im::bci::newtonadv::Shape& shape);
    void convertAnimation(im::bci::newtonadv::AnimationReference& animationReference, const Tiled::MapObject& object, const Tiled::Properties& properties);
    bool convertExternalAnimation(im::bci::newtonadv::AnimationReference& animationReference, const Tiled::Properties& properties, const QString& propertyBasename);
    void convertPikes(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertPlatform(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertBouncePlatform(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCannon(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMummy(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertBat(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertApple(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCoin(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertKey(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertDoor(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertDoorToBonusWorld(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCloud(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertWorldMap(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCompass(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMobilePikeAnchor(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertAxeAnchor(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertActivator(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMemoryActivator(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMovingPlatform(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertTeleporter(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertKeyLock(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertHelpSign(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertEgyptianBoss(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertHero(im::bci::newtonadv::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);

    QHash<QString, const im::bci::newtonadv::EntityType* > entityTypesByName_;
    const Tiled::Map* map_;
    const QString fileName_;
    QString levelAnimationFileName_;
    QStringList errors_;
    im::bci::newtonadv::Level level_;
    im::bci::nanim::Nanim levelAnimations_;
};

#endif // NEWTONADVENTUREWRITER_H
