#include "Game.h"
#include<fstream>
#include<iostream>



Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	//Read in config file here

	std::ifstream fin(path);
	if (fin.is_open())
	{
		std::string input;
		while (fin >> input)
		{
			if (input == "Window" || input == "window")
			{
				fin >> m_windowConfig.WW >> m_windowConfig.WH >> m_windowConfig.FL >> m_windowConfig.FS;

			}

			else if (input == "font" || input == "Font")
			{
				fin >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
				if (!m_font.loadFromFile(RESOURCES_PATH + m_fontConfig.F))
				{
					//some error handling
					std::cout << "failed to load tech font from file";
					exit(-1);
				}
			}

			else
			{
				std::cout << "invalid input :" << input << std::endl;
			}
		}

		//set up default window parameters
		if (m_windowConfig.FS == 1)
		{
			m_window.create(sf::VideoMode(m_window.getSize().x, m_window.getSize().y), "Boids Sim", sf::Style::Fullscreen);
		}
		else
		{
			m_window.create(sf::VideoMode(m_windowConfig.WW, m_windowConfig.WH), "Boids sim");
		}

		m_window.setFramerateLimit(m_windowConfig.FL);
		m_window.setKeyRepeatEnabled(false);

		//ImGUI setup

		if (!ImGui::SFML::Init(m_window)) {};
		//you can use whatever imgui theme you like!
		//ImGui::StyleColorsDark();				
		//imguiThemes::yellow();
		//imguiThemes::gray();
		//imguiThemes::green();
		//imguiThemes::red();
		//imguiThemes::gray();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.FontGlobalScale = 1.5f;
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
		//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;



		m_text.setFont(m_font);
		m_text.setCharacterSize(m_fontConfig.S);
		m_text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));
		m_text.setPosition(0, 0);
		m_gridLines.setPrimitiveType(sf::Lines);

		//spawnPlayer();
	}
	else
	{
		std::cout << "unable to open file" <<std::endl;
	}
}

void Game::run()
{

	m_running = true;
	srand(time(NULL)); 
	
	int cellsize = static_cast<int>(m_visionDistance) + 10;
	m_grid.init(m_window.getSize().x, m_window.getSize().y, cellsize);
	drawGrid();
	
	sBoidSpawner(m_boidsToSpawn);
	while (m_running)
	{
		m_entities.update();
		

		//calculate the delta time
		sf::Time deltaTime = m_deltaClock.restart();
		float deltaTimeSeconds = deltaTime.asSeconds();

		//make sure delta time stays within normal bounds, like between one FPS and zero FPS
		deltaTimeSeconds = std::min(deltaTimeSeconds, 1.f);
		deltaTimeSeconds = std::max(deltaTimeSeconds, 0.f);

		ImGui::SFML::Update(m_window, deltaTime);

		if (!m_paused)  
		{
			sFlocking(); 
			sCollision();
			sMovement();
		}

		sUserInput();
		sGUI();
		sRender();
		
		//increment the current frame
		m_currentFrame++;
		
	}
}

void Game::setPaused(bool paused)
{
	m_paused = !paused;
}

std::shared_ptr<Entity> Game::player()
{
	auto& players = m_entities.getEntities("Player");
	return players.front();
}

void Game::sMovement()
{
	
	for (auto& e : m_entities.getEntities())
	{
		
		auto& transform = e->get<CTransform>();
		
		//auto& shape = e->get<CShape>().polygon;

		// 1. Calculate Rotation Angle
		// atan2(y, x) gives radians. Convert to degrees.
		//float angle = std::atan2(transform.velocity.y, transform.velocity.x) * (180.0f / 3.14159f);

		// 2. Set Rotation
		//shape.setRotation(angle);


		transform.pos += transform.velocity;


	}
}

void Game::sUserInput()
{
	//TODO : handle user input here

	//note: that you should only be setting the player's input component variables here
	// you should not implement the movement logic here
	// the movement system will read the variables you set in this function

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(m_window, event);
		
		//this event triggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
			m_window.close();
		}
		else if (event.type == sf::Event::Resized)
		{
			// Adjust the viewport when the window is resized
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			m_window.setView(sf::View(visibleArea));
		}

		//this event is triggered when a key is pressed 
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				std::cout << "P key pressed\n";
				setPaused(m_paused);
				break;
			case sf::Keyboard::Escape:
				std::cout << "Esc key pressed\n";
				m_running = false;
				break;
			case sf::Keyboard::Space:
				std::cout << "Space key pressed\n";
				spawnBoid();
				break;
			default: break;
			}
		}

		//if (event.type == sf::Event::MouseButtonPressed && !m_paused)
		//{
		//	if (event.mouseButton.button == sf::Mouse::Left)
		//	{
		//		std::cout << "Left mouse button clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
		//		//spawnBullet(m_player,Vec2(event.mouseButton.x,event.mouseButton.y));
		//		spawnBullet(player(), Vec2(event.mouseButton.x, event.mouseButton.y));
		//	}

		//	if (event.mouseButton.button == sf::Mouse::Right)
		//	{
		//		std::cout << "Right mouse button clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
		//		//call special weapon here
		//	}
		//}
	}
}


