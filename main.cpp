//Inspired by https://robertheaton.com/2018/12/17/wavefunction-collapse-algorithm/

#include "Image.h"
#include "Rules.h"
#include "WfcCore.h"


#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <format>
#include <set>
#include <chrono>


#define LOG(x) ;//std::cout << x << std::endl
#define DISPLAY(x) std::cout << x << std::endl


int executeWaveFunctionCollapse();

const std::string RED = "\033[31m";      // Red color
const std::string GREEN = "\033[32m";    // Green color
const std::string YELLOW = "\033[33m";   // Yellow color
const std::string BLUE = "\033[34m";     // Blue color
const std::string GRAY = "\033[90m";     // Blue color
const std::string RESET = "\033[0m";     // Reset to default color

int main() {
    LOG("Hello World!");
    executeWaveFunctionCollapse();
}

int executeWaveFunctionCollapse(){
    std::unordered_map<int,char> tileTypes;
    tileTypes[0] = 'L';
    tileTypes[1] = 'C';
    tileTypes[2] = 'W';
    tileTypes[3] = 'M';

    std::unordered_map<char, int> tileTypesReversed;
    for (auto & type : tileTypes) {
        tileTypesReversed[type.second] = type.first;
    }

    std::set<int> allPossibleTypes;
    for (auto & type : tileTypes) {
        allPossibleTypes.insert(type.first);
    }


    Rules rulesObject;
    rulesObject.AddTwoWayRule(tileTypesReversed, 'L', 'L', 1.0f);
    rulesObject.AddTwoWayRule(tileTypesReversed, 'L', 'C', 1.0f);
    rulesObject.AddTwoWayRule(tileTypesReversed, 'L', 'M', 1.0f);

    rulesObject.AddTwoWayRule(tileTypesReversed, 'C', 'C', 0.5f);
    rulesObject.AddTwoWayRule(tileTypesReversed, 'C', 'W', 1.0f);

    rulesObject.AddTwoWayRule(tileTypesReversed, 'W', 'W', 1.0f);

    rulesObject.AddTwoWayRule(tileTypesReversed, 'M', 'M', 1.0f);

    rulesObject.PrintRules();
    rulesObject.CalculateSelfWeights();
    rulesObject.PrintSelfWeights();

    const int height = 20;
    const int width = 40;

    WfcCore wfcCore(width, height, allPossibleTypes);
    wfcCore.SetDebug(false);
    //WfcCore* wfcCorePointer = &wfcCore;
    wfcCore.PresetTile(0, 0, 2);
    wfcCore.PresetTile(height-1, width -1, 3);
    wfcCore.Run(rulesObject);
    if(!wfcCore.IsLastRunValid()){
        LOG("Last run was not valid");
        return 1;
    }
    std::vector<std::vector<Tile*>> scene = wfcCore.GetResults();


    std::unordered_map<int,Color> ColorMap;
    ColorMap[0] = Color(141, 182, 0); //Apple green
    ColorMap[1] = Color(243, 229, 171); //Vanilla
    ColorMap[2] = Color(0, 147, 175); //Blue (Munsell)
    ColorMap[3] = Color(112, 128, 144); //Slate gray

    //Color(0, 255, 191);//Aquamarine

    Image image(width, height);
    for(int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int type = *scene[y][x]->possibleTypes.begin();
            int flippedY = height - y - 1;
            image.SetColor(ColorMap[type], x, flippedY);
        }
    }
    image.Export("result.bmp");
    std::string timestamp = std::format("result_{}.bmp", std::chrono::system_clock::now().time_since_epoch().count());
    image.Export(timestamp.c_str());

    //print scene
    std::string sceneString = "";
    for (auto & i : scene) {
        for (auto & j : i) {
            sceneString+= "(";
            for (auto & k : j->possibleTypes) {
                sceneString += tileTypes[k];
            }
            sceneString+= ")";
        }
        sceneString += "\n";
    }
    LOG(sceneString);

    //print scene pretty with colors
    sceneString = "";
    for (auto & i : scene) {
        for (auto & j : i) {
            //sceneString+= "(";
            for (auto & k : j->possibleTypes) {
                if(k == 0){
                    sceneString += GREEN;
                }
                else if(k == 1){
                    sceneString += YELLOW;
                }
                else if(k == 2){
                    sceneString += BLUE;
                }
                else if(k == 3){
                    sceneString += GRAY;
                }
                sceneString += tileTypes[k];
                sceneString += RESET;
            }
            //sceneString+= ")";
        }
        sceneString += "\n";
    }

    DISPLAY(sceneString);
    return 0;
}
