
#include <stdio.h>
#include "yspng.h"
#include "yspngenc.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include<math.h>
#include<iostream>
#include <vector>
#include "yssimplesound.h"
const double Pi = 3.1415927;
const double G = 9.8;
const double tol = 1.0e-06;
void DrawCursor(double x, double y)
{
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x, y + 10);
	glVertex2d(x, y);
	glVertex2d(x + 10, y);
	glEnd();
}
void DrawLoading(double x, double y)
{

	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2d(x - 10, y - 10);
	glVertex2d(x - 10, y + 30);
	glVertex2d(x + 160, y + 30);
	glVertex2d(x + 160, y - 10);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2d(x, y);
	glVertex2d(x, y + 20);
	glVertex2d(x + 150, y + 20);
	glVertex2d(x + 150, y);
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2d(x - 10, y - 10);
	glVertex2d(x - 10, y + 30);

	glVertex2d(x - 10, y + 30);
	glVertex2d(x + 160, y + 30);

	glVertex2d(x + 160, y + 30);
	glVertex2d(x + 160, y - 10);

	glVertex2d(x + 160, y - 10);
	glVertex2d(x - 10, y - 10);
	glEnd();
	glRasterPos2d(x, y - 20);
	YsGlDrawFontBitmap20x28("Loading...");
}
void Welcome(int x1, int y1, int x2, int y2)
{
	glColor3ub(255, 0, 0);
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x1, y2);
	glVertex2i(x2, y2);
	glVertex2i(x2, y1);
	glEnd();
}


void VisualizeArrayContents(int n, int x[])
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < n; i++)
	{
		glColor3ub(0, 0, 255);
		glBegin(GL_QUADS);
		glVertex2i(200, 100 + 20 * i);
		glVertex2i(200 + 20 * x[i], 100 + 20 * i);
		glVertex2i(200 + 20 * x[i], 100 + 20 * i + 20);
		glVertex2i(200, 100 + 20 * i + 20);
		glEnd();
		glColor3ub(0, 0, 0);
		glBegin(GL_LINES);
		glVertex2i(200, 100);
		glVertex2i(600, 100);

		glVertex2i(600, 100);
		glVertex2i(600, 500);

		glVertex2i(600, 500);
		glVertex2i(200, 500);

		glVertex2i(200, 500);
		glVertex2i(200, 100);
		glEnd();
		glFlush();
		FsPollDevice();

	}
}
void SwapInt(int& a, int& b)
{
	auto c = a;
	a = b;
	b = c;
}


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
	glColor3f(0, 0, 0);
	glRasterPos2i(100, 50);
	YsGlDrawFontBitmap12x16("Apply some cool Physics concepts");
	glRasterPos2i(150, 100);
	YsGlDrawFontBitmap16x20("Visualize your Mathematical Equations");

	//FsSwapBuffers();
}


void DrawCircle(double x, double y, double r)
{
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 64; ++i)
	{
		glColor3f(0, 0, 0);
		double a = (double)i * Pi / 32.0;
		double c = cos(a);
		double s = sin(a);
		glVertex2d(x + c * r, y + s * r);
	}
	glEnd();
}

void Trans(double& wx, double& wy, double x, double y)
{
	wx = x * 10.0 + 600;
	wy = 600.0 - y * 10.0;
}

int CheckCollision(
	double x1, double y1, double vx1, double vy1,
	double x2, double y2, double vx2, double vy2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dist = sqrt(dx * dx + dy * dy);
	if (dist < 2.0)
	{
		double rvx = vx2 - vx1;
		double rvy = vy2 - vy1;
		if (dx * rvx + dy * rvy < 0.0)
		{
			return 1;
		}
	}
	return 0;
}

void Bounce(
	double x1, double y1, double& vx1, double& vy1,
	double x2, double y2, double& vx2, double& vy2)
{
	double nx, ny;
	nx = x2 - x1;
	ny = y2 - y1;

	double n = sqrt(nx * nx + ny * ny);
	if (n < tol)
	{
		return;
	}
	nx /= n;
	ny /= n;

	double k1 = vx1 * nx + vy1 * ny;
	double k2 = vx2 * nx + vy2 * ny;

	vx1 = vx1 + nx * (k2 - k1);
	vy1 = vy1 + ny * (k2 - k1);

	vx2 = vx2 + nx * (k1 - k2);
	vy2 = vy2 + ny * (k1 - k2);
}






