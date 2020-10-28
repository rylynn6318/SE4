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
	State2 state2;
};

struct Rigidity : Component<Rigidity>
{
	int rigidFrame;
};

struct Position : Component<Position>
{
	float x;
	float y;
};

struct Speed : Component<Speed>
{
	float xVeloticy;
	float yVelocity;
};

struct MovesetSpec : Component<MovesetSpec> //격투게임은 모든 이동이 등속운동, 점프의 최대 높이와 x축 이동 거리가 고정되어있음
{
	float walkSpeed;
	float runningSpeed;
	float jumpHeight;
	float jumpVelocity;
};

struct Ki : Component<Ki>
{
	int ki;	//표시는 ki / n 만큼 숫자로 표시, ki % n 만큼 막대로 표시 n은 한 칸의 용량
};

