//
// Created by Felix on 23/02/2024.
//

#include <algorithm>
#include <string>
#include <format>
#include <iostream>
#include <cmath>
#include <random>
#include "WfcCore.h"

#define LOG(x) std::cout << x << std::endl

WfcCore::WfcCore(const int& width, const int& height, const std::set<int>& defaultTypes)
    :m_width(width), m_height(height)
{
    scene = std::vector<std::vector<Tile*>>(m_height, std::vector<Tile*>(m_width));
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            scene[i][j] = &holder[i*width+j];
            scene[i][j]->xPos = i;
            scene[i][j]->yPos = j;
            scene[i][j]->possibleTypes = std::set<int>(defaultTypes);
        }
    }
}

WfcCore::~WfcCore() = default;

void WfcCore::PresetTile(int x, int y, int type) {
    scene[x][y]->possibleTypes.clear();
    scene[x][y]->possibleTypes.insert(type);
    AddNeighborsToUpdateList(x, y);
}

void WfcCore::AddNeighborsToUpdateList(int x, int y) {
    if (x > 0) {
        toUpdate.push_back(scene[x - 1][y]);
    }
    if (x < m_height - 1) {
        toUpdate.push_back(scene[x + 1][y]);
    }
    if (y > 0) {
        toUpdate.push_back(scene[x][y - 1]);
    }
    if (y < m_width - 1) {
        toUpdate.push_back(scene[x][y + 1]);
    }
}

void WfcCore::Run(const Rules& rules, unsigned int seed) {
    if(seed == 0){
        seed = rd();
    }
    generator = std::mt19937(seed);
    int seedValue;
    LOG("Seed: " + std::to_string(seed));

    toUpdate.clear();
    toCheck.clear();

    LOG("Running WFC");
    lastRunValid = true;
    for (auto & i : scene) {
        for (auto & j : i) {
            toCheck.push_back(j);
        }
    }
    bool failure = false;
    while(!toCheck.empty() && !failure){

        for (auto & i : toUpdate) {
            UpdatePossibleTileTypes(i, rules);
            if(!IsTilePossible(i)){
                std::string message  = std::format("No possible types for tile at x: {} y: {}", i->xPos, i->yPos);
                LOG(message);
                failure = true;
                break;
            }
        }
        if(failure){
            lastRunValid = false;
            break;
        }

        //calculate shannon entropy
        for (auto & i : toCheck) {
            auto & tile = i;
            float entropy = 0.0f;
            float sum = 0.0f;

            for (auto & type : tile->possibleTypes) {
                sum += rules.GetSelfWeight(type);
                //sum += 1;
            }
            for (auto & type : tile->possibleTypes) {
                float p = rules.GetSelfWeight(type) / sum;
                //float p = 1 / sum;
                entropy -= p * std::log2(p);
            }

            tile->entropy = entropy;
        }

        //sort by entropy
        std::sort(toCheck.begin(), toCheck.end(), [](Tile* a, Tile* b) {
            return a->entropy < b->entropy;
        });

        int amountOfTilesToCheck = (int)toCheck.size();
        //remove tiles with 0 entropy
        while(!toCheck.empty() && toCheck.front()->entropy == 0.0f){
            toCheck.erase(toCheck.begin());
        }
        std::string messageA = std::format("Removed {} tiles with 0 entropy", amountOfTilesToCheck - (int)toCheck.size());
        Log(messageA);

        if (toCheck.empty()) {
            std::string messageB = "toCheck is empty, ending";
            LOG(messageB);
            break;
        }

        //get tile with the lowest entropy
        Tile* tile = toCheck.front();
        std::string message  = std::format("Tile at x: {} y: {} has lowest entropy: {}", tile->xPos, tile->yPos, tile->entropy);
        Log(message);

        int tileTypesAmount = tile->possibleTypes.size();
        std::vector<int> possibleTypesVectorDebug(tile->possibleTypes.begin(), tile->possibleTypes.end());

        UpdatePossibleTileTypes(tile, rules);

        if(!IsTilePossible(tile)){
            std::string message  = std::format("No possible types for tile at x: {} y: {}", tile->xPos, tile->yPos);
            Log(message);
            break;
        }

        AddNeighborsToUpdateList(tile->xPos, tile->yPos);

        if(tileTypesAmount != tile->possibleTypes.size()){
            continue;
        }

        if(tile->possibleTypes.size() == 1){
            LOG("Tile has only one possible type, should already be removed from toCheck list");
        }
        if(!tile->possibleTypes.empty()){
            std::vector<int> possibleTypesVector(tile->possibleTypes.begin(), tile->possibleTypes.end());
            std::vector<float> weightsVectorSelf;
            for (auto & type : possibleTypesVector) {
                weightsVectorSelf.push_back(rules.GetSelfWeight(type));
            }

//            std::vector<int> neighbourTypes = GetNeighbourTypes(scene, height, width, tile);
//            std::vector<float> weightsVectorNeighbourRules;
//            for (auto & type : possibleTypesVector){
//                float weight = 0.0f;
//                int countInNeighbourTypes = (int) std::count(neighbourTypes.begin(), neighbourTypes.end(), type);
//                if(countInNeighbourTypes > 0 && rules.contains(type)){
//                    int counterInRules = 0;
//                    for (auto & ruleResult : rules.at(type)) {
//                        weight += weights[ruleResult];
//                        counterInRules++;
//                    }
//                    weight /= (float)counterInRules;
//                }
//                weight *= (float)countInNeighbourTypes;
//                weightsVectorNeighbourRules.push_back(weight);
//            }
//
//            std::vector<float> weightsCombined;
//            for (int i = 0; i < possibleTypesVector.size(); i++) {
//                weightsCombined.push_back(weightsVectorSelf[i] * weightsVectorNeighbourRules[i]);
//            }

            //log weights from calculation
//            std::string message = std::format("Weights for tile at x: {} y: {} ", tile->xPos, tile->yPos);
//            for(int i = 0; i < possibleTypesVector.size(); i++){
//                message += std::format("{}:({} * {}) = {},", tileTypes[possibleTypesVector[i]], weightsVectorSelf[i], weightsVectorNeighbourRules[i], weightsCombined[i]);
//            }
//            LOG(message);

            std::discrete_distribution<> distribution(weightsVectorSelf.begin(), weightsVectorSelf.end());
            //std::uniform_int_distribution<int> distribution(0, tile->possibleTypes.size()-1);

            int randomIndex = distribution(generator);
            //auto it = tile->possibleTypes.begin();
            //std::advance(it, randomIndex);
            //int chosenType = *it;
            int chosenType = possibleTypesVector[randomIndex];

            tile->possibleTypes = {chosenType};
            message = std::format("Chosen type for tile at x: {} y: {} is: {}", tile->xPos, tile->yPos, chosenType);
            message += " - from possible types: ";
            for (auto & type : possibleTypesVector) {
                message += std::format("{} ", type);
            }
            Log(message);
        }
        else{
            std::string message  = std::format("No possible types for tile at x: {} y: {}", tile->xPos, tile->yPos);
            Log(message);
            break;
        }
    }
}

