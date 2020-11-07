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

struct Score //승리한 라운드의
{
	int finScore; //게임을 이기기 위해 필요한 라운드의 수
	int score; //승리한 라운드의 수
	//이렇게 함으로써 얻을 수 있는 것 : 유저간 실력 차가 날 경우 A는 5판승, B는 1판승 이런식으로 승리 조건을 조정할 수 있음 default는 3이 적절할듯?
};