void GetHistogram(int hist[], YsRawPngDecoder& png)
{
	for (int i = 0; i < 256; ++i)
	{
		hist[i] = 0;
	}
	for (int i = 0; i < png.wid * png.hei; ++i)
	{
		++hist[png.rgba[i * 4]];   // Red
		++hist[png.rgba[i * 4 + 1]]; // Green
		++hist[png.rgba[i * 4 + 2]]; // Blue
	}
}

void FindThreshold(int& lowEnd, int& highEnd, int hist[], int total)
{
	auto onePercent = total / 100;
	lowEnd = 0;

	int sum = 0;
	while (lowEnd < 256)
	{
		sum += hist[lowEnd];
		if (onePercent <= sum)
		{
			break;
		}
		++lowEnd;
	}

	highEnd = 255;
	sum = 0;
	while (0 <= highEnd)
	{
		sum += hist[highEnd];
		if (onePercent <= sum)
		{
			break;
		}
		--highEnd;
	}
}

unsigned char Expand(int v, int lowEnd, int highEnd)
{
	if (highEnd != lowEnd)
	{
		v = 255 * (v - lowEnd) / (highEnd - lowEnd);
		if (v < 0)
		{
			v = 0;
		}
		else if (255 < v)
		{
			v = 255;
		}
	}
	return (unsigned char)v;
}

void ExpandImage(YsRawPngDecoder& png, int lowEnd, int highEnd)
{
	for (int i = 0; i < png.wid * png.hei; ++i)
	{
		png.rgba[i * 4] = Expand(png.rgba[i * 4], lowEnd, highEnd);   // Red
		png.rgba[i * 4 + 1] = Expand(png.rgba[i * 4 + 1], lowEnd, highEnd); // Green
		png.rgba[i * 4 + 2] = Expand(png.rgba[i * 4 + 2], lowEnd, highEnd); // Blue
	}
}

void IncreaseContrast(YsRawPngDecoder& png)
{
	int hist[256];
	GetHistogram(hist, png);

	for (int i = 0; i < 256; i += 4)
	{
		int L = hist[i] / 400;
	}
	int lowEnd, highEnd;
	FindThreshold(lowEnd, highEnd, hist, png.wid * png.hei * 3);
	ExpandImage(png, lowEnd, highEnd);
}
class CharBitmap
{
public:
	int wid, hei;
	char* pix;
	int m = 20;
	CharBitmap();
	~CharBitmap();
	void CleanUp();
	void Create(int w, int h);
	void SetPixel(int x, int y, char p);
	char GetPixel(int x, int y) const;
	void Draw() const;

};

void CharBitmap::CleanUp(void)
{
	if (pix != nullptr)
	{
		delete[] pix;
		pix = new char[wid * hei];
		wid = 0;
		hei = 0;
		pix = nullptr;

	}

}
CharBitmap::CharBitmap()
{
	wid = 0;
	hei = 0;
	pix = new char[wid * hei];
	pix = nullptr;
}

CharBitmap::~CharBitmap()
{
	CleanUp();
}

