
struct Cell {
	int row, col;
};

class Maze {
public:
	static const int BLOCK_SIZE = 20; 
	static const int MAX_MAP_SIZE = 41;  

private:
	int startRow = -1;
	int startCol = -1;
	int endRow = -1;
	int endCol = -1;
	int rowSize = -1;
	int colSize = -1;
	bool map[100][100] = {true};

public:

	Maze() { };
	void readFile(std::ifstream& input);

	void paint(bool useFancyBackground = false);

	friend std::ostream& operator<<(std::ostream& os, const Maze& aMaze);

	int getRowSize() { return rowSize; }
	int getColSize() { return colSize; }
	Cell getStart() { return { startRow, startCol }; }
	Cell getEnd() { return { endRow, endCol }; }

	bool isNavigable(int row, int col);

	void quarterTurn();

	void mirrorOnDiagonal();

	void mirrorLeftRight();

	void mirrorUpDown();

	int sumOfMap();

	
	void changeMapState(int mouseX, int mouseY, int key);

};