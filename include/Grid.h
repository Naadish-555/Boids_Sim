#pragma once
#include <vector>

class Grid
{

	int m_cellSize;										//Must be >= Vision Radius
	int m_rows;
	int m_cols;
	std::vector<std::vector<int>> cells;				//vector for storing ids of boids within the grid
	

public:
	Grid();
	Grid(int width, int height, int cellSize);

	void init(int width, int height, int cellSize);

	int rows() const;
	int columns() const;
	int cellsize() const;

	int getIndex(float x, float y);
	int getIndex(int x, int y);


	void clear();										//Wipe the grid clean at the start of every frame

	void add(int boidId, float x, float y);			//Put a boid ID into the correct cell

	void getNearby(float x, float y, std::vector<int>& result);		//Return a list of all boid IDs in the relevant cells

};