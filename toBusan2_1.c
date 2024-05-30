#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

// 기차길이
#define LEN_MIN 15   
#define LEN_MAX 50

//마동석 체력
#define STM_MIN 0    
#define STM_MAX 5

//확률
#define PROB_MIN 10  
#define PROB_MAX 90

//어그로 범위
#define AGGRO_MIN 0  
#define AGGRO_MAX 5

//마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1 
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2	

// 이거 나중에 정리해
int length, p, m_stm;
char characters[] = { 'C', 'Z', 'M' , 'V' }; // 0, 1, 2, 3
int sum1 = 0;
int turn = 0;
int c_aggro = 1, m_aggro = 1, v_aggro = 1;
int m_status, m_move;
int c, z, m, v, c_to_v, v_to_c;
int status;
int x;
int m_action_while;
int m_aggro_mid;
int z_atteck_m, z_atteck_c;
int zombie_cannotMove = 0;
int ZM_cannotMove;
int ZC_cannotMove;
int m_pull;
int gameOver = 0;
int stage = 0;
int baram, oneTime = 0, v_dead = 0;


//초기화, 초기 상태
void train_first() {
	do
	{
		printf("train length(15 ~ 50) >> ");
		scanf_s("%d", &length);
	} while (LEN_MAX < length || LEN_MIN > length);

	do
	{
		printf("madongseok stamina(0 ~ 5) >> ");
		scanf_s("%d", &m_stm);
	} while (STM_MAX < m_stm || STM_MIN > m_stm);

	do
	{
		printf("percentile probability 'p' (10 ~ 90) >> ");
		scanf_s("%d", &p);
	} while (PROB_MAX < p || PROB_MIN > p);
	printf("\n");
	c = length - 6;
	z = length - 3;
	m = length - 2;
	v = length - 5;
}

void CV_turn() {
	int r_move_c = rand() % 100;
	if (r_move_c <= 100 - p) {
		--c;
		--v;
		if (c_aggro == AGGRO_MAX)
		{
			c_aggro = c_aggro;
		}
		else
		{
			++c_aggro;
			++v_aggro;
		}
		++sum1;
	}
	else { //시민이 못움직일떄
		if (c_aggro == AGGRO_MIN) {
			c_aggro = c_aggro;
			v_aggro = v_aggro;
			sum1 = 0;
		}
		else {
			--c_aggro;
			--v_aggro;
			sum1 = 0;
		}
	}
}
void Z_turn() { //turn을 계산한것을 바탕으로 move에서 출력하기 때문에 프린트 X
	if (turn % 2 == 1) {
		if (m == z + 1 && c_aggro <= m_aggro) {
			++ZM_cannotMove;
		}
		else if (c == z - 1 && c_aggro > m_aggro) {
			++ZC_cannotMove;
		}
		else {
			if (m_aggro < c_aggro)
			{
				if (z != c + 1) //시민과 인접하지 않은 경우
				{
					--z;
				}
			}
			else
			{
				if (z != m - 1)
				{
					++z;
				}
			}
		}
	}
}

void V_turn() {
	int r_villian = rand() % 100;
	if (r_villian <= 90)
	{
		baram = 1;
		c_to_v = v;
		v_to_c = c;
	}
	else
	{
		if (baram == 1)
		{
			baram = 1;
		}
		else
		{
			baram = 0;
		}
	}

}

void CZ_turn() {
	CV_turn(); //빌런 추가됨
	Z_turn();
}

void character_position() {
	printf("#");
	for (int i = 1; i < length - 1; i++) {
		if (i == c) {
			printf("%c", characters[0]);
		}
		else if (i == z) {
			printf("%c", characters[1]);
		}
		else if (i == m) {
			printf("%c", characters[2]);
		}
		else {
			printf(" ");
		}
	}
	printf("#\n");
	for (int i = 0; i < length; i++) {
		printf("#");
	}
}

void character_position_V() {
	printf("#");
	for (int i = 1; i < length - 1; i++) {
		if (i == c) {
			if (baram == 1)
			{
				printf("%c", characters[3]);
			}
			else
			{
				printf("%c", characters[0]);
			}
		}
		else if (i == z) {
			printf("%c", characters[1]);
		}
		else if (i == m) {
			printf("%c", characters[2]);
		}
		else if (i == v) {
			if (baram == 1)
			{
				printf("%c", characters[0]);
			}
			else //baram == 0
			{
				if (v_dead == 0) //v_dead == 1 일경우 출력 X
				{
					printf("%c", characters[3]);
				}
			}
		}
		else {
			printf(" ");
		}
	}
	printf("#\n");
	for (int i = 0; i < length; i++) {
		printf("#");
	}
}

