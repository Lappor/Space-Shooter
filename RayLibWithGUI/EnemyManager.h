#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "Enemy.h"
#include <iostream>

class EnemyManager
{
public:
    using iterator = std::vector<Enemy>::iterator;
    using const_iterator = std::vector<Enemy>::const_iterator;

    EnemyManager() = default;

    void spawn(Enemy enemy,
               bool randomizePos = true,
               int minX = 0, int maxX = 1300, int minY = 0, int maxY = 900,
               int playerX = 0, int playerY = 0)
    {
        // Debug: report template path before loading
        std::cout << "[EnemyManager::spawn] textureAssetPath='" << enemy.textureAssetPath << "'\n";

        // Try to ensure texture is loaded for this instance and report result.
        if (!enemy.textureAssetPath.empty())
        {
            enemy.ensureTextureLoaded();
            bool hasTex = enemy.hasTexture();
            unsigned int texId = hasTex ? enemy.texturePtr->id : 0;
            std::cout << "[EnemyManager::spawn] after ensureTextureLoaded: hasTexture=" << (hasTex ? "yes" : "no")
                      << " texId=" << texId << "\n";
        }
        else
        {
            std::cout << "[EnemyManager::spawn] no textureAssetPath set for this enemy\n";
        }

        if (randomizePos)
        {
            enemy = enemy.randomSpawnLoc(enemy, minX, maxX, minY, maxY, playerX, playerY);
        }

        items.push_back(std::move(enemy));
    }

    void clear() { items.clear(); }
    size_t size() const { return items.size(); }
    Enemy& operator[](size_t i) { return items[i]; }
    const Enemy& operator[](size_t i) const { return items[i]; }
    Enemy& back() { return items.back(); }
    iterator begin() { return items.begin(); }
    iterator end() { return items.end(); }

    iterator erase(iterator pos) { return items.erase(pos); }
    iterator erase(iterator first, iterator last) { return items.erase(first, last); }
    void eraseAt(size_t idx) { if (idx < items.size()) items.erase(items.begin() + idx); }

    template<typename Pred>
    void removeIf(Pred pred) {
        items.erase(std::remove_if(items.begin(), items.end(), pred), items.end());
    }

    void removeDead() {
        items.erase(std::remove_if(items.begin(), items.end(), [](const Enemy& e) {
            return e.health <= 0;
        }), items.end());
    }

private:
    std::vector<Enemy> items;
};