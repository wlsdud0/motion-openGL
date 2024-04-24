#include <Windows.h>
#include <MMSystem.h>
#include <GL/glut.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>

static int Day = 0, Time = 0; // 공 time 변수 
static double time = 0;       // 걸을때, 뛸때 time 변수
static double time2 = 0;      // Jug_time 변수
static double time3 = 0;      // 뛸 때 지면 time변수

GLfloat R_Arm_x = 0; // 오른쪽 어깨
GLfloat R_Arm_y = 0; // 오른 팔
GLfloat L_Arm_x = 0; // 왼쪽 어깨
GLfloat L_Arm_y = 0; // 왼팔
GLfloat R_Leg_x = 0; // 오른쪽 허벅지
GLfloat R_Leg_y = 0; // 오른쪽 종아리
GLfloat L_Leg_x = 0; // 왼쪽 허벅지
GLfloat L_Leg_y = 0; // 왼쪽 종아리

GLUquadricObj* cyl;  // 실린더 객체 선언

int a = 0; // x축 기준(값이 1일 때 x축으로 회전)
int b = 0; // y축 기준(값이 1일 때 y축으로 회전)
int c = 0; // z축 기준(값이 1일 때 z축으로 회전)

static int flag = 0; // wireframe모드와 soild rendering모드 구분 변수
static int key = 0;  // 동작모드

#define  RUN 1
#define  JUG 2
#define INSA 3

// gl 초기화함수
void glInit(void) { 
	glEnable(GL_DEPTH_TEST); // 깊이 테스팅 사용
	glEnable(GL_NORMALIZE);  // 정규화
	glEnable(GL_SMOOTH);     //각 정점의 색상을 부드럽게 연결하여 칠해지도록
	glEnable(GL_LIGHTING);   // gl 조명, 빛을 받는 각도에 따라 로봇 색상이 변화
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 400.0, 300.0, -700.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glClearColor(1.0, 1.0, 1.0, 1.0); // 배경은 흰색
	glMatrixMode(GL_PROJECTION);      // 모드설정
	glLoadIdentity();                 // CTM값 초기화
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // 가시범위설정
}

// wire, solid 선택
void Change_Wire_Or_Solid(int i) { 
	if (flag == 1)
		gluQuadricDrawStyle(cyl, GLU_LINE); // 스타일을 와이어로 변경
}

// 왼팔을 그리는 함수
void DrawL_Arm(int x, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.2, 0.0); // 빨간색, 옷 입음(반팔)
	cyl = gluNewQuadric();    // 실린더 객채생성
	glRotatef(x, a, b, c);    // 파라미터 값만큼 회전
	glRotatef(90.0, 1.0, 0.0, 0.0); // 실린더를 x축 기준으로 90도 회전(실린더 생성시 각도 변경을 위해)
	glTranslatef(0.25, 0.0, 0.0);   //왼쪽 어깨 시작점으로 이동
	glRotatef(15.0, 0.0, 1.0, 0.0); //왼쪽 어깨를 y축 기준으로 15도 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1); // 실린더 그림
}

// 왼손을 그리는 함수
void DrawL_Hand(int y, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1);     // 노란색, 옷 안입음
	cyl = gluNewQuadric();        // 실린더 객채생성
	glTranslatef(0.0, 0.0, 0.22); // 왼손 시작점
	glRotatef(y, a, b, c);        // 파라미터 값만큼 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 15, 1); // 실린더를 그림
	glPopMatrix();
}

// 오른팔을 그리는 함수
void DrawR_Arm(int x, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.2, 0.0);        // 빨간색, 옷 입음(반팔)
	cyl = gluNewQuadric();           // 실린더 객채생성
	glRotatef(x, a, b, c);           // 파라미터 값만큼 회전
	glRotatef(90.0, 1.0, 0.0, 0.0);  // 실린더를 x축 기준으로 -90도 회전(실린더 생성시 각도 변경을 위해)
	glTranslatef(-0.25, 0.0, 0.0);   // 오른쪽 어깨 시작점으로 이동
	glRotatef(-15.0, 0.0, 1.0, 0.0); //오른쪽 어깨를 y축 기준으로 -15도 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1); // 실린더 그림
}

