//
// Created by Felix on 23/02/2024.
//

#ifndef WAVECOLLAPSEFUNCTION_RULES_H
#define WAVECOLLAPSEFUNCTION_RULES_H


#include <unordered_map>
#include <list>
#include <set>

struct Rule {
    int typeIdFrom;
    int typeIdTo;
    float weight;

    // Define a comparison operator for Rule.
    bool operator<(const Rule& other) const {
        // First compare by typeIdFrom, then typeIdTo, then weight.
        if (typeIdFrom != other.typeIdFrom) return typeIdFrom < other.typeIdFrom;
        if (typeIdTo != other.typeIdTo) return typeIdTo < other.typeIdTo;
        return weight < other.weight;
    }
};

class Rules {
public:
    Rules();
    ~Rules();

    void AddTwoWayRule(std::unordered_map<char, int> tileTypeMap, char a, char b, float weight = 1.0f);
    void AddTwoWayRule(int a, int b, float weight = 1.0f);

    std::set<int> GetPossibleNeighbourTypes(const std::set<int>& originalTypes) const;

    void PrintRules();
    void CalculateSelfWeights();
    void PrintSelfWeights();

    float GetSelfWeight(const int &typeId) const;

private:
    std::set<Rule> ruleSet;
    std::unordered_map<int, std::set<int>> mappedRules;
    std::unordered_map<int, float> selfWeights;
};


#endif //WAVECOLLAPSEFUNCTION_RULES_H
