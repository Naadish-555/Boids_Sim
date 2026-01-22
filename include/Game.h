#pragma once

#include "Entity.hpp"
#include "EntityManager.hpp"
#include "Grid.hpp"

#include<SFML\Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

struct WindowConfig { int WW, WH, FL, FS; };
struct FontConfig	{ int S, R, G, B; std::string F; };

class Game
{
	sf::RenderWindow	m_window;			
	EntityManager		m_entities;						//Vector of entities to maintain
	sf::Font			m_font;							//the font which will be use to draw
	sf::Text			m_text;							//the score text to be drawn to the screen
	sf::Clock			m_deltaClock;					//for imgui 
	WindowConfig		m_windowConfig;
	FontConfig			m_fontConfig;
	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastBoidSpawnTime = 0;
	bool				m_paused = false;				//whether we update game logic
	bool				m_running = false;				//if game is running

	
	//boids specific vars
	Grid				m_grid;

	int					m_boidsToSpawn = 200;
	int					m_maxNeighbours = 30;			//max neighbours a boid can have within the visual range

	float				m_seperationValue	= 1.5f;
	float				m_alignmentValue	= 1.0f;
	float				m_cohesionValue		= 1.0f;
	float				m_visionDistance	= 45.0f;
	float				m_avoidDistance		= 15.0f;
	float				m_maxBoidSpeed		= 3.0f;
	float				m_minBoidSpeed		= 0.1f;
	float				m_steeringStrength	= 0.01f;

	bool				m_drawDebugLines = false;
	bool				m_drawBoids = true;
	bool				m_drawGrid = false;

	sf::VertexArray		m_gridLines;

	std::vector<CTransform>		m_boidsTfCache;			//used in sFlocking to improve performance (fetching from m_entities every frame very slow)
	std::vector<int>			m_neighbourCache;		


	//Predator specific vars

	int					m_predatorLoopTraverseMode = 2;

	float				m_predatorTimeAccumulator = 0.0f;
	float				m_predatorFOV   = 150.0f;
	float				m_fearRadius	= 150.0f;
	float				m_fearStrength	= 3.0f;
	float				m_predatorDesiredLoopRadius = 150.0f;
	float				m_predatorCurrentLoopRadius = 150.0f;
	float				m_predatorSpeed = 150.0f;

	bool				m_predatorActive = false;
	bool				m_drawPredator = true;

	const char*			m_predatorTraverseList[3] = { "Oval", "Figure-8 Loop", "Spiral" };

	void init(const std::string& config);				//intializing game state with a config file path
	void setPaused(bool paused);						//pause the game

	std::shared_ptr<Entity> player();

	void sMovement(float dt);							//System : Entity position / movement update
	void sUserInput();									//System : User Input
	void sRender();										//System : Render / Drawing
	void sCollision();									//System : Collisions
	void sGUI();										//System : ImGUI interface


	//Boids systems
	void sBoidSpawner(int boidsToSpawn);				//System : boids Spawner 
	void sFlocking();									//System : boids flocking



	//Boids functions
	void spawnBoid();
	void resetSimultaion();
	void drawGrid();

	void spawnPredator();




public:
	
	Game(const std::string& config);					//constructor takes in game config

	void run();
};