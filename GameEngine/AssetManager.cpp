#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* manager) : m_manager(manager)
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string textureId)
{
	auto& projectile(m_manager->addEntity());
	projectile.addComponent<TransformComponent>(position.m_x, position.m_y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(textureId, false);
	projectile.addComponent<ProjectileCompoment>(range, speed, velocity);
	projectile.addComponent<ColliderComponent>("Projectile");
	projectile.addGroup(Game::Projectiles);
}

void AssetManager::AddTexture(std::string id, const char* filePath)
{
	m_textures.emplace(id, TextureManager::LoadTexture(filePath));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return m_textures[id];
}
