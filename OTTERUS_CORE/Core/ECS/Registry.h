#pragma once
#include <entt.hpp>

namespace otterus_core::ECS {
	class Registry
	{
	public:
		Registry();
		~Registry() = default;

		inline entt::registry& GetRegistry() { return *m_registry; }
		inline entt::entity CreateEntity() { return m_registry->create(); }

		template <typename TContext>
		TContext AddToContext(TContext context);

		template <typename TContext>
		TContext& GetContext();

	private:
		std::unique_ptr<entt::registry> m_registry; 

	};


	template<typename TContext>
	inline TContext Registry::AddToContext(TContext context)
	{
		return m_registry->ctx().emplace<TContext>(context);
	}

	template<typename TContext>
	inline TContext& Registry::GetContext()
	{
		return m_registry->ctx().get<TContext>();
	}

}