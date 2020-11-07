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
	State1 state;
	State2 state2;
};

struct Rigidity : Component<Rigidity>
{
	int rigidFrame;
};

struct Position : Component<Position>
{
	Position(float x, float y) : posX(x), posY(y){}
	float posX;
	float posY;
};

struct Speed : Component<Speed>
{
	Speed(float velX, float velY) : xVelocity(velX), yVelocity(velY){}
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

struct Score //�¸��� ������
{
	int finScore; //������ �̱�� ���� �ʿ��� ������ ��
	int score; //�¸��� ������ ��
	//�̷��� �����ν� ���� �� �ִ� �� : ������ �Ƿ� ���� �� ��� A�� 5�ǽ�, B�� 1�ǽ� �̷������� �¸� ������ ������ �� ���� default�� 3�� �����ҵ�?
};

