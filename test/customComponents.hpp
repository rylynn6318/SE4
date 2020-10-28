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

struct MovesetSpec : Component<MovesetSpec> //���������� ��� �̵��� ��ӿ, ������ �ִ� ���̿� x�� �̵� �Ÿ��� �����Ǿ�����
{
	float walkSpeed;
	float runningSpeed;
	float jumpHeight;
	float jumpVelocity;
};

struct Ki : Component<Ki>
{
	int ki;	//ǥ�ô� ki / n ��ŭ ���ڷ� ǥ��, ki % n ��ŭ ����� ǥ�� n�� �� ĭ�� �뷮
};

