//
//  RecurseOctreeToMapOperator.cpp
//  libraries/entities/src
//
//  Created by Seth Alves on 3/16/15.
//  Copyright 2013 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "RecurseOctreeToMapOperator.h"


RecurseOctreeToMapOperator::RecurseOctreeToMapOperator(QVariantMap& map, OctreeElement *top, QScriptEngine *engine) :
        RecurseOctreeOperator(),
        _map(map),
        _top(top),
        _engine(engine)
{
    // if some element "top" was given, only save information for that element and it's children.
    if (_top) {
        _withinTop = false;
    } else {
        // top was NULL, export entire tree.
        _withinTop = true;
    }
};

bool RecurseOctreeToMapOperator::preRecursion(OctreeElement* element) {
    if (element == _top) {
        _withinTop = true;
    }
    return true;
}

bool RecurseOctreeToMapOperator::postRecursion(OctreeElement* element) {

    EntityTreeElement* entityTreeElement = static_cast<EntityTreeElement*>(element);
    const QList<EntityItem*>& entities = entityTreeElement->getEntities();

    QVariantList entitiesQList = qvariant_cast<QVariantList>(_map["Entities"]);

    foreach (EntityItem* entityItem, entities) {
        EntityItemProperties properties = entityItem->getProperties();

        // XXX this is copied out of EntityScriptingInterface::getEntityProperties
        // is it needed here?

        // if (entityItem->getType() == EntityTypes::Model) {
        //     const FBXGeometry* geometry = getGeometryForEntity(entityItem);
        //     if (geometry) {
        //         properties.setSittingPoints(geometry->sittingPoints);
        //         Extents meshExtents = geometry->getUnscaledMeshExtents();
        //         properties.setNaturalDimensions(meshExtents.maximum - meshExtents.minimum);
        //     }
        // }

        QScriptValue qScriptValues =
            EntityItemPropertiesToScriptValue(_engine, properties);

        entitiesQList << qScriptValues.toVariant();
    }
    _map["Entities"] = entitiesQList;
    if (element == _top) {
        _withinTop = false;
    }
    return true;
}