void train_start() {

	for (int i = 0; i < length; i++) {
		printf("#");
	}
	printf("\n");
	character_position();
	printf("\n\n");
}

void train_start_V() {

	for (int i = 0; i < length; i++) {
		printf("#");
	}
	printf("\n");
	character_position_V();
	printf("\n\n");
}

void game_over() {
	if (c == 1)
	{
		printf("YOU WIN! citizen escapes train\n");
		++stage;
		++gameOver;
		return;
	}
	else if (c == z + 1)
	{
		printf("GAME OVER! citizen dead...");
		exit(1);
	}
	else if (z_atteck_m == 1)
	{
		if (m_stm == 1 || m_stm == 0)
		{
			printf("GAME OVER! madongseok dead...");
			exit(1);
		}
	}

}

void game_over_stage() {
	if (c == 1)
	{
		if (stage == 3)
		{
			game_over();
		}
		else
		{
			++stage;
			printf("SUCCESS ! citizen escapes next train (stage : %d)\n", stage);
			gameOver = 1;
		}
	}
	else if (c == z + 1)
	{
		printf("GAME OVER! citizen dead...");
		exit(1);
	}
	else if (z_atteck_m == 1)
	{
		if (m_stm == 1 || m_stm == 0)
		{
			printf("GAME OVER! madongseok dead...");
			exit(1);
		}
	}
}
void game_over_stage_V() {
	if (c == 0)
	{
		if (stage == 4)
		{
			printf("SUCCESS ! citizen escapes train");
			exit(1);
		}
	}
	else if (c == z + 1)
	{
		printf("GAME OVER! citizen dead...");
		exit(1);
	}
	else if (z_atteck_m == 1)
	{
		if (m_stm == 1 || m_stm == 0)
		{
			printf("GAME OVER! madongseok dead...");
			exit(1);
		}
	}
	else if (baram == 1)
	{
		if (v == z + 1)
		{
			printf("GAME OVER! citizen dead...");
			exit(1);
		}
	}

}

// 1턴 <이동> 페이즈 (시민-좀비-마동석)

int y = 0;

void C_move() {
	if (sum1 == 1 && c_aggro < AGGRO_MAX) {
		printf("citizen : %d -> %d (aggro : %d -> %d)\n", c + 1, c, c_aggro - 1, c_aggro);
	}
	else if (sum1 == 1 && c_aggro == AGGRO_MAX)
	{
		printf("citizen : %d -> %d (aggro : %d)\n", c + 1, c, c_aggro);
	}
	else {
		printf("citizen : stay %d (aggro : %d -> %d)\n", c, c_aggro + 1, c_aggro);
	}
}
void Z_move() {
	if (m_pull == 1)
	{
		printf("zombie stay(pulled) %d\n", z);
	}
	else
	{
		if (turn % 2 == 1)
		{
			if (ZM_cannotMove == 1)
			{
				printf("zombie stay %d(right madongseok)\n", z);
			}
			else if (ZC_cannotMove == 1)
			{
				printf("zombie stay %d\n", z);
			}
			else
			{
				if (c_aggro > m_aggro)
				{
					printf("zombie : %d -> %d\n", z, z - 1);
				}
				else if (c_aggro <= m_aggro)
				{
					printf("zombie : %d -> %d\n", z - 1, z);
				}
			}
		}
		else
		{
			printf("zombie stay(turn) %d\n", z);

		}
	}
	printf("\n");
}