// 오른손을 그리는 함수
void DrawR_Hand(int y, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1);     // 노란색, 옷 안입음
	cyl = gluNewQuadric();        // 실린더 객채생성
	glTranslatef(0.0, 0.0, 0.22); // 오른손 시작점
	glRotatef(y, a, b, c);        // 파라미터 값만큼 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1); // 실린더를 그림
	glPopMatrix();
}

// 몸체를 그리는 함수
void DrawBody(int x, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.2, 0.0);       // 빨간색, 옷 입음(반팔)
	cyl = gluNewQuadric();          // 실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0); // 실린더를 x축 기준으로 90도 회전(실린더 생성 시 각도 변경을 위해)
	glRotatef(x, a, b, c);          // 파라미터 값만큼 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.2, 0.2, 0.15, 50, 1); // 실린더를 그림
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1);       // 노란색, 옷 안입음(뱃살)
	cyl = gluNewQuadric();          // 실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0); // 실린더를 x축 기준으로 90도 회전(실린더 생성 시 각도 변경을 위해)
	glRotatef(x, a, b, c);          //파라미터 값만큼 회전
	glTranslatef(0.0, 0.0, 0.15);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.2, 0.2, 0.25, 50, 1); //실린더를 그림
	glPopMatrix();
}

// 왼쪽 허벅지를 그리는 함수
void DrawL_Legs(int x, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1);       // 노란색, 옷안입음
	cyl = gluNewQuadric();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.1, 0.0, 0.42);   // 위치
	glRotatef(x, a, b, c);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.15, 50, 1);
}

// 왼쪽 종아리를 그리는 함수
void DrawL_foot(int y, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1); // 노란색, 옷안입음
	cyl = gluNewQuadric();
	glTranslatef(0.0, 0.0, 0.18); // 위치
	glRotatef(y, a, b, c);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 50, 1);
	glPopMatrix();
}

// 오른쪽 허벅지를 그리는 함수
void DrawR_Legs(int x, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1); // 노란색, 옷안입음
	cyl = gluNewQuadric();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(-0.1, 0.0, 0.42);
	glRotatef(x, a, b, c);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.15, 50, 1);
}

// 오른쪽 종아리를 그리는 함수
void DrawR_foot(int y, int a, int b, int c) {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1); // 노란색, 옷안입음
	cyl = gluNewQuadric();
	glTranslatef(0.0, 0.0, 0.18);
	glRotatef(y, a, b, c);
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.05, 0.05, 0.2, 15, 1);
	glPopMatrix();
}

// 목을 그리는 함수
void DrawNeck() {
	glPushMatrix();
	glColor3f(1.0, 0.7, 0.1);
	cyl = gluNewQuadric();          // 실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0); // 실린더를 x축 기준으로 90도 회전(실린더 생성시 각도 변경을 위해)
	glTranslatef(0.0, 0.0, -0.045); // 목 시작점
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl, 0.2, 0.2, 0.025, 100, 1); // 실린더를 그림
	glPopMatrix();
}

// 지면을 그리는 함수
void DrawGround() {
	glColor3f(0.0, 0.8, 0.0);
	glTranslatef(0.0, -2.73, 0.0);  // 지면 시작점
	glRotatef(45.0, 0, 1, 0);       //45도만큼 y축을 기준으로 회전
	if (key == RUN) glRotatef(0.0 + time3 - 15, 0.0, 0.0, 1);
	else if (key == JUG) glRotatef(0.0 + time2 - 15, 0.0, 0.0, 1); // z축을 기준으로 time2만큼 지면이 회전
	else if (key == INSA) {
		glColor3f(0.529412, 0.807843, 0.980392);  // 인터넷 RGB색상에서 얻어옴
		glRotatef(0.0 + time2 - 15, 0.0, 0.0, 1); // 강물이 흐르는 속도, time2만큼 강물이 흐름
	}
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl, 2.5, 30, 90);
}

