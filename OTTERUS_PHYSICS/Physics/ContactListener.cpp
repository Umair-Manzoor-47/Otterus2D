#include "ContactListener.h"
#include<logger/Logger.h>

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

		try {
			auto any_a = std::any_cast<ObjectData>(a_data->userData);
			auto any_b = std::any_cast<ObjectData>(b_data->userData);
			
			any_a.AddContact(any_b);
			any_b.AddContact(any_a);

			a_data->userData.reset();
			a_data->userData = any_a;

			b_data->userData.reset();
			b_data->userData = any_b;

			SetUserContacts(a_data, b_data);
		}
		catch (const std::bad_any_cast& ex) {
			OTTERUS_ERROR("Faild to cast user contacts {}", ex.what());
			SetUserContacts(nullptr, nullptr);
		}
	}
	void ContactListener::EndContact(b2Contact* contact)
	{
		UserData* a_data = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetUserData().pointer);
		UserData* b_data = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetUserData().pointer);

		try
		{
			auto a_any = std::any_cast<ObjectData>(a_data->userData);
			auto b_any = std::any_cast<ObjectData>(b_data->userData);

			if (!a_any.RemoveContact(b_any))
			{
				// TODO: LOG ERROR
			}

			if (!b_any.RemoveContact(a_any))
			{
				// TODO: LOG ERROR
			}

			a_data->userData.reset();
			a_data->userData = a_any;

			b_data->userData.reset();
			b_data->userData = b_any;

		}
		catch (const std::bad_any_cast& ex)
		{
			OTTERUS_ERROR("Failed to cast user contacts: {}", ex.what());
		}

		SetUserContacts(nullptr, nullptr);

	}
	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}
	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
	}
}