void CV_move() {
	if (sum1 == 1 && c_aggro < AGGRO_MAX) {
		if (baram == 1) //발암 == 1 > 시민 위치가 빌런위치 바뀜 c_to_v = v, v_to_c = c
		{
			printf("citizen : %d -> %d (aggro : %d -> %d)\n", c_to_v + 1, c_to_v, c_aggro - 1, c_aggro);
		}
		else
		{
			printf("citizen : %d -> %d (aggro : %d -> %d)\n", c + 1, c, c_aggro - 1, c_aggro);
		}
		if (stage == 4)
		{
			if (baram == 1)
			{
				printf("villian : %d -> %d (aggro : %d -> %d)\n", v_to_c + 1, v_to_c, v_aggro - 1, v_aggro);
			}
			else
			{
				printf("villian : %d -> %d (aggro : %d -> %d)\n", v + 1, v, v_aggro - 1, v_aggro);
			}
		}
	}
	else if (sum1 == 1 && c_aggro == AGGRO_MAX)
	{
		if (baram == 1)
		{
			printf("citizen : %d -> %d (aggro : %d)\n", c_to_v + 1, c_to_v, v_aggro);
		}
		else
		{
			printf("citizen : %d -> %d (aggro : %d)\n", c + 1, c, c_aggro);
		}
		if (stage == 4)
		{
			if (baram == 1)
			{
				printf("villian : %d -> %d (aggro : %d)\n", v_to_c + 1, v_to_c, v_aggro);
			}
			else
			{
				printf("villian : %d -> %d (aggro : %d)\n", v + 1, v, v_aggro);
			}
		}
	}
	else {
		if (baram == 1)
		{
			printf("citizen stay %d (aggro : %d -> %d)\n", c_to_v, c_to_v + 1, v_aggro);
		}
		else
		{
			printf("citizen : stay %d (aggro : %d -> %d)\n", c, c_aggro + 1, c_aggro);
		}
		if (stage == 4)
		{
			if (baram == 1)
			{
				printf("villian : stay %d (aggro : %d -> %d)\n", v_to_c, v_to_c + 1, c_aggro);
			}
			else
			{
				printf("villian : stay %d (aggro : %d -> %d)\n", v, v_aggro + 1, v_aggro);
			}
		}
	}
}

void CZ_move() { //프린트 부분
	train_start();
	C_move();
	Z_move();
}

void CZ_move_V() { //프린트 부분
	train_start_V();
	CV_move();
	Z_move();
}

void m_move_Q() {
	int a, b = 0;
	do
	{
		if (m == z + 1) {
			printf("madongseok move(0 : stay) >> ");
			a = 1;
		}
		else {
			printf("madongseok move(0 : stay, 1 : left) >> ");
			a = 2;
		}
		scanf_s("%d", &m_move);
		if (m_move == MOVE_STAY && a == 1)
		{
			b = 1;
		}
		else if (m_move == MOVE_STAY || m_move == MOVE_LEFT)
		{
			if (a == 2)
			{
				b = 1;
			}
		}
	} while (b != 1);
}

void M_move_LEFT() {
	if (m_move == MOVE_LEFT)
	{
		--m;
		y = 1;
		if (stage == 4)
		{
			train_start_V();
		}
		else
		{
			train_start();
		}
		if (m_aggro == AGGRO_MAX) //AGGRO_MAX일때 움직이면 어그로 그대로
		{
			printf("madongseok move %d -> %d (aggro : %d, stamina : %d)\n", m + 1, m, m_aggro, m_stm);
		}
		else
		{
			++m_aggro;
			printf("madongseok move %d -> %d (aggro : %d -> %d, stamina : %d)\n", m + 1, m, m_aggro - 1, m_aggro, m_stm);
		}
	}
}

void M_move_STAY() {
	if (m_move == MOVE_STAY)
	{
		y = 1;
		if (stage == 4)
		{
			train_start_V();
		}
		else
		{
			train_start();
		}
		if (m_aggro == AGGRO_MIN)
		{
			printf("madongseok stay %d (aggro : %d, stamina : %d)\n", m, m_aggro, m_stm);
		}
		else
		{
			--m_aggro;
			printf("madongseok stay %d (aggro : %d -> %d, stamina : %d)\n", m, m_aggro + 1, m_aggro, m_stm);
		}
	}
}

void M_move() {
	y = 0;
	do
	{
		m_move_Q();
		M_move_LEFT();
		M_move_STAY();
	} while (!y);
	printf("\n");
}


void m_action_Q() {
	do
	{
		if (m == z + 1) //mz 인접 할경우
		{
			if (m_stm == 0 || m_stm == 1)
			{
				printf("madongseok action (0.rest, 1.provoke) >> ");
				scanf_s("%d", &status);
				if (status == 0 || status == 1)
				{
					++m_action_while;
				}
			}
			else
			{
				printf("madongseok action (0.rest, 1.provoke, 2. pull) >> ");
				scanf_s("%d", &status);
				if (status == 0 || status == 1 || status == 2)
				{
					++m_action_while;
				}
			}

		}
		else //mz 인접 안할경우
		{
			printf("madongseok action (0.rest, 1.provoke)");
			scanf_s("%d", &status);
			if (status == 0 || status == 1)
			{
				++m_action_while;
			}
		}

	} while (m_action_while == 0);
}

