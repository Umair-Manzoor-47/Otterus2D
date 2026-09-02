#pragma once
#include <Box2D/box2d.h>
#include <memory>

namespace otterus_physics {

    using PhysicsWorld = std::shared_ptr<b2World>;

    struct BodyDestroyer {
        void operator()(b2Body* body) const;
    };

    static std::shared_ptr<b2Body> MakeSharedBody(b2Body* body) {
        return std::shared_ptr<b2Body>(body, BodyDestroyer{});
    }
}