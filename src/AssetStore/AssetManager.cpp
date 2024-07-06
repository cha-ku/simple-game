//
// Created by chaku on 23/09/23.
//

#include "AssetManager.hpp"
#include "Logger.hpp"

void AssetManager::ClearAssets() {
  for(auto& [key, texture] : textures) {
    SDL_DestroyTexture(texture);
  }
  textures.clear();
}

void AssetManager::AddTexture(const std::string_view name, const std::string& filePath, SDL_Renderer* renderer) {
  SDL_Texture* value = nullptr;

  SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

  if (loadedSurface == nullptr) {
    Logger::Error("Could not load texture from " + filePath);
    return;
  }

  Logger::Info("Loaded texture from " + filePath);
  value = SDL_CreateTextureFromSurface(renderer, loadedSurface);

  if (value == nullptr) {
    Logger::Error("Could not create texture from surface " + filePath);
    return;
  }

  Logger::Info("Created texture from surface " + filePath);
  SDL_FreeSurface(loadedSurface);
  textures[std::string(name)] = value;
}

std::optional<SDL_Texture*> AssetManager::GetTexture(const std::string& key) const {
  return textures.at(key);
}