void m_action_contact() {
	if (m == z + 1) // 인접할 경우(m_stm 무조건 깎임 X --> 공격당할때만 깎임), m_action_Q 에서 status가 0,1,2인 경우 밖에 없음
	{
		if (status == ACTION_REST) //status == 0
		{
			if (m_stm == STM_MAX)
			{
				m_stm = m_stm;
			}
			else
			{
				++m_stm;
			}
			if (m_aggro == AGGRO_MIN) //m_aggro == 0
			{
				printf("madongseok rests...\nmadongseok : %d (aggro : %d, stamina : %d -> %d)\n", m, m_aggro, m_stm - 1, m_stm);
			}
			else
			{
				--m_aggro;
				printf("madongseok rests...\nmadongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", m, m_aggro + 1, m_aggro, m_stm - 1, m_stm);
			}
		}
		else if (status == ACTION_PROVOKE)
		{
			if (m_aggro == AGGRO_MAX)
			{
				printf("madongseok provoked zombie...\nmadongseok : %d (aggro %d, stamina : %d)\n", m, m_aggro, m_stm);
			}
			else
			{
				m_aggro_mid = m_aggro;
				m_aggro = AGGRO_MAX;
				printf("madongseok provoked zombie...\nmadongseok : %d (aggro %d -> %d, stamina : %d)\n", m, m_aggro_mid, m_aggro, m_stm);
			}
		}
		else //status == ACTION_PULL
		{
			if (m_stm == 1 || m_stm == 0)
			{
				//입력 못하게해야됨
				--m_stm;
				game_over();
			}
			else
			{
				--m_stm;
			}
			if (m_aggro <= 3)
			{
				m_aggro += 2;
			}
			else if (m_aggro == 4)
			{
				m_aggro += 1;
			}
			else
			{
				m_aggro = m_aggro;
			}
			int r_pull_m = rand() % 100;
			if (r_pull_m <= 100 - p)
			{
				++m_pull;
				printf("madongseok pulled zombie... Next turn, it's can't move\nmadongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n\n\n", m, m_aggro - 2, m_aggro, m_stm + 1, m_stm);
			}
			else
			{
				printf("madongseok failed to pull zombie\nmadongseok : %d (aggro : %d -> %d, stamina: %d -> %d)\n\n\n", m, m_aggro - 2, m_aggro, m_stm + 1, m_stm);
			}
		}
	}
}
void m_action_notContact() {
	if (m != z + 1)//인접하지 않을경우 (pull 없음)
	{
		if (status == ACTION_REST)
		{
			if (m_stm == STM_MAX)
			{
				m_stm = m_stm;
			}
			else
			{
				++m_stm;
			}
			if (m_aggro == AGGRO_MAX) //m_aggro = 5
			{
				--m_aggro;
				if (m_stm == STM_MAX)
				{
					printf("madongseok rests...\nmadongseok : %d (aggro : %d -> %d, stamina %d)\n", m, m_aggro + 1, m_aggro, m_stm);
				}
				else
				{
					printf("madongseok rests...\nmadongseok : %d (aggro :%d -> %d, stamina %d -> %d)\n", m, m_aggro + 1, m_aggro, m_stm - 1, m_stm);
				}
			}
			else if (m_aggro == AGGRO_MIN) //m_aggro == 0
			{
				if (m_stm == STM_MAX)
				{
					printf("madongseok rests...\nmadongseok : %d (aggro : %d, stamina : %d)\n", m, m_aggro, m_stm);
				}
				else
				{
					printf("madongseok rests...\nmadongseok : %d (aggro : %d, stamina : %d -> %d)\n", m, m_aggro, m_stm - 1, m_stm);
				}
			}
			else
			{
				--m_aggro;
				printf("madongseok rests...\nmadongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", m, m_aggro + 1, m_aggro, m_stm - 1, m_stm);
			}
		}
		else if (status == ACTION_PROVOKE)
		{
			if (m_aggro == AGGRO_MAX)
			{
				printf("madongseok provoked zombie...\nmadongseok : %d (aggro %d, stamina : %d)\n", m, m_aggro, m_stm);
			}
			else
			{
				m_aggro_mid = m_aggro;
				m_aggro = AGGRO_MAX;
				printf("madongseok provoked zombie...\nmadongseok : %d (aggro %d -> %d, stamina : %d)\n", m, m_aggro_mid, m_aggro, m_stm);
			}
		}
	}
}

void m_action() {
	m_action_Q();
	printf("\n");
	m_action_contact();
	m_action_notContact();

}