// 머리를 그리는 함수(귀, 눈, 주둥이 + 코)
void DrawHead() {
	glTranslatef(0.0, 0.02, 0.0); // 머리시작점
	glPushMatrix();               // 최초 저장 좌표계 다시저장
	cyl = gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl, 0.20, 30, 10); // 머리그리기
	glColor3f(1.0, 0.7, 0.1);

	//왼쪽 귀 그리기
	glRotatef(90.0, 1.0, 0.0, 0.0);  // 실린더 90도 x축을 기준으로 회전
	glTranslatef(-0.16, 0.0, -0.22); // 왼쪽 귀 시작점
	glRotatef(35.0, 0.0, 1.0, 0.0);  // 35도 y 축을 기준으로 회전
	gluCylinder(cyl, 0.05, 0.07, 0.1, 3, 1); // 실린더를 그림
	glPopMatrix();
	glPushMatrix();

	// 오른쪽 귀 그리기
	glRotatef(90.0, 1.0, 0.0, 0.0);   // 실린더 90도 x축을 기준으로 회전
	glTranslatef(0.16, 0.0, -0.22);   // 오른쪽 귀 시작점
	glRotatef(-35.0, 0.0, 1.0, 0.0);  // -35도 y 축을 기준으로 회전
	gluCylinder(cyl, 0.05, 0.07, 0.1, 3, 1); // 실린더를 그림
	glPopMatrix();
	glPushMatrix();

	// 왼쪽 눈 그리기
	glTranslatef(-0.1, 0.1, 0.13); // 왼쪽 눈 시작점
	glColor3f(0.0, 0.0, 0.0);      //눈컬러 설정(흑색)
	gluSphere(cyl, 0.025, 10, 10);
	glPopMatrix();
	glPushMatrix();

	// 오른쪽 눈 그리기
	glTranslatef(0.1, 0.1, 0.13); // 오른쪽 눈 시작점
	gluSphere(cyl, 0.025, 10, 10);
	glPopMatrix();
	glPushMatrix();

	// 주둥이 만들기
	glRotatef(90.0, 1.0, 0.0, 0.0);  // 실린더 90도 x축을 기준으로 회전
	glTranslatef(0.02, 0.15, -0.04); // 주둥이 시작점
	glRotatef(35.0, 0.0, 1.0, 0.0);  // 35도 y 축을 기준으로 회전
	glColor3f(1.0, 0.7, 0.1);
	gluSphere(cyl, 0.08, 10, 10);
	glPopMatrix();
	glPushMatrix();

	// 코 만들기
	glTranslatef(0.02, 0.035, 0.21); // 코 시작점
	glColor3f(0.0, 0.0, 0.0);        // 눈컬러 설정(흑색)
	gluSphere(cyl, 0.03, 10, 10);
	glPopMatrix();
	glPushMatrix();
}

// 안드로이드 푸 전체를 그리는 함수
void DrawAndroid() {
	DrawBody(0, 0, 0, 0);         // 몸통함수 호출
	DrawNeck();                   // 목
	DrawHead();                   // 머리
	DrawR_Arm(R_Arm_x, 1, 0, 0);  // 오른팔
	DrawR_Hand(R_Arm_y, 1, 0, 0); // 오른손
	DrawL_Arm(L_Arm_x, 1, 0, 0);  // 왼팔
	DrawL_Hand(L_Arm_y, 1, 0, 0); // 왼손
	DrawL_Legs(L_Leg_x, 1, 0, 0); // 왼쪽 다리
	DrawL_foot(L_Leg_y, 1, 0, 0); // 왼쪽 발
	DrawR_Legs(R_Leg_x, 1, 0, 0); // 오른쪽 다리
	DrawR_foot(R_Leg_y, 1, 0, 0); // 오른쪽 발
}

