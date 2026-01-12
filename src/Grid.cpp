#include "Grid.h"

Grid::Grid() : m_rows(0), m_cols(0), m_cellSize(0) {}
	
Grid::Grid(int width, int height, int cellSize)
{
	init(width, height, cellSize);
}

void Grid::init(int width, int height, int cellSize) 
{
	if (cellSize == m_cellSize) return; // Don't do work if size hasn't changed

	m_cellSize = cellSize;
	m_cols = (width / m_cellSize) + 1;  // +1 to include the last col in case it was missed in divison
	m_rows = (height / m_cellSize) + 1; // +1 to include the last col in case it was missed in divison

	cells.clear();
	cells.resize(m_cols * m_rows);
}


int Grid::rows() const
{
	return m_rows;
}

int Grid::columns() const
{
	return m_cols;
}

int Grid::cellsize() const
{
	return m_cellSize;
}

void Grid::clear()
{
	for (auto& cell : cells)
	{
		cell.clear();
	}
}

int Grid::getIndex(float x, float y)
{
	int posX = static_cast<int>(x / m_cellSize);
	int posY = static_cast<int>(y / m_cellSize);

	if (posX < 0 || posX >= m_cols || posY < 0 || posY >= m_rows)	return -1;	//out of grid bounds

	return  (posY * m_cols) + posX;
}

int Grid::getIndex(int x, int y)
{

	if (x < 0 || x >= m_cols || y < 0 || y >= m_rows)	return -1;	//out of grid bounds

	return (y * m_cols) + x;
}

void Grid::add(int boidId, float x, float y)
{
	int index = getIndex(x, y);
	if(index != -1)
		cells[index].emplace_back(boidId);
}

void Grid::getNearby(float x, float y, std::vector<int>& result)
{
	result.clear();
	
	int centerCol = static_cast<int>(x / m_cellSize);
	int centerRow = static_cast<int>(y / m_cellSize);

	for (int offsetY = -1; offsetY <= 1; offsetY++)
	{
		for (int offsetX = -1; offsetX <= 1; offsetX++)
		{
			int index = getIndex(x + offsetX, y + offsetY);

			if (index == -1)	continue;

			const auto& bucket = cells[index];
			result.insert(result.end(), bucket.begin(), bucket.end());
			//result.emplace_back(bucket);
		}

	}
}

