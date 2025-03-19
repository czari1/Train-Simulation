/**
 * @file CLI.hpp
 * @brief Header file for the Command Line Interface (CLI) class for managing trains, stations, and routes.
 */

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
    static void displayUsedObjects(const std::string& prompt, 
                                 const std::vector<T>& collection,
                                 std::function<std::string(const T&)> getIdFunc) {
        std::cout << prompt << std::endl;
        for (const auto& item : collection) {
            std::cout << formatStationName(getIdFunc(item)) << "\n";
        }
    }
    
    static void getNewValidId(int& id);
    static std::string formatStationName(const std::string& name);
    static bool isStationNameTaken(const std::string& name);
    static bool compareStationNames(const std::string& name1, const std::string& name2);
    
    static void displayMainMenu();
    static void handleTrainOperations();
    static void handleStationOperations();
    static void handleRouteOperations();

public:
    static void run();
};

} // namespace CJ