void CharBitmap::Create(int w, int h)
{
	wid = w;
	hei = h;
	if (pix == nullptr)
	{
		pix = new char[w * h];
	}

	for (int i = 0; i < w * h; i++)
	{
		pix[i] = '0';
	}


}
void CharBitmap::SetPixel(int x, int y, char p)
{
	if (x >= 0 && x <= wid - 1 && y >= 0 && y <= hei - 1)
	{
		pix[y * wid + x] = p;
	}
}
char CharBitmap::GetPixel(int x, int y) const
{
	if (x < 0 || x > wid - 1 || y<0 || y > hei - 1)
	{
		return 0;
	}
	else
	{
		return pix[y * wid + x];
	}

}
void CharBitmap::Draw(void) const
{
	int i, j;

	for (i = 0; i < wid; i++)
	{
		for (j = 0; j < hei; j++)
		{
			if (GetPixel(i, j) == '0')
			{
				glColor3ub(0, 0, 0);
			}
			if (GetPixel(i, j) == '1')
			{
				glColor3ub(0, 0, 255);
			}
			if (GetPixel(i, j) == '2')
			{
				glColor3ub(255, 0, 0);
			}
			if (GetPixel(i, j) == '3')
			{
				glColor3ub(255, 0, 255);
			}
			if (GetPixel(i, j) == '4')
			{
				glColor3ub(0, 255, 0);
			}
			if (GetPixel(i, j) == '5')
			{
				glColor3ub(0, 255, 255);
			}
			if (GetPixel(i, j) == '6')
			{
				glColor3ub(255, 255, 0);
			}
			if (GetPixel(i, j) == '7')
			{
				glColor3ub(255, 255, 255);
			}
			glBegin(GL_QUADS);
			glVertex2i(i * m, j * m);
			glVertex2i(i * m, (j + 1) * m);
			glVertex2i((i + 1) * m, (j + 1) * m);
			glVertex2i((i + 1) * m, j * m);
			glEnd();

		}
	}

	for (int i = 0; i < wid; i++)
	{
		for (int j = 0; j < hei; j++)
		{
			glColor3ub(255, 255, 255);
			glBegin(GL_LINES);
			glVertex2i(i * m, j * m);
			glVertex2i(i * m, (j + 1) * m);
			glEnd();

			glColor3ub(255, 255, 255);
			glBegin(GL_LINES);
			glVertex2i(i * m, j * m);
			glVertex2i((i + 1) * m, (j)*m);
			glEnd();
		}

	}

}

