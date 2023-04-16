#include <stdio.h>
#include <stdlib.h>
#include "fssimplewindow.h"

template <class T>
class BitmapTemplate
{
private:
	T* pix;
	int wid, hei;

public:

	BitmapTemplate();
	~BitmapTemplate();
	void CleanUp(void);

	BitmapTemplate<T>(const BitmapTemplate<T>& incoming);
	BitmapTemplate<T>& operator=(const BitmapTemplate<T>& incoming);
	void copyFrom(const BitmapTemplate<T>& incoming);
	int GetWidth() const;
	int GetHeight() const;
	void SetPixel(int x, int y, T p);
	T GetPixel(int x, int y);
	void Create(int w, int h);
};
template <class T>
BitmapTemplate <T>::BitmapTemplate()
{
	wid = 0;
	hei = 0;
	pix = nullptr;
}
template <class T>
BitmapTemplate <T>::~BitmapTemplate()
{
	CleanUp();
}

template <class T>
void BitmapTemplate <T>::CleanUp(void)
{
	wid = 0;
	hei = 0;
	if (nullptr != pix)
	{
		delete[] pix;
	}
	pix = nullptr;
}

template <class T>
void BitmapTemplate<T>::copyFrom(const BitmapTemplate<T>& incoming)
{

	CleanUp();
	wid = incoming.wid;
	hei = incoming.hei;

	pix = new T[wid * hei];

	for (int i = 0; i < wid; i++)
	{
		for (int j = 0; j < hei; j++)
		{
			pix[j * wid + i] = incoming.pix[j * wid + i];
		}
	}

}
template <class T>
BitmapTemplate<T>::BitmapTemplate(const BitmapTemplate<T>& incoming)
{

	wid = 0;
	hei = 0;

	pix = NULL;

	copyFrom(incoming);

}
template <class T>
BitmapTemplate<T>& BitmapTemplate<T>::operator=(const BitmapTemplate<T>& incoming) {
	copyFrom(incoming);
	return *this;
}

template <class T>
int BitmapTemplate<T>::GetWidth() const
{
	return wid;
}
template <class T>
int BitmapTemplate<T>::GetHeight() const
{
	return hei;
}
template <class T>
void BitmapTemplate<T>::SetPixel(int x, int y, T p)
{
	if (0 <= x && x < wid && 0 <= y && y < hei)
	{
		pix[y * wid + x] = p;
	}
}
template <class T>
T BitmapTemplate<T>::GetPixel(int x, int y)
{
	if (0 <= x && x < wid && 0 <= y && y < hei)
	{
		return pix[y * wid + x];
	}
	return pix[0];
}
template <class T>
void BitmapTemplate<T>::Create(int w, int h)
{
	CleanUp();
	pix = new T[w * h];
	wid = w;
	hei = h;

	for (int i = 0; i < wid * hei; ++i)
	{
		pix[i] = 0;
	}
}


class CharBitmap : public BitmapTemplate <char>
{
public:
	void Draw(void);
};
void CharBitmap::Draw(void)
{
	glBegin(GL_QUADS);

	for (int y = 0; y < GetHeight(); ++y)
	{
		for (int x = 0; x < GetWidth(); ++x)
		{
			auto pix = GetPixel(x, y);
			// If you use switch & case, that's ok, too.
			int b = 255 * (pix % 2);
			int r = 255 * ((pix / 2) % 2);
			int g = 255 * ((pix / 4) % 2);

			glColor3ub(r, g, b);
			glVertex2i(x * 20, y * 20);
			glVertex2i(x * 20 + 20, y * 20);
			glVertex2i(x * 20 + 20, y * 20 + 20);
			glVertex2i(x * 20, y * 20 + 20);
		}
	}

	glEnd();
}


int main(void)
{
	int wid = 0, hei = 0;
	while (0 == wid || 64 < wid || 0 == hei || 64 < hei)
	{
		// Either cut & paste Parse function, or write parsing algorithm here.
		printf("Enter Dimension>");

		char str[256];
		fgets(str, 255, stdin);

		int state = 0, n = 0;
		for (int i = 0; 0 != str[i]; ++i)
		{
			if (0 == state)
			{
				if (' ' != str[i])
				{
					if (0 == n)
					{
						wid = atoi(str + i);
					}
					else if (1 == n)
					{
						hei = atoi(str + i);
					}
					++n;
					state = 1;
				}
			}
			else
			{
				if (' ' == str[i])
				{
					state = 0;
				}
			}
		}
		if (n < 2)
		{
			wid = 0;
			hei = 0;
		}
	}


	CharBitmap bmp;
	bmp.Create(wid, hei);
	CharBitmap undo(bmp);
	FsOpenWindow(0, 0, wid * 20, hei * 20, 1);

	for (;;)
	{
		FsPollDevice();
		auto key = FsInkey();

		int lb, mb, rb, mx, my;
		auto evt = FsGetMouseEvent(lb, mb, rb, mx, my);

		if (FSKEY_ESC == key)
		{
			break;
		}
		if (FSKEY_0 <= key && key <= FSKEY_7)
		{
			undo = bmp;
			int px = mx / 20;
			int py = my / 20;
			int colorCode = key - FSKEY_0;
			bmp.SetPixel(px, py, colorCode);
		}

		if (key == FSKEY_U)
		{
			bmp = undo;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bmp.Draw();

		glColor3ub(255, 255, 255); // Make sure the lattice is white.
		glBegin(GL_LINES);
		// Since the window size must match the bitmap size, the loop condition can be <= or <.
		// The last line won't be visible anyway.
		for (int x = 0; x <= wid; ++x)
		{
			glVertex2i(x * 20, 0);
			glVertex2i(x * 20, hei * 20);
		}
		for (int y = 0; y <= hei; ++y)
		{
			glVertex2i(0, y * 20);
			glVertex2i(wid * 20, y * 20);
		}
		glEnd();

		FsSwapBuffers();

		FsSleep(25);
	}
}
