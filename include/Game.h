#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "Grid.h"

#include<SFML\Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

struct WindowConfig { int WW, WH, FL, FS; };
struct FontConfig	{ int S, R, G, B; std::string F; };
struct PlayerConfig	{ int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig	{ int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow	m_window;			
	EntityManager		m_entities;			//Vector of entities to maintain
	sf::Font			m_font;				//the font which will be use to draw
	sf::Text			m_text;				//the score text to be drawn to the screen
	sf::Clock			m_deltaClock;		//for imgui 
	WindowConfig		m_windowConfig;
	FontConfig			m_fontConfig;
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_bulletConfig;
	int					m_score = 0;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	int					m_lastBoidSpawnTime = 0;
	bool				m_paused = false;	//whether we update game logic
	bool				m_running = false;  //if game is running

	//boids specific vars

	Grid				m_grid;

	int					m_boidsToSpawn = 200;

	float				m_seperationValue = 1.5f;
	float				m_alignmentValue = 1.0f;
	float				m_cohesionValue = 1.0f;
	float				m_visioinDistance = 75.0f;
	float				m_avoidDistance = 30.0f;
	float				m_maxBoidSpeed = 3.0f;
	float				m_agility = 0.05f;

	bool				m_drawDebugLines = false;
	bool				m_drawBoids = true;
	bool				m_drawGrid = false;

	sf::VertexArray		m_gridLines;

	
	//std::shared_ptr<Entity> m_player;

	void init(const std::string& config);	//intializing game state with a config file path
	void setPaused(bool paused);			//pause the game

	std::shared_ptr<Entity> player();

	void sMovement();						//System : Entity position / movement update
	void sUserInput();						//System : User Input
	void sLifespan();						//System : Lifespan
	void sRender();							//System : Render / Drawing
	void sEnemySpawner();					//System : Spawns Enemies
	void sCollision();						//System : Collisions
	void sGUI();							//System : ImGUI interface


	//Boids systems
	void sBoidSpawner(int boidsToSpawn);					//System : boids Spawner 
	void sFlocking();						//System : boids flocking



	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity , const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

	//Boids functions
	void spawnBoid();
	void resetSimultaion();
	void drawGrid();




public:
	
	Game(const std::string& config); //constructor takes in game config

	void run();
};