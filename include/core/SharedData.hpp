#pragma once
#include <map>

namespace se4 {
	class SharedData {
	public:
		auto getSharedData(SharedDataID key)const->std::any;
		auto setSharedData(SharedDataID key, std::any data)->void;
	private:
		std::map<SharedDataID, std::any> sharedData;
	};
}