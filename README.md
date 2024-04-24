# PooMotion

## 움직이는 푸

open GL 교제에 나오는 안드로이드 그래픽 모션을 활용하여 곰돌이 푸를 만들고 동작을 추가한 프로그램이다.

소스를 활용해 glut환경에서 그래픽 모델링을 하였고 이것을 응용하여 렌더링 하는 것에 초점을 두었다.

### Motion

1. 달리는 푸
2. 저글링 하는 푸
3. 강물에 발을 담궈 앉아 인사하는 푸

## 실행화면
<img src="/resource/poo_motion_실행.gif">


## 응용내용
```
// 몸체를 그리는 함수
void DrawBody(int x, int a, int b, int c) {
 glPushMatrix();
 glColor3f(1.0, 0.2, 0.0);// 빨간색, 옷 입음(반팔)
 cyl = gluNewQuadric();// 실린더 객체 생성
 glRotatef(90.0, 1.0, 0.0, 0.0);// 실린더를 x축 기준으로 90도 회전(실린더 생성 시 각도 변경을 위해)
 glRotatef(x, a, b, c);// 파라미터 값만큼 회전
 Change_Wire_Or_Solid(flag);
 gluCylinder(cyl, 0.2, 0.2, 0.15, 50, 1);// 실린더를 그림
 glPopMatrix();
 glPushMatrix();
 glColor3f(1.0, 0.7, 0.1);// 노란색, 옷 안입음(뱃살)
 cyl = gluNewQuadric();// 실린더 객체 생성
 glRotatef(90.0, 1.0, 0.0, 0.0);// 실린더를 x축 기준으로 90도 회전(실린더 생성 시 각도 변경을 위해)
 glRotatef(x, a, b, c);//파라미터 값만큼 회전
 glTranslatef(0.0, 0.0, 0.15);
 Change_Wire_Or_Solid(flag);
 gluCylinder(cyl, 0.2, 0.2, 0.25, 50, 1);//실린더를 그림
 glPopMatrix();
}
```
몸체의 기본적인 색상을 glColer3f() 함수로 진한 노란색(1.0, 0.7, 0.1)을 표현하였고, 빨간색 옷을 입히기 위해 몸통을 두 실린더 객체로 나누고 팔과 어깨에 해당하는 색상을 빨간색(1.0, 0.2, 0.0)으로 설정했다.

<br/>

```
// 저글링할 때 돌아가는 공 함수
void Ball() {
 glClear(GL_COLOR_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 cyl = gluNewQuadric();
 Change_Wire_Or_Solid(flag);
 glTranslatef(0.3, 0.5, 0.0);// 중점의 x,y좌표 변경
 
 glPushMatrix();// 공1 : 빨간색 공
 glRotatef(Day, 0.0, 0.3, 1.0);
 glTranslatef(-0.3, 0.0, 0.0);
 glColor3f(1.0, 0.0, 0.0);
 glRotatef((GLfloat)Time, 0.0, 0.0, 1.0);
 gluSphere(cyl, 0.05, 10, 10);
 glPopMatrix();
 glPushMatrix();// 공2 : 파란색 공
 glRotatef(Day, 0.0, 0.3, 1.0);
 glTranslatef(0.3, 0.0, 0.0);
 glColor3f(0.0, 0.0, 1.0);
 glRotatef((GLfloat)Time, 0.0, 0.0, 1.0);
 gluSphere(cyl, 0.05, 10, 10);
 glPopMatrix();
}
```
Ball() 함수는 세로로 회전하도록하며 반지름이 0.05인 구를 두개 생성했다. glutTimerFunc() 함수를 활용해 두 개의 구가 자동으로 회전하도록 만들었다.

<br/>

```
// 지면을 그리는 함수
void DrawGround() {
 glColor3f(0.0, 0.8, 0.0);
 glTranslatef(0.0, -2.73, 0.0);// 지면 시작점
 glRotatef(45.0, 0, 1, 0);//45도만큼 y축을 기준으로 회전
 if (key == RUN) glRotatef(0.0 + time3 - 15, 0.0, 0.0, 1);
 else if (key == JUG) glRotatef(0.0 + time2 - 15, 0.0, 0.0, 1);// z축을 기준으로 time2만큼 지면이 회전
 else if (key == INSA) {
 glColor3f(0.529412, 0.807843, 0.980392);// 인터넷 RGB색상에서 얻어옴
 glRotatef(0.0 + time2 - 15, 0.0, 0.0, 1);// 강물이 흐르는 속도, time2만큼 강물이 흐름
 }
 Change_Wire_Or_Solid(flag);
 gluSphere(cyl, 2.5, 30, 90);
}
```
푸가 달릴 때는 지면이 빠르게, 저글링 할 때는 느리게 움직이도록 수정 했다. 
저글링 하는 함수인 Jug()는 팔의 각도를 변경하여 어깨는 고정, 손은 아래, 위로 움직이되, 반대방향으로 움직이도록 설정했다. 

<br/>

```
// 키보드 콜백
void MyKeyboard(unsigned char KeyPressed, int x, int y) {
 switch (KeyPressed) {
 case 'w':// Wire 모드
 flag = 1;
 break;
 case 's':// Solid 모드
 flag = 0;
 break;
 }
}

```

키보드 콜백 : Wire/Solid 모드를 ‘w’, ‘s’를 사용하여 조정 가능하도록 만들었고 메인함수에서 서브메뉴를 추가하여서 설정할 수 있게 했다.
마우스 콜백 : 오른쪽 마우스를 누르면 메뉴를 띄우고 wire/solid설정 메뉴, run, juggling, insa, exit메뉴로 구성했다.