std::vector <std::string> Parse(std::string input)
{
	std::vector <std::string> args;

	int state = 0;
	for (auto c : input)
	{
		if (0 == state)
		{
			if (' ' != c && '\t' != c && '\n' != c)
			{
				args.push_back("");
				(args.back()).push_back(c);
				state = 1;
			}
		}
		else
		{
			if (' ' != c && '\t' != c && '\n' != c)
				args.back().push_back(c);
			else
				state = 0;
		}
	}

	return args;
}
int main(void)
{
	double cursorX, cursorY;
	cursorX = 0;
	cursorY = 0;
	int time_t = 0;
	int x1, y1, x2, y2;
	x1 = 0; y1 = 0; x2 = 1000; y2 = 150;
	int x[20] = { 17,10,13,14,15,6,3,2,9,8,18,7,12,11,16,4,1,20,5,19 };
	int count1 = 10;

	YsRawPngDecoder png;
	YsRawPngDecoder png1;
	YsRawPngDecoder png2;
	YsRawPngDecoder png3;
	YsRawPngDecoder png4;
	YsRawPngDecoder png5;
	YsRawPngDecoder png6;
	YsRawPngDecoder png7;
	YsRawPngDecoder png8;
	YsRawPngDecoder p1;
	YsRawPngDecoder p2;
	YsRawPngDecoder p3;
	YsRawPngDecoder p4;
	YsRawPngDecoder p5;

	L = 4;
	h = 1;
	double a_n, v_n, x_n, a_n_update, v_n_update, x_n_update, L_natural, k, c, m;
	int i_bit = 0;
	//double dt;
	m = 1;
	k = 1;
	c = 0.5;
	L_natural = 2;
	//dt = 0.010;
	v_n = 0;
	x_n = L;
	a_n = (k * (L_natural - x_n) - c * v_n) / m;
	double dt;

	YsSoundPlayer player;
	YsSoundPlayer::SoundData sound1;
	YsSoundPlayer::SoundData sound2;
	char mouse[256] = "pcmouseclick4 (1).wav";
	char background[256] = "399085__newagesoup__queens-loop-120bpm (1) (1).wav";
	if (YSOK != sound1.LoadWav(mouse))
	{
		printf("Error!  Cannot load music.wav!\n");
	}
	if (YSOK != sound2.LoadWav(background))
	{
		printf("Error!  Cannot load background.wav!\n");
	}

	srand(time(nullptr));

	const int n = 100;
	double x_ball[n], y_ball[n], vx_ball[n], vy_ball[n];

	for (int i = 0; i < n; ++i)
	{
		x_ball[i] = (double)(rand() % 78 - 39);
		y_ball[i] = 30.0 + (double)(rand() % 20);
		vx_ball[i] = (double)(rand() % 20 - 10);
		vy_ball[i] = 0.0;
	}

	CharBitmap bitmap;
	bitmap.Create(50, 30);


	if (YSOK == png.Decode("ss.png"))
	{
		png.Flip();


	}
	if (YSOK == png1.Decode("shape1.png"))
	{
		png1.Flip();


	}
	if (YSOK == png2.Decode("shape3.png"))
	{
		png2.Flip();


	}
	if (YSOK == png3.Decode("shape4.png"))
	{
		png3.Flip();


	}
	if (YSOK == png4.Decode("photo1.png"))
	{
		png4.Flip();


	}
	if (YSOK == png5.Decode("photo2.png"))
	{
		png5.Flip();


	}
	if (YSOK == png6.Decode("photo3.png"))
	{
		png6.Flip();


	}
	if (YSOK == p1.Decode("program1.png"))
	{
		p1.Flip();


	}
	if (YSOK == p2.Decode("program2.png"))
	{
		p2.Flip();


	}
	if (YSOK == p3.Decode("program3.png"))
	{
		p3.Flip();


	}
	if (YSOK == p4.Decode("program4.png"))
	{
		p4.Flip();


	}
	if (YSOK == p5.Decode("program5.png"))
	{
		p5.Flip();
	}
	FsOpenWindow(250, 75, 1000, 600, 1);
	player.Start();
	//player.PlayBackground(sound2);
	for (;;)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//IncreaseContrast(png);
		FsPollDevice();

		if (time_t > 12000)
		{
			player.PlayBackground(sound2);
		}

		if (time_t > 49000)
		{
			player.Stop(sound2);
		}

		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}
		if (time_t <= 2000) //Fall 21 declaration
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(220, 500);
			YsGlDrawFontBitmap28x44("Fall 21 Registration..");
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);

		}
		if (time_t > 2000 && time_t <= 2500) //Wait time
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);
		}

		if (time_t > 2500 && time_t <= 5000) //Cursor to Register
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);

			if (cursorX <= 285)
			{
				cursorX += 4;

			}
			if (cursorY <= 223)
			{
				cursorY += 4;
			}
		}
		if (time_t > 5000 && time_t <= 8000) //Cursor to professor's name
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);
			if (cursorX <= 680)
			{
				cursorX += 4;

			}
			if (cursorY >= 140)
			{
				cursorY -= 1;
			}
		}
		if (time_t > 8000 && time_t <= 11000) //cursor to view demo
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);
			if (cursorX >= 285)
			{
				cursorX -= 2.5;

			}
			if (cursorY <= 237)
			{
				cursorY += 0.7;
			}
		}
		if (time_t > 11000 && time_t <= 11500) //cursor to view demo
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);


		}



		if (time_t > 11500 && time_t <= 12000)
		{
			DrawLoading(450, 300);
		}
		if (time_t > 12000 && time_t <= 15000)
		{
			Welcome(x1, y1, x2, y2);
			if (time_t > 12000)
			{

				glColor3f(0, 0, 0);
				glRasterPos2i(400, 75);
				YsGlDrawFontBitmap24x40("WELCOME");
			}
			if (time_t > 12500)
			{
				y2 = 300;
				glColor3f(0, 0, 0);
				glRasterPos2i(450, 225);
				YsGlDrawFontBitmap24x40("TO");
			}
			if (time_t > 13000)
			{
				y2 = 450;
				glColor3f(0, 0, 0);
				glRasterPos2i(200, 375);
				YsGlDrawFontBitmap24x40("ENGINEERING COMPUTATION DEMO");
			}
			if (time_t > 13500)
			{
				y2 = 600;
				glColor3f(0, 0, 0);
				glRasterPos2i(450, 525);
				YsGlDrawFontBitmap24x40("PART I");
			}
		}
		if (time_t > 15000 && time_t <= 20500)
		{
			glColor3f(0, 0, 0);
			glRasterPos2i(100, 50);
			YsGlDrawFontBitmap8x12("Interested in Learning programming but afraid of cpp syntax?");
			if (time_t > 16000)
			{
				glRasterPos2i(100, 75);
				YsGlDrawFontBitmap16x20("No Worries, We will sort out everything");
			}
			if (time_t > 19000)
			{
				glRasterPos2i(100, 400);
				YsGlDrawFontBitmap28x44("Quite literally");
			}
			if (time_t > 17000)
			{
				VisualizeArrayContents(20, x);
				glColor3f(0, 0, 0);
				if (time_t > 18000)
				{
					glRasterPos2i(200, 550);
					YsGlDrawFontBitmap28x44("Quite Literally!");
				}
				glRasterPos2i(100, 50);
				YsGlDrawFontBitmap8x12("Interested in Learning programming but afraid of cpp syntax?");
				glRasterPos2i(100, 75);
				YsGlDrawFontBitmap16x20("No Worries, We will sort out everything");
				for (int i = 0; i < 20; i++)
				{
					if (time_t % 300 == 0)
					{
						for (int j = i + 1; j < 20; j++)
						{
							if (x[i] > x[j])
							{
								SwapInt(x[i], x[j]);
								break;
							}
						}
					}
				}
			}

		}

		if (time_t > 20500 && time_t <= 24000)
		{

			glRasterPos2i(0, 599);
			glDrawPixels(png1.wid, png1.hei, GL_RGBA, GL_UNSIGNED_BYTE, png1.rgba);
			if (time_t > 21500)
			{
				glRasterPos2i(400, 599);
				glDrawPixels(png3.wid, png3.hei, GL_RGBA, GL_UNSIGNED_BYTE, png3.rgba);
			}
			if (time_t > 22500)
			{
				glRasterPos2i(300, 250);
				glDrawPixels(png2.wid, png2.hei, GL_RGBA, GL_UNSIGNED_BYTE, png2.rgba);
				glRasterPos2i(100, 80);
				YsGlDrawFontBitmap8x12("And learn to use large datasets");
			}

			glColor3f(0, 0, 0);
			glRasterPos2i(100, 50);
			YsGlDrawFontBitmap8x12("Learn to Draw Complicated shapes");

		}
		if (time_t > 24000 && time_t <= 30000)
		{

			double dt_ball = 0.0125;

			for (int i = 0; i < n; ++i)
			{
				x_ball[i] = x_ball[i] + vx_ball[i] * dt_ball;
				y_ball[i] = y_ball[i] + vy_ball[i] * dt_ball;
				vy_ball[i] -= G * dt_ball;
				if (y_ball[i] < 1.0 && vy_ball[i] < 0.0)
				{
					vy_ball[i] = -1 * vy_ball[i];
				}
				if ((x_ball[i] < -59.0 && vx_ball[i] < 0.0) || (39.0 < x_ball[i] && 0.0 < vx_ball[i]))
				{
					vx_ball[i] = -1 * vx_ball[i];
				}
			}
			for (int i = 0; i < n; ++i)
			{
				for (int j = i + 1; j < n; ++j)
				{
					if (CheckCollision(x_ball[i], y_ball[i], vx_ball[i], vy_ball[i], x_ball[j], y_ball[j], vx_ball[j], vy_ball[j]))
					{
						Bounce(x_ball[i], y_ball[i], vx_ball[i], vy_ball[i], x_ball[j], y_ball[j], vx_ball[j], vy_ball[j]);
					}
				}
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < n; ++i)
			{
				double wx, wy;
				Trans(wx, wy, x_ball[i], y_ball[i]);
				DrawCircle(wx, wy, 10.0);
			}
			glColor3f(0, 0, 0);
			glRasterPos2i(100, 50);
			YsGlDrawFontBitmap16x20("Apply some cool Physics concepts");
		}
		if (time_t > 30000 && time_t <= 35000)
		{
			if (L == L_natural)
			{
				r = 0;
			}
			else if (L < L_natural && L >= L_natural - 1)
			{
				r = 2 - L;
			}
			else if (L >= L_natural && L <= L_natural + 1)
			{
				r = L - 2;
			}
			else
			{
				r = 1;
			}
			FsRegisterOnPaintCallBack(Render, nullptr);
			dt = 0.05;
			//a_n_update = 3 - x_n - v_n * dt - 0.5 * v_n - 0.5 * a_n * dt;
			a_n_update = (k * (L_natural - (x_n + v_n * dt)) - c * (v_n + a_n * dt)) / m;
			a_n = a_n_update;
			v_n_update = v_n + a_n * dt;
			v_n = v_n_update;
			x_n_update = x_n + v_n * dt;
			x_n = x_n_update;
			L = x_n;
			FsPushOnPaintEvent();
		}
		if (time_t > 35000 && time_t <= 38000)
		{
			glRasterPos2i(500, 599);
			glDrawPixels(png4.wid, png4.hei, GL_RGBA, GL_UNSIGNED_BYTE, png4.rgba);

			glRasterPos2i(600, 300);
			glDrawPixels(png5.wid, png5.hei, GL_RGBA, GL_UNSIGNED_BYTE, png5.rgba);

			glRasterPos2i(0, 500);
			glDrawPixels(png6.wid, png6.hei, GL_RGBA, GL_UNSIGNED_BYTE, png6.rgba);

			glColor3f(0, 0, 0);
			glRasterPos2i(25, 50);
			YsGlDrawFontBitmap12x16("Apply some cool Physics concepts");
			glRasterPos2i(25, 100);
			YsGlDrawFontBitmap12x16("Visualize your Mathematical Equations");
			glRasterPos2i(25, 150);
			YsGlDrawFontBitmap16x20("Showoff image Processing skills");
			if (time_t > 37000)
			{
				glRasterPos2i(25, 550);
				YsGlDrawFontBitmap16x20("And do much more!");
			}
			if (time_t > 36000 && time_t <= 36500)
			{
				IncreaseContrast(png4);
			}
			if (time_t > 36500 && time_t <= 37000)
			{
				IncreaseContrast(png5);

			}
			if (time_t > 37000 && time_t <= 37500)
			{
				IncreaseContrast(png6);

			}

		}
		if (time_t > 38000 && time_t <= 40000)
		{
			glColor3f(0, 0, 0);
			glRasterPos2i(200, 100);
			YsGlDrawFontBitmap16x20("Looks Pretty Simple, Right?!");

			if (time_t > 39000)
			{
				glColor3f(0, 0, 0);
				glRasterPos2i(300, 150);
				YsGlDrawFontBitmap16x20("Well, Um...?!");
			}
			if (time_t > 39000)
			{
				glColor3f(0, 0, 0);
				glRasterPos2i(400, 250);
				YsGlDrawFontBitmap16x20("Not Exactly!");
			}

		}
		if (time_t > 40000 && time_t <= 50000)
		{
			glRasterPos2i(0, 450);
			glDrawPixels(p1.wid, p1.hei, GL_RGBA, GL_UNSIGNED_BYTE, p1.rgba);
			if (time_t > 40500)
			{
				glRasterPos2i(200, 400);
				glDrawPixels(p2.wid, p2.hei, GL_RGBA, GL_UNSIGNED_BYTE, p2.rgba);
			}
			if (time_t > 41000)
			{
				glRasterPos2i(400, 450);
				glDrawPixels(p3.wid, p3.hei, GL_RGBA, GL_UNSIGNED_BYTE, p3.rgba);
			}
			if (time_t > 41500)
			{
				glRasterPos2i(600, 350);
				glDrawPixels(p4.wid, p4.hei, GL_RGBA, GL_UNSIGNED_BYTE, p4.rgba);
			}
			if (time_t > 42000)
			{
				glRasterPos2i(300, 500);
				glDrawPixels(p5.wid, p5.hei, GL_RGBA, GL_UNSIGNED_BYTE, p5.rgba);
			}

			if (time_t > 42500)
			{
				glRasterPos2i(50, 150);
				YsGlDrawFontBitmap16x24("You have to either write a big code");
				glRasterPos2i(50, 200);
				YsGlDrawFontBitmap16x20("requiring lot of patience and hundreds of debugging steps");
			}
			if (time_t > 45500)
			{
				glColor3f(1, 0, 0);
				glRasterPos2i(50, 300);
				YsGlDrawFontBitmap16x20("Or");
			}
			if (time_t > 46000)
			{
				glColor3f(0, 0, 0);
				glRasterPos2i(50, 400);
				YsGlDrawFontBitmap16x20("You can learn object oriented Programming");
				glRasterPos2i(50, 450);
				YsGlDrawFontBitmap16x20("By Registering for Engineering Computation-24780");
			}
			if (time_t > 46000)
			{
				glColor3f(1, 0, 0);
				glRasterPos2i(50, 500);
				YsGlDrawFontBitmap16x20("Who Knows!");
				glRasterPos2i(50, 550);
				YsGlDrawFontBitmap16x20("You might be the one making PART 2");
				glColor3f(0, 0, 0);
			}
		}
		if (time_t > 50000 && time_t <= 60000)
		{
			glRasterPos2i(0, 599);
			glDrawPixels(png.wid, png.hei, GL_RGBA, GL_UNSIGNED_BYTE, png.rgba);
			glColor3f(0, 0, 0);
			glRasterPos2i(255, 223);
			YsGlDrawFontBitmap8x8("REGISTER");
			glRasterPos2i(255, 237);
			YsGlDrawFontBitmap8x8("VIEW DEMO");
			DrawCursor(cursorX, cursorY);
			if (time_t > 51000 && time_t <= 52000)
			{
				if (cursorX <= 800)
				{
					cursorX += 6;
				}
				if (cursorY >= 50)
				{
					cursorY -= 2.5;
				}
			}
			if (time_t > 52000 && time_t <= 53000)
			{
				if (cursorX >= 600)
				{
					cursorX -= 3;
				}
				if (cursorY <= 500)
				{
					cursorY += 6;
				}
			}
			if (time_t > 53000 && time_t <= 57000)
			{
				if (cursorX >= 285)
				{
					cursorX -= 6;
				}
				if (cursorY >= 223)
				{
					cursorY -= 6;
				}
			}
			if (time_t > 55000)
			{
				DrawLoading(450, 300);
			}
		}
		if (time_t > 57000 && time_t <= 60000)
		{
			bitmap.Draw();
			if (time_t % 200 == 0 && i_bit < 7)
			{
				bitmap.pix[50 * (i_bit + 6) + 3] = '7'; //T
				bitmap.pix[50 * (6) + i_bit] = '7';

				bitmap.pix[50 * (i_bit + 6) + 8] = '7';//H
				if (i_bit != 6)
				{
					bitmap.pix[50 * (9) + i_bit + 8] = '7';
				}

				bitmap.pix[50 * (i_bit + 6) + 13] = '7';

				bitmap.pix[50 * (12 - i_bit) + 15 + i_bit] = '7'; //A
				bitmap.pix[50 * (6 + i_bit) + 21 + i_bit] = '7';
				bitmap.pix[50 * (9) + 18 + i_bit] = '7';

				bitmap.pix[50 * (i_bit + 6) + 29] = '7';//N
				bitmap.pix[50 * (6 + i_bit) + 29 + i_bit] = '7';
				bitmap.pix[50 * (i_bit + 6) + 36] = '7';

				bitmap.pix[50 * (i_bit + 6) + 38] = '7'; //K
				if (i_bit < 4)
				{
					bitmap.pix[50 * (9 - i_bit) + 39 + i_bit] = '7';
					bitmap.pix[50 * (9 + i_bit) + 39 + i_bit] = '7';
				}

				if (i_bit < 4)//Y
				{
					bitmap.pix[50 * (17 + i_bit) + i_bit] = '7';
					bitmap.pix[50 * (20 - i_bit) + 3 + i_bit] = '7';
				}
				if (i_bit < 4)
				{
					bitmap.pix[50 * (i_bit + 20) + 3] = '7';

				}

				bitmap.pix[50 * (i_bit + 17) + 8] = '7'; //O
				bitmap.pix[50 * (i_bit + 17) + 12] = '7';
				if (i_bit < 5)
				{
					bitmap.pix[50 * (23) + i_bit + 8] = '7';
					bitmap.pix[50 * (17) + i_bit + 8] = '7';
				}
				bitmap.pix[50 * (i_bit + 17) + 14] = '7'; //U
				bitmap.pix[50 * (23) + i_bit + 14] = '7';
				bitmap.pix[50 * (i_bit + 17) + 20] = '7';

				if (i_bit != 5)
					bitmap.pix[50 * (i_bit + 17) + 22] = '7';
				i_bit++;
			}

		}
		if (time_t > 7600 && time_t < 7630)
		{
			player.PlayOneShot(sound1);
		}

		if (time_t > 7630 && time_t < 11230)
		{
			player.Stop(sound1);
		}
		if (time_t > 11200 && time_t < 11230)
		{
			player.PlayOneShot(sound1);
		}

		if (time_t > 11230)
		{
			player.Stop(sound1);
		}
		if (time_t > 55000 && time_t < 55020)
		{
			player.PlayOneShot(sound1);
		}

		if (time_t > 55020)
		{
			player.Stop(sound1);
		}

		if (time_t >= 60000)
		{
			break;
		}
		FsSleep(10);
		time_t += 10;
		count1 += 10;
		FsSwapBuffers();
	}
	player.End();
	return 0;
}
