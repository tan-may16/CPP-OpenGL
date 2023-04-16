
#include <iostream>
#include "fssimplewindow.h"
#include <chrono>
#include <thread>

int main()
{
	FsOpenWindow(0, 0, 800, 600, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int x1, x2, y1, y2;
	x1 = 0;
	x2 = 800;
	y1 = 0;
	y2 = 99;





	for (;;)
	{
		FsPollDevice();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3ub(0, 255, 0);
		glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x1, y2);
		glVertex2i(x2, y2);
		glVertex2i(x2, y1);
		glEnd();
		glFlush();

		std::this_thread::sleep_for(std::chrono::seconds(1));

		y1 = y1 + 100;
		y2 = y2 + 100;
		if (y1 >= 600)
		{
			break;
		}

	}
	return 0;
}