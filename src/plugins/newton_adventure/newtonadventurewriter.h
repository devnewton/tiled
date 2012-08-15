/*
 * Copyright (c) 2012 devnewton <devnewton@bci.im>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of 'devnewton <devnewton@bci.im>' nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
    const im::bci::newtonadv::nal::EntityType& getOrCreateEntityType(const Tiled::MapObject& object);
    QString getEntityTypeName(const Tiled::MapObject& object);
    const im::bci::newtonadv::nal::EntityType* createEntityType(QString name, const Tiled::MapObject& object);
    bool hasAnimation(std::string name);
    const Tiled::MapObject* findObjectByName(QString name);
    void convertShape(const Tiled::MapObject& object, im::bci::newtonadv::nal::Shape& shape);
    void convertAnimation(im::bci::newtonadv::nal::AnimationReference& animationReference, const Tiled::MapObject& object, const Tiled::Properties& properties);
    bool convertExternalAnimation(im::bci::newtonadv::nal::AnimationReference& animationReference, const Tiled::Properties& properties, const QString& propertyBasename);
    void convertQImageToNanimImage(const QImage& qimage, im::bci::nanim::Image& image, const std::string& name) const;
    void convertPikes(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertPlatform(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertBouncePlatform(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCannon(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMummy(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertBat(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertApple(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCoin(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertKey(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertDoor(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertDoorToBonusWorld(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCloud(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertWorldMap(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertCompass(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMobilePikeAnchor(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertAxeAnchor(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertActivator(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMemoryActivator(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertMovingPlatform(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertTeleporter(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertKeyLock(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertHelpSign(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertEgyptianBoss(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);
    void convertHero(im::bci::newtonadv::nal::EntityType& entityType, const Tiled::MapObject& object, const Tiled::Properties& properties);

    QHash<QString, const im::bci::newtonadv::nal::EntityType* > entityTypesByName_;
    const Tiled::Map* map_;
    const QString fileName_, levelAnimationFileName_;
    QStringList errors_;
    im::bci::newtonadv::nal::Level level_;
    im::bci::nanim::Nanim levelAnimations_;
};

#endif // NEWTONADVENTUREWRITER_H
