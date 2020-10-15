#include <component/ComponentMask.hpp>
#include <se4.hpp>
namespace se4 {
	bool ComponentMask::isNewMatch(se4::ComponentMask oldMask, se4::ComponentMask systemMask) {
		return matches(systemMask) && !oldMask.matches(systemMask);
	}

	bool ComponentMask::isNoLongerMatched(se4::ComponentMask oldMask, se4::ComponentMask systemMask) {
		return oldMask.matches(systemMask) && !matches(systemMask);
	}

	bool ComponentMask::matches(se4::ComponentMask systemMask) { return ((mask & systemMask.mask) == systemMask.mask); }
}  // namespace se4
