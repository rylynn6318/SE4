#include "core/FileManager.hpp"

namespace se4 {
	auto FileManager::setPath(std::filesystem::path p)-> void {
		this->path = p;
	}

	auto FileManager::getPath()->std::filesystem::path {
		return path;
	}
}
