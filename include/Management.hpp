#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Train.hpp"
#include "Station.hpp"
#include "Route.hpp"
#include "DatabaseManager.hpp" 

namespace CJ {

class Management {
private:
    friend class CLI;
    static std::vector<Train> m_trains;
    static std::vector<Station> m_stations;
    static std::vector<Route> m_routes;
    static DatabaseManager m_dbManager;

public:
    // Update function signatures to use shared_ptr
    static void addRoute(int depHour, int depMin, int arrHour, int arrMin,
                         Train& trainName, int duration,
                         const std::vector<std::string>& intermediateStops);
    
    static void displayAllRoutes();
    
    static void addTrain(const std::string& trainName, int speed, int capacity, int id,
                        const std::string& startStation, const std::string& endStation,
                        int wagonCount, std::shared_ptr<Route> depHour = nullptr, 
                        std::shared_ptr<Route> depMin = nullptr,
                        std::shared_ptr<Route> arrHour = nullptr, 
                        std::shared_ptr<Route> arrMin = nullptr);
    
    static bool deleteTrain(int id);
    
    static void displayTrainInfo(int id);
    
    static void addStation(std::shared_ptr<Train> trainName, int platformCount,
                         const std::vector<std::shared_ptr<Route>>& intermediateStops,
                         std::shared_ptr<Train> startStation, std::shared_ptr<Train> endStation,
                         const std::string& name);
    
    static bool removeStation(const std::string& name);
    
    static void displayStationInfo(const std::string& name);
    
    static bool initializeSystem();
};

} // namespace CJ