// 푸가 달리는 것을 표현한 함수
void Run() {
	glLoadIdentity();          // CTM초기화
	L_Arm_x = sin(time) * 80; // 왼팔은 80도까지 움직이되 sin()으로주기적인 움직임 설정
	R_Arm_x = -L_Arm_x;       // 오른팔은 왼팔 반대 방향 80도 각도까지
	R_Arm_y = -abs(sin(time) * 60 + 50);  // 오른팔 각도조절(절댓값을 줌으로써 팔이 뒤로 꺽이지 않음)
	L_Arm_y = -abs(-sin(time) * 60 + 50); // 왼팔 각도조절
	R_Leg_y = abs(-sin(time) * 30 - 30);  // 오른쪽 종아리 각도조절
	L_Leg_y = abs(sin(time) * 30 - 30);   // 왼쪽 종아리 각도조절
	R_Leg_x = sin(time) * 60; // 오른쪽 다리는 60도까지 움직이되 sin()으로 주기적인 움직임 설정
	L_Leg_x = -R_Leg_x;       //왼쪽다리는 오른쪽 다리 반대로

	cyl = gluNewQuadric(); // 실린더 객체생성
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 초기화
	glMatrixMode(GL_MODELVIEW); // 모드설정

	DrawGround();     // 지면을 그림
	glLoadIdentity(); // CTM초기화

	// 로봇이 달리면서 앞, 뒤로 뒤뚱거리고 몸이 틀어지는 것을 표현
	glRotatef(-230.0, 0, 1, 0);              // y축 기준으로 회전
	glRotatef(abs(sin(time) * 16), 1, 0, 0); // x축을 기준으로 16도까지 각도가 틀어짐.
	glRotatef(sin(time) * 16, 0, 1, 0);      // y축을 기준으로 16도까지 각도가 틀어짐 sin()함수로 주기적인 움직임 설정

	// 로봇이 달리면서 상하로 움직이는 것을 표현
	float i = 0;
	i = abs(sin(time)*0.08); // i 변수 값
	glPushMatrix();          // 최초 저장 좌표계 다시저장
	glTranslatef(0.0, i, 0); // 로봇의 몸체가 y축 방향으로 변수 i만큼 이동
	glTranslated(0.0, 0.5, 0.0); // 로봇의 최초위치
	DrawAndroid();
	glutSwapBuffers();
}

