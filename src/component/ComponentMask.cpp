#include <component/ComponentMask.hpp>
#include <se4.hpp>
namespace se4 {
	bool ComponentMask::isNewMatch(const ComponentMask &oldMask, const ComponentMask &systemMask) const {
		return matches(systemMask) && !oldMask.matches(systemMask);
	}

	bool ComponentMask::isNoLongerMatched(const ComponentMask &oldMask, const ComponentMask &systemMask) const {
		return oldMask.matches(systemMask) && !matches(systemMask);
	}

	bool ComponentMask::matches(se4::ComponentMask const & systemMask) const {
	    return (mask & systemMask.mask) == systemMask.mask;
	}
}  // namespace se4
