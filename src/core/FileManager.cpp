#include "core/FileManager.hpp"
#include <iostream>

namespace se4 {
	 FileManager::FileManager() {
		 std::filesystem::create_directory("./save");
		 path = std::filesystem::current_path() / "save";
	}

	auto FileManager::setPath(std::filesystem::path p)-> void {
		this->path = p;
	}

	auto FileManager::getPath() const ->std::filesystem::path {
		return path;
	}

	auto FileManager::inOpen(const char* fileName)->bool {
		for (const std::filesystem::directory_entry& entry :
			std::filesystem::recursive_directory_iterator(path)) {
			if (entry.path().c_str() == path / fileName) {
				in.open(entry.path().c_str());
				if (in.is_open())
					return true;
				else
					return false;
			}
		}
		return false;
	}

	auto FileManager::inClose()->void {
		in.close();
	}
	auto FileManager::outOpen(const char* fileName) -> bool
	{
		for (const std::filesystem::directory_entry& entry :
			std::filesystem::recursive_directory_iterator(path)) {
			if (entry.path().c_str() == path / fileName) {
				out.open(entry.path().c_str());
				if (out.is_open())
					return true;
				else
					return false;
			}
		}
		return false;
	}
	auto FileManager::outClose() -> void
	{
		out.close();
	}
}
