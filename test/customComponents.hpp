#include <component/Component.hpp>
#include <StateEnum.hpp>

struct Health : Component<Health>
{
	float maxHP;
	float curruntHP;
	//float regeneratePerSec;
};

struct State : Component<State>
{
	State state;
	
};
