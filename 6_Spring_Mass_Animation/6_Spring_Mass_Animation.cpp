#include <stdio.h>
#include "fssimplewindow.h"

void TransformCoordinate(int& sx, int& sy, double px, double py)
{
	sx = 100 + (int)(px * 100.0);
	sy = 300 - (int)(py * 100.0);
}

void DrawWall(void)
{
	int sx, sy;

	glBegin(GL_LINES);

	glColor3ub(255, 0, 0);

	TransformCoordinate(sx, sy, 0.0, -3.0);
	glVertex2i(sx, sy);
	TransformCoordinate(sx, sy, 0.0, 3.0);
	glVertex2i(sx, sy);

	for (double y = -3.0; y < 3.0; y += 0.3)
	{
		TransformCoordinate(sx, sy, 0.0, y);
		glVertex2i(sx, sy);
		TransformCoordinate(sx, sy, -0.5, y - 0.5);
		glVertex2i(sx, sy);
	}

	glEnd();
}

void DrawMass(double L, double h, double r)
{
	int sx, sy;

	//glColor3ub(0, 0, 0);
	glColor3f(r, 0, 0);
	glBegin(GL_QUADS);

	TransformCoordinate(sx, sy, L, h / 2.0);
	glVertex2i(sx, sy);
	TransformCoordinate(sx, sy, L, -h / 2.0);
	glVertex2i(sx, sy);
	TransformCoordinate(sx, sy, L + h, -h / 2.0);
	glVertex2i(sx, sy);
	TransformCoordinate(sx, sy, L + h, h / 2.0);
	glVertex2i(sx, sy);

	glEnd();
}

void DrawSpring(double L, double h)
{
	int sx, sy;

	glColor3ub(0, 0, 255);

	glBegin(GL_LINES);

	TransformCoordinate(sx, sy, 0.0, 0.0);
	glVertex2i(sx, sy);
	TransformCoordinate(sx, sy, L / 3.0, 0.0);
	glVertex2i(sx, sy);

	TransformCoordinate(sx, sy, L * 2.0 / 3.0, 0.0);
	glVertex2i(sx, sy);
	TransformCoordinate(sx, sy, L, 0.0);
	glVertex2i(sx, sy);

	glEnd();

	glBegin(GL_LINE_STRIP);

	TransformCoordinate(sx, sy, L / 3.0, 0.0);
	glVertex2i(sx, sy);

	double y = -h / 2.0;
	for (double x = L / 3.0 + L / 36.0; x < L * 2.0 / 3.0; x += L / 18.0)
	{
		TransformCoordinate(sx, sy, x, y);
		glVertex2i(sx, sy);
		y = -y;
	}

	TransformCoordinate(sx, sy, L * 2.0 / 3.0, 0.0);
	glVertex2i(sx, sy);

	glEnd();
}

double L, h;
double r;

void Render(void*)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawWall();

	//glColor3f(r, 0, 0);
	DrawMass(L, h, r);

	DrawSpring(L, h);

	FsSwapBuffers();
}

int main(void)
{


	printf("24-780 Engineering Computation Problem Set 2-2 - Mass-Spring System\n");
	printf("Enter L and h:");
	scanf("%lf%lf", &L, &h);

	FsOpenWindow(32, 32, 800, 600, 1);
	double a_n, v_n, x_n, a_n_update, v_n_update, x_n_update, L_natural, k, c, m;
	//double dt;
	m = 1;
	k = 1;
	c = 0.5;
	L_natural = 3;
	//dt = 0.010;
	v_n = 0;
	x_n = L;
	a_n = (k * (L_natural - x_n) - c * v_n) / m;
	FsPassedTime();
	unsigned int dt1;
	double dt;
	while (FSKEY_ESC != FsInkey())
	{
		if (L == L_natural)
		{
			r = 0;
		}
		else if (L < L_natural && L >= L_natural - 1)
		{
			r = 3 - L;
		}
		else if (L >= L_natural && L <= L_natural + 1)
		{
			r = L - 3;
		}
		else
		{
			r = 1;
		}

		FsRegisterOnPaintCallBack(Render, nullptr);
		dt1 = FsPassedTime();

		dt = double(dt1);
		//double dt = FsPassedTime();
		dt = (dt / 1000.0);
		//a_n_update = 3 - x_n - v_n * dt - 0.5 * v_n - 0.5 * a_n * dt;
		a_n_update = (k * (L_natural - (x_n + v_n * dt)) - c * (v_n + a_n * dt)) / m;
		a_n = a_n_update;
		v_n_update = v_n + a_n * dt;
		v_n = v_n_update;
		x_n_update = x_n + v_n * dt;
		x_n = x_n_update;
		L = x_n;



		FsPollDevice();
		FsPushOnPaintEvent();
		FsSleep(10);

	}

	return 0;
}
