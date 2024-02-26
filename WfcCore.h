//
// Created by Felix on 23/02/2024.
//

#ifndef WAVECOLLAPSEFUNCTION_WFCCORE_H
#define WAVECOLLAPSEFUNCTION_WFCCORE_H


#include <vector>
#include <set>
#include <random>
#include "Rules.h"

struct Tile {
    std::set<int> possibleTypes;
    int xPos;
    int yPos;
    float entropy;
};

class WfcCore {
public:
    WfcCore(const int& width, const int& height, const std::set<int>& defaultTypes);
    ~WfcCore();

    void SetDebug(bool debug);
    void PresetTile(int x, int y, int type);
    void Run(const Rules& rules, unsigned int seed = 0);
    [[nodiscard]] bool IsLastRunValid() const;
    std::vector<std::vector<Tile *>> GetResults();


private:
    const int m_width;
    const int m_height;
    const int tileCount = m_width * m_height;
    std::vector<Tile> holder = std::vector<Tile>(tileCount);
    std::vector<std::vector<Tile*>> scene;
    std::vector<Tile*> toCheck;
    std::vector<Tile*> toUpdate;
    bool lastRunValid = true;
    bool debug = false;

    std::random_device rd;
    std::mt19937 generator = std::mt19937(rd());

    std::vector<Tile *> GetNeighbours(Tile *pTile);

    void AddNeighborsToUpdateList(int x, int y);
    void UpdatePossibleTileTypes(Tile* tile, const Rules& rules);
    static bool IsTilePossible(Tile* tile);
    void Log(const std::string& message) const;
};


#endif //WAVECOLLAPSEFUNCTION_WFCCORE_H
