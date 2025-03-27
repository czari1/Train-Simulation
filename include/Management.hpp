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
    static Management* instance;
    DatabaseManager m_database;
    Management() = default;  // Private constructor

public:
    static Management& getInstance() {
        if (instance == nullptr) {
            instance = new Management();
        }
        return *instance;
    }

    DatabaseManager& getDatabase() { return m_database; }

    static void addRoute(int depHour, int depMin, int arrHour, int arrMin,
                         Train& trainName, int duration,
                         const std::vector<std::string>& intermediateStops);
    static void displayAllRoutes();
    

    static bool addTrain(const std::string& trainName, int speed, int capacity, int id,
                        int wagonCount);
    static bool deleteTrain(int id);
    static void displayTrainInfo(int id);
    

    static void addStation(std::shared_ptr<Train> trainName, int platformCount,
                         const std::vector<std::shared_ptr<Route>>& intermediateStops,
                         std::shared_ptr<Train> startStation, std::shared_ptr<Train> endStation,
                         const std::string& name);
    static bool removeStation(const std::string& name);
    static void displayStationInfo(const std::string& name);
    

    static bool initializeSystem();
    static std::string formatStationName(const std::string& name);
    static bool compareStationNames(const std::string& name1, const std::string& name2);


    static const std::vector<Train>& getTrains() { return m_trains; }
    static const std::vector<Station>& getStations() { return m_stations; }
    static const std::vector<Route>& getRoutes() { return m_routes; }
};

} // namespace CJ