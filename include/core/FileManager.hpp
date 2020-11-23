#pragma once
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

namespace se4 {

	class FileManager {
	public:
		FileManager() = default;
		virtual ~FileManager() = default;
		auto setPath(std::filesystem::path p) -> void;
		auto getPath() -> std::filesystem::path;

	private:
		std::filesystem::path path;
	};
}