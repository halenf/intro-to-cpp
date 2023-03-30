#pragma once

#include <array>

class Application
{
public:
	Application();
	~Application();

	void Run();

	void Load();
	void Unload();

	void Update(float dt);
	void Draw();

	int CalculateTileState(int index, int* currentState);

protected:
private:

	int m_windowWidth = 1200;
	int m_windowHeight = 800;
	
	int m_cols = 120;
	int m_rows = 80;

	int* m_grid = nullptr;
	int* m_gridBuffer = nullptr;

};