void Game::sRender()
{
	//TODO : change the code below to draw all of the entities in case of multiple species 
	m_window.clear();


	auto& boids = m_entities.getEntities("Boid");
	sf::VertexArray va(sf::Triangles, boids.size() * 3);

	for (size_t i = 0; i < boids.size(); i++)
	{
		auto& b = boids[i];

		auto& pos = b->get<CTransform>().pos;
		auto& vel = b->get<CTransform>().velocity;
		auto& mesh = b->get<CMesh>();

		//calculating direction vector for boids rotation
		float len = vel.magnitude();

		Vec2 dir(0.0f, -1.0f); //default facing up

		dir = { vel.x / len, vel.y / len };

		//Calculate Perpendicular Vector(Side Vector) in 2D, perpendicular to (x, y) is (-y, x)
		Vec2 perp(-dir.y, dir.x);

		// Nose: Forward from center
		Vec2 v1 = pos + (dir * mesh.size);

		// Rear Left: Behind center, then shifted Left
		Vec2 v2 = pos - (dir * mesh.size) + (perp * mesh.width);

		// Rear Right: Behind center, then shifted Right
		Vec2 v3 = pos - (dir * mesh.size) - (perp * mesh.width);

		size_t idx = i * 3;

		// Vertex 1 (Nose)
		va[idx + 0].position = sf::Vector2f(v1.x, v1.y);
		va[idx + 0].color = mesh.color;

		// Vertex 2 (Rear Left)
		va[idx + 1].position = sf::Vector2f(v2.x, v2.y);
		va[idx + 1].color = mesh.color;

		// Vertex 3 (Rear Right)
		va[idx + 2].position = sf::Vector2f(v3.x, v3.y);
		va[idx + 2].color = mesh.color;


	}

	if (m_drawBoids)
	{
		///m_window.draw(e->get<CShape>().polygon);
		m_window.draw(va);
	}

	
	//e->get<CShape>().polygon.setPosition(e->get<CTransform>().pos.x, e->get<CTransform>().pos.y);


	if (m_drawDebugLines)
	{
		sf::VertexArray debugVelocityLines(sf::Lines);

		for (auto& e : boids)
		{
			auto& pos = e->get<CTransform>().pos;
			auto& vel = e->get<CTransform>().velocity;
				
			debugVelocityLines.append(sf::Vertex(sf::Vector2f(pos.x,pos.y), sf::Color::Red)); //start of line

			Vec2 endPos = pos + (vel * 20.0f);
			debugVelocityLines.append(sf::Vertex(sf::Vector2f(endPos.x, endPos.y), sf::Color::Red)); //end of line
		}

		m_window.draw(debugVelocityLines);

	}

	if(m_drawGrid)
		m_window.draw(m_gridLines);

	//Text rendering 
	//std::string score = "Score : " + std::to_string(m_score); 
	////std::cout << score << std::endl;
	//m_text.setString(score);
	//m_window.draw(m_text);
	

	ImGui::SFML::Render(m_window);



	m_window.display();
}


void Game::sBoidSpawner(int boidsToSpawn)
{
	//TODO : code which implements enemy spawning should go here
	for (int i = 0; i < boidsToSpawn; i++)
	{
		spawnBoid();
	}

	//if (m_currentFrame - m_lastBoidSpawnTime > m_enemyConfig.SI)
	//{
	//	spawnBoid();
	//}
	//		use(m_currentFrame - m_lastEnemySpawnTime) to determine
	//		how long it has been since the last enemy spawned
	
}

void Game::sCollision()
{

	//Handles screen wrapping only for now , later may introduce obstacle avoidance
	for (auto &e : m_entities.getEntities())
	{
		
		//screen wrapping
		if (e->tag() == "Boid")
		{
			//auto& shape = e->get<CShape>();
			auto& transform = e->get<CTransform>();
			
			//float width = shape.polygon.getLocalBounds().width;
			//float height = shape.polygon.getLocalBounds().height;

			float height = e->get<CMesh>().size;
			float width = e->get<CMesh>().width;


			if (transform.pos.y < -height)
			{
				transform.pos.y = m_window.getSize().y + height;
			}
			else if (transform.pos.y > m_window.getSize().y + height)
			{
				transform.pos.y = -height;
			}

			if (transform.pos.x < -width)
			{
				transform.pos.x = m_window.getSize().x + width;
			}
			else if (transform.pos.x > m_window.getSize().x + width)
			{
				transform.pos.x = -width;        
			}
			

		}

	}



}


