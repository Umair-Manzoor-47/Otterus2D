#include "ContactListener.h"

namespace otterus_physics {
	void ContactListener::SetUserContacts(UserData* a, UserData* b)
	{
		m_UserDataA = a;
		m_UserDataB = b;
	}
	void ContactListener::BeginContact(b2Contact* contact)
	{
		UserData* a_data = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetUserData().pointer);
		UserData* b_data = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetUserData().pointer);
		SetUserContacts(a_data, b_data);
	}
	void ContactListener::EndContact(b2Contact* contact)
	{
		SetUserContacts(nullptr, nullptr);

	}
	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}
	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
	}
}