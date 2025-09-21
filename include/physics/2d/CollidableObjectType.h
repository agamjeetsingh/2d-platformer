//
// Created by Agamjeet Singh on 19/07/25.
//

#ifndef COLLIDABLEOBJECTTYPE_H
#define COLLIDABLEOBJECTTYPE_H

enum class CollidableObjectType {
    Movable, // e.g. Players -- Can be moved by both movables and immovables.
    Immovable, // e.g. Walls -- Cannot be moved by either movables or immovables but can move on their own
};

#endif //COLLIDABLEOBJECTTYPE_H