void Game::sGUI()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	ImGui::PopStyleColor(2);

	float oldVisionDistance = m_visionDistance;

	ImGui::Begin("Flocking controls");
	ImGui::SliderFloat("Max speed", &m_maxBoidSpeed, 0.0f, 5.0f);
	ImGui::SliderFloat("Steering Strength", &m_steeringStrength, 0.001f, 0.10f);
	ImGui::SliderFloat("Avoid Radius", &m_avoidDistance, 10.0f, 30.0f);
	ImGui::SliderFloat("Vision Radius", &m_visionDistance, 10.0f, 65.0f);
	if (m_visionDistance != oldVisionDistance)
	{
		int newCellSize = static_cast<int>(m_visionDistance) + 10;
		m_grid.init(m_window.getSize().x, m_window.getSize().y, newCellSize);
		drawGrid();
	}

	
	ImGui::Separator();

	ImGui::SliderFloat("Separation", &m_seperationValue, 0.0f, 2.0f);
	ImGui::SliderFloat("Alignment", &m_alignmentValue, 0.0f, 2.0f);
	ImGui::SliderFloat("Cohesion", &m_cohesionValue, 0.0f, 2.0f);
	
	ImGui::SeparatorText("Debug options");
	ImGui::Checkbox("Draw debug velocity", &m_drawDebugLines);
	ImGui::Checkbox("Draw grid lines", &m_drawGrid);
	ImGui::Checkbox("Draw boids", &m_drawBoids);
	
	ImGui::Separator();
	ImGui::SliderInt("Boids to spawn", &m_boidsToSpawn, 1, 10000);
	
	if (ImGui::Button("Reset Simulation"))
	{
		resetSimultaion();
	}

	
	ImGui::SeparatorText("Performance");

	// %.1f means "floating point with 1 decimal place"
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Boids spawned : %d ", m_entities.getEntities("Boid").size());
	ImGui::End();



}

void Game::sFlocking()
{
	//get all boid entities
	auto& boids = m_entities.getEntities("Boid");

	//std::cout << "Boid Count: " << boids.size() << "\n";
	//rebuild the grid every frame 
	m_grid.clear();
	m_boidsTfCache.clear();

	m_boidsTfCache.reserve(boids.size());
	m_neighbourCache.reserve(m_maxNeighbours + 20);

	//loop through boids to populate the grid
	for (int i = 0; i < boids.size(); i++)
	{
		auto& b = boids[i];

		if (!b->has<CBoid>())	continue;

		// One-time pointer chase per frame 
		m_boidsTfCache.emplace_back(b->get<CTransform>());
		m_grid.add(i, m_boidsTfCache[i].pos.x, m_boidsTfCache[i].pos.y);


	}


	for (int i = 0; i < boids.size(); i++)
	{
		auto& b = boids[i];

		if (!b->has<CBoid>()) continue;

		Vec2 seperation(0, 0);
		Vec2 alignment(0, 0);
		Vec2 cohesion(0, 0);

		int neighbours = 0;

		auto& bVel = b->get<CTransform>().velocity;			//since we want to update it directly so getting from m_entities
		auto& bPos = m_boidsTfCache[i].pos;
		auto& bBoid = b->get<CBoid>();

		//get all nearby boids
		m_grid.getNearby(bPos.x, bPos.y, m_neighbourCache);

		// 3. Compare with every OTHER nearby boid (The "Neighbors")
		for (auto& index : m_neighbourCache)
		{
			if (index == i) continue;

			//auto& other = boids[index];


			auto& oPos = m_boidsTfCache[index].pos;
			auto& oVel = m_boidsTfCache[index].velocity;
			

			float distSq = bPos.distsq(oPos);

			//if (b->id() == 1) // Only print for the first boid (to avoid console spam)
			//{
			//	std::cout << "DistSq: " << distsq << " | VisionSq: " << (bBoid.visionDistance * bBoid.visionDistance) << "\n";
			//}


			//putting a cap on neighbours for handling cases of large visual distance and high cohesion that is multiple boids swarming around same local poistion
			if (neighbours >= m_maxNeighbours)	break; 

			//if within visual range
			if (distSq < (m_visionDistance * m_visionDistance))
			{
				neighbours++;

				alignment += oVel; //accumulate neighbour velocities

				cohesion += oPos; //accumualte neighbour positions

				//seperation
				if (distSq < (m_avoidDistance * m_avoidDistance))
				{
					Vec2 diff = bPos - oPos;
					seperation += diff / (distSq + 0.1f); //closer they are stronger the push , adding 0.1f to ensure denominator is never zero
				}
			}
		}

		//average and normalize the forces
		if (neighbours > 0)
		{
			//finish cohesion : calculate the avg center, and find vector towards it
			cohesion /= static_cast<float>(neighbours);
			cohesion -= bPos;

			//finish alignment : cal avg dxn
			alignment /= static_cast<float>(neighbours);

			//normalize all vectors
			cohesion.normalize();
			alignment.normalize();
			seperation.normalize();
			//b->get<CShape>().circle.setFillColor(sf::Color::Cyan);
		}
		

		
		
		Vec2 totalForce = (seperation * m_seperationValue) + (alignment * m_alignmentValue) + (cohesion * m_cohesionValue);

		auto& mesh = b->get<CMesh>();

		//coloring boids based on neighbour count
		if (neighbours >= 10 )
		{
			///b->get<CShape>().polygon.setFillColor(sf::Color::White);	
			mesh.color = sf::Color::Cyan;
		}
		else if (neighbours >=  5)
		{
			///b->get<CShape>().polygon.setFillColor(sf::Color::Green);
			mesh.color = sf::Color::Yellow;
		}
		else 
		{
			mesh.color = sf::Color::White;
			///b->get<CShape>().polygon.setFillColor(sf::Color::Cyan);
		}

		//apply to velocity
		bVel += totalForce * m_steeringStrength; //small factor to smooth movement

		//Clamp speed otherwise accelerate to infinity
		float speed = bVel.magnitude();
		if (speed > m_maxBoidSpeed)
		{
			bVel = (bVel / speed) * m_maxBoidSpeed;
		}

	}
}





