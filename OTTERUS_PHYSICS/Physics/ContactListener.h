#pragma once

#include "Box2Dwrappers.h"
#include "UserData.h"

namespace otterus_physics {
	
	class ContactListener : public b2ContactListener
	{
	private:
		UserData* m_UserDataA{ nullptr };
		UserData* m_UserDataB{ nullptr };

		void SetUserContacts(UserData* a, UserData* b);

	public:
		
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
		
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

		inline const UserData* GetUserDataA() const { return m_UserDataA; };
		inline const UserData* GetUserDataB() const { return m_UserDataB; };


	};

}