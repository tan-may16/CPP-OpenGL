# include<stdio.h>
# include<iostream>
#include <fstream>
#include <string>
#include <vector>
# include "fssimplewindow.h"
#include <stdlib.h>
#include <ctype.h>

char* YsFgets(char buf[], unsigned long long int maxSize, FILE* fp)
{
	buf[0] = 0;
	if (nullptr != fgets(buf, maxSize, fp))
	{
		int ptr = 0;
		for (ptr = 0; 0 != buf[ptr]; ++ptr)
		{
		}
		for (ptr = ptr - 1; 0 <= ptr && 0 == isprint(buf[ptr]); --ptr)
		{
			buf[ptr] = 0;
		}
		return buf;
	}
	return nullptr;
}

class FilePointer
{
public:
	FILE* fp = nullptr;

	FilePointer();
	~FilePointer();
	void Open(const char fName[], const char mode[]);
	void Close(void);
};

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
				args.back().push_back(c);
				state = 1;
			}
		}
		else
		{
			if (' ' != c && '\t' != c && '\n' != c)
			{
				args.back().push_back(c);
			}
			else
			{
				state = 0;
			}
		}
	}

	return args;
}

class Line2D
{
protected:
	int x1, y1, x2, y2, r, g, b;

public:
	Line2D();
	~Line2D();
	void Draw() const;
	void MakeFromString(char[]);
	void Cleanup();
};


Line2D::Line2D()
{
	x1 = y1 = x2 = y2 = r = g = b = 0;

}
void Line2D::Cleanup()
{
	x1 = y1 = x2 = y2 = r = g = b = 0;

}
Line2D::~Line2D()
{
	Cleanup();
}

void Line2D::Draw() const
{
	glColor3ub(r, g, b);
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

void Line2D::MakeFromString(char data[])
{

	char s[2] = " ";
	char* token;

	/*token = strtok(data, s);
	x1 = atoi(token);
	token = strtok(data, s);
	y1 = 600 - atoi(token);
	token = strtok(data, s);
	x2 = atoi(token);
	token = strtok(data, s);
	y2 = 600 - atoi(token);
	token = strtok(data, s);
	r = atoi(token);
	token = strtok(data, s);
	g = atoi(token);
	token = strtok(data, s);
	b = atoi(token);*/

	auto args = Parse(data);
	if (7 <= args.size())
	{
		x1 = atoi(args[0].c_str());
		x2 = atoi(args[2].c_str());

		y1 = 600 - atoi(args[1].c_str());
		y2 = 600 - atoi(args[3].c_str());


		r = atoi(args[4].c_str());
		g = atoi(args[5].c_str());
		b = atoi(args[6].c_str());
	}



}

FilePointer::FilePointer()
{
	fp = nullptr;
}
FilePointer::~FilePointer()
{
	Close();
}
void FilePointer::Open(const char fName[], const char mode[])
{
	fp = fopen(fName, mode);
}
void FilePointer::Close(void)
{
	if (nullptr != fp)
	{
		fclose(fp);
		fp = nullptr;
	}
}


class Drawing2D
{
protected:
	int num_lines = 0;
	Line2D* line = nullptr;

public:
	int ReadFile(const char fn[]);
	void Draw() const;
	Drawing2D();
	~Drawing2D();
	void CleanUp(void);
};

Drawing2D::Drawing2D()
{
	num_lines = 0;
	line = nullptr;
}
Drawing2D::~Drawing2D()
{
	CleanUp();
}

void Drawing2D::CleanUp(void)
{
	if (nullptr != line)
	{
		delete[] line;
		line = nullptr;
	}
	num_lines = 0;
}

int Drawing2D::ReadFile(const char fn[])
{
	CleanUp();
	FilePointer f;

	/*while (true)
	{
		char fn[256];  // File name
		printf("Enter File Name>");

		YsFgets(fn, 255, stdin);
		f.Open(fn, "r");
		if (f.fp != nullptr)
		{
			bool lineone = true;
			int nline = 0;
			char str[256];
			while (nullptr != YsFgets(str, 255, f.fp))
			{
				if (true == lineone)
				{
					num_lines = atoi(str);
					nline = 0;
					line = new Line2D[num_lines];
					lineone = false;
				}
				else
				{
					line[nline].MakeFromString(str);
					++nline;
				}
			}

			//fclose(fp);
			break;
		}
		else
		{
			printf("Cannot open a file.Please try again\n");

		}*/

	//char fn[256];  // File name
	//printf("Enter File Name>");
	f.Open(fn, "r");
	if (f.fp != nullptr)
	{
		bool lineone = true;
		int nline = 0;
		char str[256];
		while (nullptr != YsFgets(str, 255, f.fp))
		{
			if (true == lineone)
			{
				num_lines = atoi(str);
				nline = 0;
				line = new Line2D[num_lines];
				lineone = false;
			}
			else
			{
				line[nline].MakeFromString(str);
				++nline;
			}
		}
		return 1;

	}
	else
	{
		printf("Cannot open a file.Please try again\n");
		return 0;

	}


}
void Drawing2D::Draw(void) const
{
	for (int i = 0; i < num_lines; i++)
	{
		line[i].Draw();
	}
}


int main(void)
{
	Drawing2D Drawing;
	char fn[256];  // File name
	printf("Enter File Name>");
	YsFgets(fn, 255, stdin);
	int a;
	
	a= Drawing.ReadFile(fn);
	if (a == 0)
	{
		return 0;
	}
	
	FsOpenWindow(16, 16, 800, 600, 1);
	//glRotated(180, 1, 0, 0);

	for (;;)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FsPollDevice();
		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}

		Drawing.Draw();
		FsSwapBuffers();

	}

	return 0;
}