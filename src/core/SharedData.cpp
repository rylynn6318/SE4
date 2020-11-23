#include "core/SharedData.hpp"
#include <optional>

auto se4::SharedData::getSharedData(SharedDataID key) const ->std::any {
    std::optional<std::any>;
    if (sharedData.find(key) != sharedData.end())
        return sharedData.at(key);
    else
        return std::nullopt;
}

auto se4::SharedData::setSharedData(SharedDataID key, std::any data)->void {
    sharedData[key] = data;
}