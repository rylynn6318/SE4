#include <component/ComponentMask.hpp>
#include <se4.hpp>
namespace se4 {
	bool ComponentMask::isNewMatch(se4::ComponentMask& oldMask, se4::ComponentMask& systemMask) {
		return matches(systemMask) && !oldMask.matches(systemMask);
	}

	bool ComponentMask::isNoLongerMatched(se4::ComponentMask& oldMask, se4::ComponentMask& systemMask) {
		return oldMask.matches(systemMask) && !matches(systemMask);
	}

	bool ComponentMask::matches(se4::ComponentMask& systemMask) {
		for (int i = 0; i < 32; i++)
		{
			if ((mask[i] & systemMask.mask[i]) != systemMask.mask[i])
			{
				return false;
			}
		}

		return true;
	}
}  // namespace se4
