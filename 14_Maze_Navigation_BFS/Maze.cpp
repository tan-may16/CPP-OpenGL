#include <sstream>
#include <fstream>
#include <iostream>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "DrawingUtilNG.h"
#include "Maze.h"

using namespace std;

void Maze::readFile(std::ifstream &input)
{
	string wholeLineString;  
	stringstream lineStream; 
	int colonLocation; 

	while (!input.eof()) {
		lineStream.clear();

		getline(input, wholeLineString);
		if ((colonLocation = wholeLineString.find(":")) != string::npos)
			lineStream.str(wholeLineString.substr(colonLocation + 1));
		else {
			colonLocation = -1;
			lineStream.str(wholeLineString);
		}

		if (wholeLineString.find("Start:") != string::npos) 
			lineStream >> startRow >> startCol; 

		else if (wholeLineString.find("End:") != string::npos) 
			lineStream >> endRow >> endCol;

		else if (wholeLineString.find("Begin:") != string::npos) {
			lineStream >> colSize;
			bool continueReadingMap = true;
			rowSize = 0;
			while (continueReadingMap && !input.eof()) { 
				getline(input, wholeLineString);
				if (wholeLineString.find("End:") != string::npos)
					continueReadingMap = false;
				else {
					lineStream.clear();
					lineStream.str(wholeLineString);
					rowSize++;
					for (int j = 1; j <= colSize; j++)  
						lineStream >> map[rowSize][j];
				}
			}
		}
	}
}

void Maze::paint(bool useFancyBackground)
{
	using namespace DrawingUtilNG;

	glColor3ub(100, 100, 100);
	drawRectangle(0, 0, (MAX_MAP_SIZE - 1) * BLOCK_SIZE, (MAX_MAP_SIZE - 1) * BLOCK_SIZE, true);

	if (useFancyBackground) {
		glShadeModel(GL_SMOOTH);
		glBegin(GL_QUADS);
		glColor3ub(240, 0, 240); 
		glVertex2i(0, 0); 
		glColor3ub(0, 240, 240);
		glVertex2i(colSize * BLOCK_SIZE, 0); 
		glColor3ub(0, 0, 240); 
		glVertex2i(colSize * BLOCK_SIZE, rowSize * BLOCK_SIZE);
		glColor3ub(240, 240, 0); 
		glVertex2i(0, rowSize * BLOCK_SIZE);
		glEnd();
		glShadeModel(GL_FLAT); 

	}
	else {
		
		glColor3ub(0, 0, 0);
		drawRectangle(0, 0, colSize * BLOCK_SIZE, rowSize * BLOCK_SIZE, true);
	}

	
	glColor3ub(240, 240, 240);
	for (int i = 1; i <= rowSize; i++) {
		for (int j = 1; j <= colSize; j++) {
			if (!map[i][j]) {
				drawRectangle((j - 1) * BLOCK_SIZE, (i - 1) * BLOCK_SIZE,
					BLOCK_SIZE, BLOCK_SIZE, true);
			}
		}
	}

	
	glColor3ub(0, 255, 0);  // green
	drawRectangle((startCol - 1) * BLOCK_SIZE, (startRow - 1) * BLOCK_SIZE,
		BLOCK_SIZE, BLOCK_SIZE, true);
	glColor3ub(255, 0, 0);  // red
	drawRectangle((endCol - 1) * BLOCK_SIZE, (endRow - 1) * BLOCK_SIZE,
		BLOCK_SIZE, BLOCK_SIZE, true);

}

void Maze::quarterTurn()
{
	
	bool temp[MAX_MAP_SIZE][MAX_MAP_SIZE];
	for (int i = 1; i < MAX_MAP_SIZE; i++)
		for (int j = 1; j < MAX_MAP_SIZE; j++)
			temp[i][j] = map[i][j];

	swap(rowSize, colSize);
	for (int i = 1; i <= rowSize; i++)
		for (int j = 1; j <= colSize; j++) {
			map[i][j] = temp[colSize - j + 1][i];
		}

	if (startRow != -1 && endRow != -1) {
		int newStartCol = colSize - startRow + 1;
		int newEndCol = colSize - endRow + 1;
		startRow = startCol;
		startCol = newStartCol;
		endRow = endCol;
		endCol = newEndCol;
	}
}

void Maze::mirrorOnDiagonal()
{
	for (int i = 1; i < MAX_MAP_SIZE; i++)
		for (int j = i; j < MAX_MAP_SIZE; j++) {
			swap(map[i][j], map[j][i]);
		}

	if (startRow != -1 && endRow != -1) {
		swap(rowSize, colSize);
		swap(startRow, startCol);
		swap(endRow, endCol);
	}
}

void Maze::mirrorLeftRight()
{
	for (int i = 1; i <= rowSize; i++)
		for (int j = 1; j <= colSize / 2; j++) {
			swap(map[i][j], map[i][colSize - j + 1]);
		}

	if (startRow != -1 && endRow != -1) {
		startCol = colSize - startCol + 1;
		endCol = colSize - endCol + 1;
	}
}

void Maze::mirrorUpDown()
{
	for (int i = 1; i <= rowSize / 2; i++)
		for (int j = 1; j <= colSize; j++) {
			swap(map[i][j], map[rowSize - i + 1][j]);
		}

	if (startRow != -1 && endRow != -1) {
		startRow = rowSize - startRow + 1;
		endRow = rowSize - endRow + 1;
	}
}

int Maze::sumOfMap()
{
	int navigableCount = 0;
	for (int i = 1; i <= rowSize; i++)
		for (int j = 1; j <= colSize; j++)
			if (!map[i][j]) navigableCount++;
	return navigableCount;
}


void Maze::changeMapState(int mouseX, int mouseY, int key)
{
	int selectedRow = mouseY / BLOCK_SIZE + 1; 
	int selectedCol = mouseX / BLOCK_SIZE + 1;

	if (1 <= selectedRow && selectedRow <= rowSize && 1 <= selectedCol && selectedCol <= colSize) {
		if (key == FSMOUSEEVENT_LBUTTONDOWN) { 
			if ((selectedRow != startRow && selectedRow != endRow)  
				|| (selectedCol != startCol && selectedCol != endCol) )
				map[selectedRow][selectedCol] = !map[selectedRow][selectedCol];
		}
		else if (key == FSKEY_S && !map[selectedRow][selectedCol]) {
			startRow = selectedRow;
			startCol = selectedCol;
		}
		else if (key == FSKEY_E && !map[selectedRow][selectedCol]) { 
			endRow = selectedRow;
			endCol = selectedCol;
		}
	}
}

bool Maze::isNavigable(int row, int col)
{
	return (0 < row && row <= rowSize
		&& 0 < col && col <= colSize
		&& !map[row][col]);
}

std::ostream& operator<<(std::ostream &os, const Maze& aMaze)
{
	os << "Start: " << aMaze.startRow << " " << aMaze.startCol << endl;
	os << "End: " << aMaze.endRow << " " << aMaze.endCol << endl;
	os << "Map Begin: " << aMaze.colSize << endl;
	for (int i = 1; i <= aMaze.rowSize; i++) {
		for (int j = 1; j <= aMaze.colSize; j++)
			os << aMaze.map[i][j] << " ";
		os << endl;
	}
	os << "Map End:" << endl;
	return os;
}

