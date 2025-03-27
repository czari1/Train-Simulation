#pragma once
#include <string>
#include <iostream>
#include <limits>
#include <functional>
#include "Train.hpp"
#include "Station.hpp"
#include "Route.hpp"
#include "Management.hpp"

namespace CJ { 

class CLI {
private:
    static void getIntInput(int& input);
    static std::string getStringInput();
    static void getValidIntInput(int min, int max, int& input);
    static void getValidPositiveInt(int& input);
    
    template <typename T>
    static bool displayUsedObjects(const std::string& prompt, 
                                 const std::vector<T>& collection,
                                 std::function<std::string(const T&)> getIdFunc);
    
    static void getNewValidId(int& id);
    static std::string formatStationName(const std::string& name);
    static bool isStationNameTaken(const std::string& name);
    static bool compareStationNames(const std::string& name1, const std::string& name2);
    
    static void displayMainMenu();
    static void handleTrainOperations(DatabaseManager& db);
    static void handleStationOperations(DatabaseManager& db);
    static void handleRouteOperations(DatabaseManager& db);

public:
    static void run();
};

} // namespace CJ