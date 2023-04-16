#include <stdio.h> 
#include "fssimplewindow.h" 
void VisualizeArrayContents(int n, int x[])
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < n; i++)
	{
		glColor3ub(0, 0, 255);
		glBegin(GL_QUADS);
		glVertex2i(0, 20 * i);
		glVertex2i(20 * x[i], 20 * i);
		glVertex2i(20 * x[i], 20 * i + 20);
		glVertex2i(0, 20 * i + 20);
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
void BubbleSort(int n, int x[])
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		while (FSKEY_NULL == FsInkey())
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			VisualizeArrayContents(n, x);
			FsPollDevice();
			FsSwapBuffers();
			FsSleep(10);

		}
		for (j = i + 1; j < n; j++)
		{
			if (x[i] > x[j])
			{
				SwapInt(x[i], x[j]);
			}
		}
		FsPollDevice();


	}
}
int main(void)
{
	int x[20] = { 17,10,13,14,15,6,3,2,9,8,18,7,12,11,16,4,1,20,5,19 };
	FsOpenWindow(16, 16, 400, 400, 1);
	//VisualizeArrayContents(20, x);
	BubbleSort(20, x);
	return 0;
}