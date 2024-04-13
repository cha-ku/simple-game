//
// Created by chaku on 23/09/23.
//

#include "AssetStore.hpp"
#include "Logger.hpp"

void AssetStore::ClearAssets() {
  for(auto& texture : textures) {
    SDL_DestroyTexture(texture.second);
  }
  textures.clear();
}

void AssetStore::AddTexture(const std::string& name, const std::string& filePath, SDL_Renderer* renderer) {
  SDL_Texture* value = nullptr;
  //std::string filePath(filePathSV);

  SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
  if (loadedSurface == nullptr) {
    Logger::Error("Could not load texture from " + filePath);
  }
  else {
    Logger::Info("Loaded texture from " + filePath);
    value = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (value == nullptr) {
      Logger::Error("Could not create texture from surface " + filePath);
    }
    else {
      Logger::Info("Created texture from surface " + filePath);
      SDL_FreeSurface(loadedSurface);
      textures[std::string(name)] = value;
    }
  }
}

SDL_Texture*& AssetStore::GetTexture(std::string& key) {
  return textures[key];
}