void Game::spawnBoid()
{
	auto boid = m_entities.addEntity("Boid");
	//boid->add<CShape>(10, 3, sf::Color::White, sf::Color::White, 0);
	///boid->add<CShape>();

	boid->add<CMesh>();

	boid->add<CBoid>(); 

	//float r = boid->get<CShape>().circle.getRadius();
	///float x = boid->get<CShape>().polygon.getLocalBounds().width;
	///float y = boid->get<CShape>().polygon.getLocalBounds().height;
	float x = boid->get<CMesh>().size;
	float y = boid->get<CMesh>().width;

	int mx = x + (rand() % (size_t)(1 + (m_window.getSize().x - x) - x));
	int my = y + (rand() % (size_t)(1 + (m_window.getSize().y - y) - y));


	float sx = (static_cast<float>(rand()) / RAND_MAX) * (m_maxBoidSpeed - m_minBoidSpeed) + (m_minBoidSpeed); //random float number generation between max and min value
	float sy = (static_cast<float>(rand()) / RAND_MAX) * (m_maxBoidSpeed - m_minBoidSpeed) + (m_minBoidSpeed);

	int sAngle = (static_cast<float>(rand()) / RAND_MAX) * 360;

	boid->add<CTransform>(Vec2(mx, my), Vec2(sx, sy), sAngle);

	m_lastBoidSpawnTime = m_currentFrame;

}

void Game::resetSimultaion()
{
	auto& boids = m_entities.getEntities("Boid");
	for (auto& b : boids)
	{
		b->destroy();
	}

	sBoidSpawner(m_boidsToSpawn);
}

void Game::drawGrid()
{
	int rows = m_grid.rows(), cols = m_grid.columns();
	int totalVertices = ((rows+1) + (cols+1)) * 2;						//Times 2 since every line has start and end point

	m_gridLines.clear();
	m_gridLines.resize(totalVertices);

	int vertexIndex = 0;
	sf::Color gridColor = sf::Color(125, 125, 125);

	//vertical debugVelocityLines
	for (int i = 0; i <= cols; i++)
	{
		int x = i * m_grid.cellsize();

		//top points
		m_gridLines[vertexIndex].position = sf::Vector2f(x, 0);
		m_gridLines[vertexIndex].color = gridColor;
		vertexIndex++;
		
		//bottom points
		m_gridLines[vertexIndex].position = sf::Vector2f(x, m_window.getSize().y);
		m_gridLines[vertexIndex].color = gridColor;
		vertexIndex++;
	}

	//horizontal debugVelocityLines
	for (int j = 0; j <= rows; j++)
	{
		int y = j * m_grid.cellsize();

		//top points
		m_gridLines[vertexIndex].position = sf::Vector2f(0, y);
		m_gridLines[vertexIndex].color = gridColor;
		vertexIndex++;
		
		//bottom points
		m_gridLines[vertexIndex].position = sf::Vector2f(m_window.getSize().x, y);
		m_gridLines[vertexIndex].color = gridColor;
		vertexIndex++;
	}
}
