#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>

namespace se4 {

	class FileManager {
	public:
		FileManager();
		virtual ~FileManager() = default;
		auto setPath(std::filesystem::path p) -> void;
		auto getPath() const -> std::filesystem::path;
		auto inOpen(const char* fileName) -> bool;
		auto inClose() -> void;
		auto outOpen(const char* fileName) -> bool;
		auto outClose() -> void;

	private:
		std::filesystem::path path;
		std::ifstream in;
		std::ofstream out;
	};
}