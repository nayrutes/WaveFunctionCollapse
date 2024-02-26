//
// Created by Felix on 23/02/2024.
//

#include <string>
#include <format>
#include <iostream>
#include "Rules.h"

#define LOG(x) std::cout << x << std::endl

Rules::Rules() {

}

Rules::~Rules() {

}

void Rules::AddTwoWayRule(std::unordered_map<char, int> tileTypeMap, char a, char b, float weight) {
    ruleSet.insert({tileTypeMap[a], tileTypeMap[b], weight});
    ruleSet.insert({tileTypeMap[b], tileTypeMap[a], weight});

    mappedRules[tileTypeMap[a]].insert(tileTypeMap[b]);
    mappedRules[tileTypeMap[b]].insert(tileTypeMap[a]);
}

void Rules::AddTwoWayRule(int a, int b, float weight) {
    ruleSet.insert({a, b, weight});
    ruleSet.insert({b, a, weight});

    mappedRules[a].insert(b);
    mappedRules[b].insert(a);
}

std::set<int> Rules::GetPossibleNeighbourTypes(const std::set<int>& originalTypes) const {
    std::set<int> neighbourAllowedTypes;
    for(auto & type : originalTypes){
        if(mappedRules.contains(type)){
            for (auto & ruleResult : mappedRules.at(type)) {
                neighbourAllowedTypes.insert(ruleResult);
            }
        }
    }
//    std::string message = "Allowed types for neighbour at x:" + std::to_string(neighbourTile.xPos) + " y:" + std::to_string(neighbourTile.yPos) + " ";
//    for (auto & type : neighbourAllowedTypes) {
//        message += std::to_string(type) + " ";
//    }
//    LOG(message);

    return neighbourAllowedTypes;
}

void Rules::PrintRules() {

    for (auto & rule : ruleSet) {
        std::string message = std::format("Rule from {} to {} with weight {}", rule.typeIdFrom, rule.typeIdTo, rule.weight);
        LOG(message);
    }
}

void Rules::CalculateSelfWeights() {
    for (auto & rule : mappedRules) {
        float weight = 0;
        for (auto & type : rule.second) {
            for (auto & ruleSet : ruleSet) {
                if (ruleSet.typeIdFrom == rule.first && ruleSet.typeIdTo == type) {
                    weight += ruleSet.weight;
                }
            }
        }
        selfWeights[rule.first] = weight;
    }
}

void Rules::PrintSelfWeights() {
    for (auto & weight : selfWeights) {
        std::string message = std::format("Self weight for type {}: {}", weight.first, weight.second);
        LOG(message);
    }
}

float Rules::GetSelfWeight(const int &typeId) const {
    return selfWeights.at(typeId);
}


//    std::unordered_map<int, std::list<int>> forbiddenRules;
//    //calculate forbidden rules
//    for (auto & rule:rules) {
//        for (auto & type:tileTypes) {
//            if (std::find(rule.second.begin(), rule.second.end(), type.first) == rule.second.end()) {
//                forbiddenRules[rule.first].push_back(type.first);
//            }
//        }
//    }
//
//    //print forbidden rules
//    for (auto & rule : forbiddenRules) {
//        std::string message = std::format("Forbidden rules for type {}: ", rule.first);
//        for (auto & type : rule.second) {
//            message += std::format("{} ", type);
//        }
//        LOG(message);
//    }