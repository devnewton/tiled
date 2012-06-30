#include "newtonadventurewriter.h"
#include <fstream>
#include <QtCore/QFileInfo>
#include "mapobject.h"
#include "tile.h"

NewtonAdventureWriter::NewtonAdventureWriter(const Tiled::Map *map, const QString &fileName)
    : map_( map )
    , fileName_(fileName)
    , levelAnimationFileName_(QFileInfo(fileName_).baseName() + ".nanim")
{
}

bool NewtonAdventureWriter::write()
{
    convertMap();

    std::fstream out(fileName_.toStdString().c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    level_.SerializeToOstream(&out);
    out.close();

    std::fstream nanimOut(levelAnimationFileName_.toStdString().c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    levelAnimations_.SerializeToOstream(&nanimOut);
    nanimOut.close();
    return true;
}

void NewtonAdventureWriter::convertMap()
{
    int zorder = 0;
    Q_FOREACH(Tiled::Layer* layer, map_->layers())
    {
        const Tiled::ObjectGroup* objectGroup = layer->asObjectGroup();
        if(objectGroup) {
            convertObjectGroup(*objectGroup, zorder++);
        }
    }
    level_.set_rotate_gravity_possible("true" == map_->properties()["newton_adventure.rotate_gravity_possible"]);
}

void NewtonAdventureWriter::convertObjectGroup(const Tiled::ObjectGroup& objectGroup, int zorder)
{
    Q_FOREACH(Tiled::MapObject* object, objectGroup.objects())
    {
        im::bci::newtonadv::nal::Entity& entity = *level_.add_entities();
        entity.mutable_position()->set_x(object->x());
        entity.mutable_position()->set_y(object->y());
        entity.set_zorder(zorder);
        const im::bci::newtonadv::nal::EntityType& entityType = getOrCreateEntityType(*object);
        entity.set_type(entityType.name());
    }
}

const im::bci::newtonadv::nal::EntityType& NewtonAdventureWriter::getOrCreateEntityType(const Tiled::MapObject& object)
{
    QString name = getEntityTypeName(object);
    QHash<QString, const im::bci::newtonadv::nal::EntityType* >::const_iterator it = entityTypesByName_.find(name);
    if(it != entityTypesByName_.end())
    {
        return *it.value();
    } else
    {
        const im::bci::newtonadv::nal::EntityType* entityType = createEntityType(name, object);
        entityTypesByName_.insert(name, entityType);
        return *entityType;
    }

}

QString NewtonAdventureWriter::getEntityTypeName(const Tiled::MapObject& object)
{
    if(Tiled::Tile* tile = object.tile())
    {
        return QString("tile_%s").arg(tile->id());
    } else
    {
        return object.name();
    }
}

Tiled::Properties getObjectProperties(const Tiled::MapObject& object)
{
    Tiled::Properties properties;
    if(Tiled::Tile* tile = object.tile())
    {
        properties.merge(tile->properties());
    }
    properties.merge(object.properties());
    return properties;
}

const Tiled::MapObject* NewtonAdventureWriter::findObjectByName(QString name)
{
    Q_FOREACH(const Tiled::ObjectGroup* group, map_->objectGroups())
    {
         Q_FOREACH(const Tiled::MapObject* object, group->objects())
         {
             if(object->name() == name)
             {
                 return object;
             }
         }
    }
    return 0;
}

const im::bci::newtonadv::nal::EntityType* NewtonAdventureWriter::createEntityType(QString name, const Tiled::MapObject& object)
{
    im::bci::newtonadv::nal::EntityType& entityType =  *level_.add_entity_types();
    entityType.set_name(name.toStdString());
    convertShape(object, *entityType.mutable_shape());

    Tiled::Properties properties = getObjectProperties(object);
    QString type = object.type();

    if(type == "pikes")
        convertPikes(entityType, object, properties);
    else if(type == "platfom")
        convertPlatform(entityType, object, properties);
    else if(type == "bounce_platform")
        convertBouncePlatform(entityType, object, properties);
    else if(type == "cannon")
        convertCannon(entityType, object, properties);
    else if(type == "mumy")
        convertMummy(entityType, object, properties);
    else if(type == "bat")
        convertBat(entityType, object, properties);
    else if(type == "apple")
        convertApple(entityType, object, properties);
    else if(type == "coin")
        convertCoin(entityType, object, properties);
    else if(type == "key")
        convertKey(entityType, object, properties);
    else if(type == "door")
        convertDoor(entityType, object, properties);
    else if(type == "door_to_bonus_world")
        convertDoorToBonusWorld(entityType, object, properties);
    else if(type == "cloud")
        convertCloud(entityType, object, properties);
    else if(type == "world_map")
        convertWorldMap(entityType, object, properties);
    else if(type == "compass")
        convertCompass(entityType, object, properties);
    else if(type == "mobile_pike_anchor")
        convertMobilePikeAnchor(entityType, object, properties);
    else if(type == "axe_anchor")
        convertAxeAnchor(entityType, object, properties);
    else if(type == "activator")
        convertActivator(entityType, object, properties);
    else if(type == "memory_activator")
        convertMemoryActivator(entityType, object, properties);
    else if(type == "moving_platform")
        convertMovingPlatform(entityType, object, properties);
    else if(type == "teleporter")
        convertTeleporter(entityType, object, properties);
    else if(type == "key_lock")
        convertKeyLock(entityType, object, properties);
    else if(type == "help_sign")
        convertHelpSign(entityType, object, properties);
    else if(type == "egyption_boss")
        convertEgyptianBoss(entityType, object, properties);
    else if(type == "hero")
        convertHero(entityType, object, properties);

    return &entityType;

}

void NewtonAdventureWriter::convertShape(const Tiled::MapObject& object, im::bci::newtonadv::nal::Shape& shape)
{
    int bcitodo = 0;//circle
    switch(object.shape())
    {
    case Tiled::MapObject::Rectangle:
        {
            im::bci::newtonadv::nal::Rectangle& rectangle = *shape.mutable_rectangle();
            rectangle.set_width(object.width());
            rectangle.set_height(object.height());
            break;
        }
    case Tiled::MapObject::Polygon:
        {
            im::bci::newtonadv::nal::ConvexPolygon& polygon = *shape.mutable_polygon();
            Q_FOREACH(QPointF point, object.polygon())
            {
                im::bci::newtonadv::nal::Vertex& vertex = *polygon.add_vertices();
                vertex.set_x(point.x());
                vertex.set_y(point.y());
            }
            break;
        }
    case Tiled::MapObject::Polyline:
        break;
    }
}

bool NewtonAdventureWriter::convertExternalAnimation(im::bci::newtonadv::nal::AnimationReference& animationReference, const Tiled::Properties& properties, const QString& propertyBasename)
{
    QString gfxFile = properties[propertyBasename + ".file"];
    if(!gfxFile.isEmpty())
    {
        animationReference.set_file(gfxFile.toStdString());
        QString gfxAnimation = properties[propertyBasename + ".name"];
        if(!gfxAnimation.isEmpty())
            animationReference.set_name(gfxAnimation.toStdString());
        return true;
    } else
        return false;
}

void NewtonAdventureWriter::convertAnimation(im::bci::newtonadv::nal::AnimationReference& animationReference, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    if(convertExternalAnimation(animationReference, properties, "newton_adventure.animation"))
        return;
    else if(Tiled::Tile* tile = object.tile())
    {
        std::string name = getEntityTypeName(object).toStdString();
        animationReference.set_file(levelAnimationFileName_.toStdString());
        animationReference.set_name(name);
        if(!hasAnimation(name))
        {
            im::bci::nanim::Animation& animation =* levelAnimations_.add_animations();
            animation.set_name(name);

            im::bci::nanim::Frame& frame = *animation.add_frames();
            frame.set_duration(1000000);
            frame.set_imagename(name);
            frame.set_u1(0.0f);
            frame.set_v1(0.0f);
            frame.set_u2(1.0f);
            frame.set_v2(1.0f);

            QPixmap pixmap = tile->image();
            int w = pixmap.width();
            int h = pixmap.height();
            bool hasAlpha = pixmap.hasAlpha();
            im::bci::nanim::Image& image = *levelAnimations_.add_images();
            image.set_name(name);
            image.set_width(w);
            image.set_height(h);
            image.set_format(hasAlpha ? im::bci::nanim::RGBA_8888 : im::bci::nanim::RGB_888 );
            std::string& pixels = *image.mutable_pixels();
            pixels.reserve(w * h * (hasAlpha ? 4 : 3));
            QImage qimage = pixmap.toImage();
            for(int y=0;y<qimage.height(); ++y)
            {
                for(int x=0;x<qimage.width(); ++x)
                {
                    QRgb rgb = qimage.pixel(x, y);
                    pixels.push_back( qRed(rgb) );
                    pixels.push_back( qGreen(rgb) );
                    pixels.push_back( qBlue(rgb) );
                    if(hasAlpha)
                        pixels.push_back( qAlpha(rgb) );
                }
            }
        }
    }
}

bool NewtonAdventureWriter::hasAnimation(std::string name)
{
    for(int i = 0; i<levelAnimations_.animations_size(); ++i)
    {
        const im::bci::nanim::Animation& animation = levelAnimations_.animations(i);
        if(animation.name() == name)
            return true;
    }
    return false;
}

void NewtonAdventureWriter::convertPikes(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::Pikes& pikes = *entityType.mutable_pikes();

    QString dangerousSide = properties["newton_adventure.pikes.dangerous_side"];
    if(dangerousSide == "up")
        pikes.set_dangerous_side(im::bci::newtonadv::nal::Pikes_DangerousSide_UP);
    else if(dangerousSide == "down")
        pikes.set_dangerous_side(im::bci::newtonadv::nal::Pikes_DangerousSide_DOWN);
    else if(dangerousSide == "left")
        pikes.set_dangerous_side(im::bci::newtonadv::nal::Pikes_DangerousSide_LEFT);
    else if(dangerousSide == "right")
        pikes.set_dangerous_side(im::bci::newtonadv::nal::Pikes_DangerousSide_RIGHT);

    convertAnimation(*pikes.mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertPlatform(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::Platform& platform = *entityType.mutable_platform();

    platform.set_enabled( properties["newton_adventure.platform.enabled"] != "false" );

    QString friction = properties["newton_adventure.platform.friction"];
    if(!friction.isEmpty())
        platform.set_friction( friction.toFloat() );

    convertAnimation(*platform.mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertBouncePlatform(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_bounce_platform()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertCannon(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::Cannon& cannon = *entityType.mutable_cannon();

    QString orientation = properties["newton_adventure.cannon.orientation"];
    if(orientation == "up")
        cannon.set_orientation(im::bci::newtonadv::nal::Cannon_Orientation_UP);
    else if(orientation == "down")
        cannon.set_orientation(im::bci::newtonadv::nal::Cannon_Orientation_DOWN);
    else if(orientation == "left")
        cannon.set_orientation(im::bci::newtonadv::nal::Cannon_Orientation_LEFT);
    else if(orientation == "right")
        cannon.set_orientation(im::bci::newtonadv::nal::Cannon_Orientation_RIGHT);

    convertAnimation(*cannon.mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertMummy(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_mummy()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertBat(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_bat()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertApple(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_apple()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertCoin(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_coin()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertKey(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_key()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertDoor(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_door()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertDoorToBonusWorld(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_door_to_bonus_world()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertCloud(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_cloud()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertWorldMap(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_world_map()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertCompass(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_compass()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertMobilePikeAnchor(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::MobilePikeAnchor& mobilePikeAnchor = *entityType.mutable_mobile_pike_anchor();
    convertAnimation(*mobilePikeAnchor.mutable_animation(), object, properties);
    convertExternalAnimation(*mobilePikeAnchor.mutable_mobile_pikes_animation(), properties, "newton_adventure.mobile_pike_anchor.mobile_pike_animation");
}

void NewtonAdventureWriter::convertAxeAnchor(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::AxeAnchor& axeAnchor = *entityType.mutable_axe_anchor();
    convertAnimation(*axeAnchor.mutable_animation(), object, properties);
    convertExternalAnimation(*axeAnchor.mutable_axe_animation(), properties, "newton_adventure.axe_anchor.axe_animation");
}

void NewtonAdventureWriter::convertActivator(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::Activator& activator = *entityType.mutable_activator();
    activator.set_activableid(properties["newton_adventure.activator.activate_id"].toInt());
    convertExternalAnimation(*activator.mutable_on_animation(), properties, "newton_adventure.activator.on_animation");
    convertExternalAnimation(*activator.mutable_off_animation(), properties, "newton_adventure.activator.off_animation");
}

void NewtonAdventureWriter::convertMemoryActivator(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::MemoryActivator& activator = *entityType.mutable_memory_activator();
    activator.set_activableid(properties["newton_adventure.activator.activate_id"].toInt());
    convertExternalAnimation(*activator.mutable_on_animation(), properties, "newton_adventure.memory_activator.on_animation");
    convertExternalAnimation(*activator.mutable_off_animation(), properties, "newton_adventure.memory_activator.off_animation");
    convertExternalAnimation(*activator.mutable_hide_animation(), properties, "newton_adventure.memory_activator.hide_animation");
}

void NewtonAdventureWriter::convertMovingPlatform(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::MovingPlatform& movingPlatform = *entityType.mutable_moving_platform();
    convertAnimation(*movingPlatform.mutable_animation(), object, properties);
    const Tiled::MapObject* pathObject = findObjectByName(properties["newton_adventure.moving_platform.path"]);
    if(pathObject)
    {
        Q_FOREACH(QPointF point, pathObject->polygon())
        {
            im::bci::newtonadv::nal::Position& position = *movingPlatform.mutable_path()->add_positions();
            position.set_x(point.x());
            position.set_y(point.y());
        }
    }
}

void NewtonAdventureWriter::convertTeleporter(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::Teleporter& teleporter = *entityType.mutable_teleporter();
    convertAnimation(*teleporter.mutable_animation(), object, properties);
    teleporter.set_color(properties["newton_adventure.teleporer.color"].toStdString());
}

void NewtonAdventureWriter::convertKeyLock(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_keylock()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertHelpSign(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_help_sign()->mutable_animation(), object, properties);
}

void NewtonAdventureWriter::convertEgyptianBoss(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    im::bci::newtonadv::nal::EgyptianBoss& egyptianBoss = *entityType.mutable_egyptian_boss();
    convertExternalAnimation(*egyptianBoss.mutable_body_animation(), properties, "newton_adventure.egyptian_boss.body_animation");
    convertExternalAnimation(*egyptianBoss.mutable_hand_animation(), properties, "newton_adventure.egyptian_boss.hand_animation");
}

void NewtonAdventureWriter::convertHero(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties)
{
    convertAnimation(*entityType.mutable_hero()->mutable_animation(), object, properties);
}