std::vector<Tile *> WfcCore::GetNeighbours(Tile *tile) {
    std::vector<Tile*> neighbours = std::vector<Tile *>();
    if (tile->xPos > 0) {
        neighbours.push_back(scene[tile->xPos - 1][tile->yPos]);
    }
    if (tile->xPos < m_height - 1) {
        neighbours.push_back(scene[tile->xPos + 1][tile->yPos]);
    }
    if (tile->yPos > 0) {
        neighbours.push_back(scene[tile->xPos][tile->yPos - 1]);
    }
    if (tile->yPos < m_width - 1) {
        neighbours.push_back(scene[tile->xPos][tile->yPos + 1]);
    }
    return neighbours;
}

void WfcCore::UpdatePossibleTileTypes(Tile* tile, const Rules& rules) {
    std::set<int> possibleTypes = tile->possibleTypes;
    std::vector<Tile*> neighbourTypes = GetNeighbours(tile);

    for (auto & neighbour : neighbourTypes) {
        std::set<int> gatheredAllowedByNeighbourTypes = rules.GetPossibleNeighbourTypes(neighbour->possibleTypes);
        std::set<int> newPossibleTypes;
        std::set_intersection(possibleTypes.begin(), possibleTypes.end(), gatheredAllowedByNeighbourTypes.begin(), gatheredAllowedByNeighbourTypes.end(), std::inserter(newPossibleTypes, newPossibleTypes.begin()));
        possibleTypes = newPossibleTypes;
    }
    tile->possibleTypes = possibleTypes;
}

bool WfcCore::IsTilePossible(Tile* tile){
    if(tile->possibleTypes.empty()){
        return false;
    }
    return true;
}

std::vector<std::vector<Tile *>> WfcCore::GetResults() {
    return scene;
}

bool WfcCore::IsLastRunValid() const {
    return lastRunValid;
}

void WfcCore::SetDebug(bool debug) {
    this->debug = debug;
}

void WfcCore::Log(const std::string& message) const {
    if(debug)
    {
        LOG(message);
    }
}


