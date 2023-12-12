#pragma once 
#include <iostream>
#include <string>
#include <fstream>


struct Entity
{
	float x, y;

	void LoadLevelFromAFile(const std::string& filename);
};