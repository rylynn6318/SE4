#pragma once
#include "Entity.hpp"

namespace se4 {
	class EntityManager {
	public:
		Entity createEntity();
		void destroy(Entity entity);

	private:
		int lastEntity = 0;
	};
}  // namespace se4
