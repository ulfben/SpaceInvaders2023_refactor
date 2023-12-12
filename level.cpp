#include <iostream>
#include <fstream>
#include <vector> 
#include <string>
#include "level.h"

void LoadLevelFromFile(const std::string& filename) 
{
	std::ifstream file(filename); 

	if (file.is_open())
	{
		std::vector<Entity> entities;
		float x, y; 

		while (file >> x >> y) 
		{
		Entity entity;  
		entity.x = x;
		entity.y = y; 
		entities.push_back(entity); 
		}

	    file.close(); 

		for (int i = 0; i < entities.size(); i++)
		{
			const Entity& entity = entities[i]; 
			std::cout << "Spawn entity at X:" << entity.x << ",Y:" << entity.y << std::endl; 
		}
	}
	else 
	{
		std::cout << "Unable to open file:" << filename << std::endl; 
	}
	
}


	