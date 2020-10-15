#include <entity/EntityManager.hpp>


namespace se4 {
	Entity EntityManager::createEntity() {
		lastEntity++;
		return { lastEntity };
	}

	void EntityManager::destroy(se4::Entity entity) {
		// Do nothing for now. This will be covered in a future blog post.
	}
}  // namespace se4