void CZM_status() {
	if (sum1) {
		printf("citizen move.\n");
	}
	else {
		printf("citizen deos nothing\n");
	}
	if (stage == 4)
	{
		/*if (c == 0)
		{
			game_over_stage;
		}
		if (baram == 1)
		{
			if (c_to_v == -1 || v_to_c == 0)
			{
				game_over();
			}
		}
		else
		{
			if (v == 0 || c == 1)
			{
				game_over();
			}
		}*/
		if (oneTime != 1)
		{
			if (baram == 1) //빌런 출력 부분
			{
				oneTime = 1;
				printf("villian has switched location with the citizen\n");

			}
			else if (baram == 0)
			{
				printf("villian couldn't change locations with the citizen\n");
			}
		}
		if (baram == 0)
		{
			if (v == z + 1)
			{
				printf("zombie attecked villian\n");
				v_dead = 1;
			}
		}
		else //baram == 1 이므로 위치 : VC ( 보이는건 V가 앞이지만 변수는 v = 9 이면, c = 8 )
		{ 
			if (v == z + 1)
			{
				printf("zobie attecked citizen");
				game_over_stage_V();
			}
		}

	}

	if (c == z - 1) {
		game_over();
	}
	else if (m == z + 1) {
		if (m_stm == 1 || m_stm == 0)
		{
			++z_atteck_m;
			if (stage == 0)
			{
				game_over();
			}
			else
			{
				game_over_stage();
			}
		}
		--m_stm;
		printf("zombie attcked madongseok (madongseok stamina : %d -> %d)", m_stm + 1, m_stm);
	}
	else if (c == z - 1 && m == z + 1)
	{
		if (m_stm == 1 || m_stm == 0)
		{
			++z_atteck_m;
			if (stage == 0)
			{
				game_over();
			}
			else
			{
				game_over_stage();
			}
		}
		--m_stm;
		printf("zombie atteck madongseok (aggro : %d vs %d, madongseok stamina : %d -> %d)", c_aggro, m_aggro, m_stm + 1, m_stm);
	}
	else {
		printf("zombie attcked nobody");
	}
	printf("\n");
	if (stage == 0)
	{
		if (c != 1)
		{
			m_action();
		}
		else
		{
			game_over();
		}
	}
	else if (stage == 4)
	{
		if (c != 0)
		{
			m_action();
		}
		else // c == 0
		{
			game_over_stage_V(); //이부분땜에 이상하게 끝남에휴
		}
	}
	else
	{
		if (c != 1)
		{
			m_action();
		}
		else
		{
			game_over_stage(); //이부분땜에 이상하게 끝남에휴
		}
	}
}


void functionReset() {
	x = 0;
	sum1 = 0;
	++turn;
	ZM_cannotMove = 0;
	ZC_cannotMove = 0;
	m_action_while = 0;
	m_move;
}

void reset() {
	turn = 0;
	gameOver = 0;
	c = length - 6;
	z = length - 3;
	m = length - 2;
	v = length - 5;
	sum1 = 0;
	c_aggro = 1;
	m_aggro = 1;
	v_aggro = 1;
}


int main() {
	// 2 스테이지
	srand((unsigned int)time(NULL));
	train_first(); //열차 길이, 마동석 stm, 확률 설정
	train_start();
	while (stage == 0)
	{
		functionReset();
		printf("\nturn : %d\n", turn);
		CZ_turn(); //C_turn > Z_turn
		CZ_move(); //train_start > C_move > Z_move	
		m_pull = 0;
		M_move();
		CZM_status(); //m_action
	}

	// 3 - 1 스테이지
	printf("\n%d\n", stage);
	printf("\n==========================\n  3 - 1 STAGE ( %d  / 3 )\n==========================\n\n", stage);

	train_first(); //열차 길이, 마동석 stm, 확률 설정
	while (stage != 4)
	{
		reset();
		train_start(); //열차 출력
		while (gameOver != 1)
		{
			functionReset();
			printf("\nturn : %d\n", turn);
			CZ_turn(); //CZ_turn > Z_turn
			CZ_move(); //train_start_V > C_move > Z_move	
			m_pull = 0;
			M_move();
			CZM_status(); //m_action
			if (c == 1)
			{
				if (stage < 4)
				{
					printf("\n==========================\n  3 - 1 STAGE ( %d  / 3 )\n==========================\n\n", stage);
				}
			}
		}
	}
	//3-2 스테이지
	printf("\n===============\n  3 - 2 STAGE\n===============\n\n");
	reset();  //c,z,m,v, 어그로, turn 초기화
	train_start_V();
	while (stage == 4)
	{
		functionReset();
		printf("\nturn : %d\n", turn);
		CZ_turn(); //CV_turn,Z_turn 
		CZ_move_V(); //train_start_V > C_move > Z_move	
		m_pull = 0;
		V_turn();
		M_move();
		CZM_status(); //m_action
	}
	exit(1);
	return 0;

}