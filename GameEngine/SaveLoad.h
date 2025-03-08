#pragma once

#include <string>
#include <vector>

class Entity;

void SaveEntities(const std::vector<Entity*>& entities, const std::string& filename);

void LoadEntities(const std::string& filename);