// 저글링할 때 돌아가는 공 함수
void Ball() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cyl = gluNewQuadric();
	Change_Wire_Or_Solid(flag);
	glTranslatef(0.3, 0.5, 0.0); // 중점의 x,y좌표 변경
	
	glPushMatrix(); // 공1 : 빨간색 공
	glRotatef(Day, 0.0, 0.3, 1.0);
	glTranslatef(-0.3, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glRotatef((GLfloat)Time, 0.0, 0.0, 1.0);
	gluSphere(cyl, 0.05, 10, 10);
	glPopMatrix();

	glPushMatrix(); // 공2 : 파란색 공
	glRotatef(Day, 0.0, 0.3, 1.0);
	glTranslatef(0.3, 0.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glRotatef((GLfloat)Time, 0.0, 0.0, 1.0);
	gluSphere(cyl, 0.05, 10, 10);
	glPopMatrix();
}

// 푸가 저글링하는 함수
void Jug() {
	glLoadIdentity();   // CTM초기화

	L_Arm_x = -50;      // 왼쪽 어깨의 각도 시작은 -50 상태
	R_Arm_x = L_Arm_x;  // 오른쪽 어깨의 각도 시작은 -50 상태에서 왼쪽어깨 움직이므이 반대로 설정
	R_Arm_y = -abs(cos(time2) * 40);  // 오른팔 각도조절, 손을 위로 튕기는 듯이 표현
	L_Arm_y = -abs(-sin(time2) * 40); // 왼팔 각도조절, 왼쪽 손은 오른쪽 손과 반대로 움직이게 표현
	R_Leg_y = abs(-sin(time) * 5);    // 오른쪽 종아리 각도조절(절댓값을 사용해 앞으로 꺾이지 않게)
	L_Leg_y = abs(sin(time) * 5);     // 왼쪽 종아리 각도조절
	R_Leg_x = sin(time) * 10;         // 오른쪽 다리는 10도까지. sin()함수로 주기적인 움직임 설정
	L_Leg_x = -R_Leg_x;               // 왼쪽다리는 오른쪽 다리 반대로

	cyl = gluNewQuadric(); // 실린더 객체생성
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 초기화
	glMatrixMode(GL_MODELVIEW); // 모드설정

	Ball(); // 돌아가는 공을 호출하는 함수
	glLoadIdentity(); //CTM초기화

	DrawGround();     // 지면을 그림
	glLoadIdentity(); //CTM초기화

	glRotatef(-230.0, 0, 1, 0);        // y축을 기준으로 회전
	glRotatef(sin(time) * 2, 0, 0, 1); //좌우 반동 표현(좌우로 2도만큼 주기적인 움직임 설정)

	// 상하로 움직이는 것을 표현
	float j = 0;
	j = abs(sin(time2)*0.005);
	glPushMatrix();
	glTranslatef(0.0, j, 0.0);
	glTranslatef(0.0, 0.5, 0.0);
	DrawAndroid();
	glutSwapBuffers();
}

// 강물에 발을담궈 인사하는 함수
void Insa() {
	glLoadIdentity(); // CTM초기화
	L_Arm_x = sin(time) * 10;
	R_Arm_x = -L_Arm_x;
	R_Arm_y = -abs(sin(time));
	L_Arm_y = -abs(-sin(time) * 80 + 80);
	R_Leg_y = 15;
	L_Leg_y = 15;
	R_Leg_x = -75;
	L_Leg_x = -75;

	cyl = gluNewQuadric(); // 실린더 객체생성
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 초기화
	glMatrixMode(GL_MODELVIEW); // 모드설정

	DrawGround();     // 지면을 그림
	glLoadIdentity(); // CTM초기화


	glRotatef(-210.0, 0, 1, 0);   // y축 기준으로 회전
	glTranslated(0.0, 0.22, 0.0); // 로봇의 최초위치

	// 로봇이 달리면서 상하로 움직이는 것을 표현
	DrawAndroid();
	glutSwapBuffers();
}

// 키보드 콜백
void MyKeyboard(unsigned char KeyPressed, int x, int y) {
	switch (KeyPressed) {
	case 'w': // Wire 모드
		flag = 1;
		break;
	case 's': // Solid 모드
		flag = 0;
		break;
	}
}

void MyDisplay() {
	if (key == RUN) { //달릴때
		Run();
		glPopMatrix();
	}
	else if (key == JUG) { //저글링 할 때
		Jug();
		glPopMatrix();
	}
	else if (key == INSA) { //강물에 발을 담궈 인사할 때
		Insa();
		glPopMatrix();
	}
	else if (key == 7) { //종료
		exit(0);
	}
}

void MySubMenu(int entryID) {
	if (entryID == 1)
		flag = 1; // 와이어
	else if (entryID == 2)
		flag = 0; // 솔리드
}

// 타이머 함수
void MyTimer(int Value) {
	time = time + 0.1;   // 뛸 때 쓰이는 타이머 
	time2 = time2 + 0.2; // 저글링시 쓰이는 타이머 변수
	time3 = time3 + 0.5; // 인사할때 흐르는 강물에 대한 타이머 변수
	Day = (Day + 10) % 360;
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}

// 마우스 콜백 함수
void MyMainMenu(int entryID) {
	key = entryID;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("움직이는 푸 20171549허진영");
	glInit();
	GLint MySubMenuID = glutCreateMenu(MySubMenu);
	glutAddMenuEntry("Wire모드", 1);  // wire모드 서브메뉴
	glutAddMenuEntry("Solid모드", 2); // solid모드 서브메뉴
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("Change Soid/Wire(s,w)", MySubMenuID); //서브메뉴항목 할당
	glutAddMenuEntry("Run", 1);        //런 메뉴
	glutAddMenuEntry("Juggling", 2);   //저글링 메뉴
	glutAddMenuEntry("Insa", 3);       // 인사 메뉴
	glutAddMenuEntry("종료(Exit)", 7);  //종료 메뉴
	glutAttachMenu(GLUT_RIGHT_BUTTON); //마우스 오른쪽 버튼을 클릭하면 메뉴 팝업
	glutKeyboardFunc(MyKeyboard);
	glutTimerFunc(40, MyTimer, 1);
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}