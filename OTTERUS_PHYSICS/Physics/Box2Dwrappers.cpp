#include "Box2Dwrappers.h"

void otterus_physics::BodyDestroyer::operator()(b2Body* body) const
{
	body->GetWorld()->DestroyBody(body);
}
