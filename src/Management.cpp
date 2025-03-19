#include "../include/Management.hpp"
#include <algorithm>
#include <iostream>

namespace CJ {
    std::vector<Train> Management::m_trains;
    std::vector<Station> Management::m_stations;
    std::vector<Route> Management::m_routes;
    DatabaseManager Management::m_dbManager;

    void Management::addRoute(int depHour, int depMin, int arrHour, int arrMin,
                      Train& train, int duration,
                      const std::vector<std::string>& intermediateStops) {
    auto trainPtr = std::make_shared<Train>(train);
    Route newRoute(depHour, depMin, arrHour, arrMin,
                duration, trainPtr, nullptr, nullptr, intermediateStops);
    
    // Save route to database first
    if (m_dbManager.saveRoute(newRoute)) {
        // If database save successful, add to memory
        m_routes.push_back(newRoute);
        // Assign train to route
        m_dbManager.assignTrainToRoute(train.getId(), intermediateStops);
    } else {
        throw std::runtime_error("Failed to save route to database");
    }
}

    void Management::displayAllRoutes() {
        std::vector<Route> routes;
        if (!m_dbManager.loadRoutes(routes)) {
            std::cout << "Failed to load routes from database.\n";
            return;
        }
    
        if (routes.empty()) {
            std::cout << "No routes available.\n";
            return;
        }
    
        std::cout << "\nAll Routes:\n";
        size_t routeNumber = 1;
        for (const auto& route : routes) {
            std::cout << "\nRoute #" << routeNumber++ << ":\n"
                     << "Departure Time: " 
                     << (route.getDepartureTimeHour() < 10 ? "0" : "") << route.getDepartureTimeHour() << ":"
                     << (route.getDepartureTimeMinute() < 10 ? "0" : "") << route.getDepartureTimeMinute() << "\n"
                     << "Arrival Time: " 
                     << (route.getArrivalTimeHour() < 10 ? "0" : "") << route.getArrivalTimeHour() << ":"
                     << (route.getArrivalTimeMinute() < 10 ? "0" : "") << route.getArrivalTimeMinute() << "\n"
                     << "Duration: " << route.getDuration() << " minutes\n"
                     << "Intermediate Stops:\n";
    
            for (const auto& stop : route.getIntermediateStops()) {
                std::cout << "- " << stop << "\n";
            }
        }
    }

    void Management::addTrain(const std::string& trainName, int speed, int capacity, int id,
                    const std::string& startStation, const std::string& endStation,
                    int wagonCount, std::shared_ptr<Route> depHour, 
                    std::shared_ptr<Route> depMin,
                    std::shared_ptr<Route> arrHour, 
                    std::shared_ptr<Route> arrMin) {
    // First ensure stations exist or create them
    try {
        Station startStationObj;  // Create temporary station objects
        Station endStationObj;
        
        if (!m_dbManager.getStationByName(startStation, startStationObj)) {
            addStation(nullptr, 1, {}, nullptr, nullptr, startStation);
        }
        if (!m_dbManager.getStationByName(endStation, endStationObj)) {
            addStation(nullptr, 1, {}, nullptr, nullptr, endStation);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create required stations: " + std::string(e.what()));
    }

    // Create and save train
    Train newTrain(trainName, speed, capacity, id, startStation, endStation,
                  wagonCount, depHour, depMin, arrHour, arrMin);
    
        if (m_dbManager.saveTrain(newTrain)) {
            m_trains.push_back(newTrain);
        } else {
            throw std::runtime_error("Failed to save train to database");
        }
    }

    bool Management::deleteTrain(int id) {
        // Delete from database first
        if (m_dbManager.deleteTrain(id)) {
            // If database delete successful, remove from memory
            auto it = std::find_if(m_trains.begin(), m_trains.end(),
                                [id](const Train& train) { return train.getId() == id; });
            if (it != m_trains.end()) {
                m_trains.erase(it);
                return true;
            }
        }
        return false;
    }

    void Management::displayTrainInfo(int id) {
        Train train;
        if (!m_dbManager.getTrainById(id, train)) {
            std::cout << "Train with ID " << id << " not found in database.\n";
            return;
        }
    
        std::cout << "Train Information:\n"
                  << "Name: " << train.getTrainName() << "\n"
                  << "ID: " << train.getId() << "\n"
                  << "Speed: " << train.getSpeed() << " km/h\n"
                  << "Capacity: " << train.getCapacity() << " passengers\n"
                  << "Start Station: " << train.getStartStation() << "\n"
                  << "End Station: " << train.getEndStation() << "\n"
                  << "Wagon Count: " << train.getWagonCount() << "\n";
    
        // Display associated routes
        std::vector<std::vector<std::string>> routes;
        if (m_dbManager.getRoutesForTrain(id, routes)) {
            if (!routes.empty()) {
                std::cout << "Associated Routes:\n";
                for (const auto& route : routes) {
                    std::cout << "- Route: ";
                    for (size_t i = 0; i < route.size(); ++i) {
                        std::cout << route[i];
                        if (i < route.size() - 1) std::cout << " -> ";
                    }
                    std::cout << "\n";
                }
            }
        }
    }

    void Management::addStation(std::shared_ptr<Train> trainName, int platformCount,
                        const std::vector<std::shared_ptr<Route>>& intermediateStops,
                        std::shared_ptr<Train> startStation, std::shared_ptr<Train> endStation,
                        const std::string& name) {
    // Validate platform count
    if (platformCount <= 0) {
        platformCount = 1; // Set default platform count if invalid
    }

    // Create new station
    try {
        Station newStation(trainName, platformCount, intermediateStops,
                         startStation, endStation, name);
        
        // Save to database first
        if (m_dbManager.saveStation(newStation)) {
            // If database save successful, add to memory
            m_stations.push_back(newStation);
        } else {
            throw std::runtime_error("Failed to save station to database");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create station: " + std::string(e.what()));
    }
}

    bool Management::removeStation(const std::string& name) {
        // Delete from database first
        if (m_dbManager.deleteStation(name)) {
            // If database delete successful, remove from memory
            auto it = std::find_if(m_stations.begin(), m_stations.end(),
                                [&name](const Station& station) {
                                    return station.getName() == name;
                                });
            if (it != m_stations.end()) {
                m_stations.erase(it);
                return true;
            }
        }
        return false;
    }

    void Management::displayStationInfo(const std::string& name) {
        Station station;
        if (!m_dbManager.getStationByName(name, station)) {
            std::cout << "Station with name " << name << " not found in database.\n";
            return;
        }
    
        std::cout << "Station Information:\n"
                  << "Name: " << station.getName() << "\n"
                  << "Platform Count: " << station.getPlatformCount() << "\n";
    
        // Display routes passing through this station
        std::vector<Route> routes;
        if (m_dbManager.loadRoutes(routes)) {
            std::cout << "Routes passing through this station:\n";
            bool foundRoute = false;
            for (const auto& route : routes) {
                const auto& stops = route.getIntermediateStops();
                if (std::find(stops.begin(), stops.end(), name) != stops.end()) {
                    foundRoute = true;
                    std::cout << "- Departure: "
                             << (route.getDepartureTimeHour() < 10 ? "0" : "") 
                             << route.getDepartureTimeHour() << ":"
                             << (route.getDepartureTimeMinute() < 10 ? "0" : "") 
                             << route.getDepartureTimeMinute() << "\n";
                }
            }
            if (!foundRoute) {
                std::cout << "No routes currently pass through this station.\n";
            }
        }
    }

    bool Management::initializeSystem() {
        // Connect to database
        if (!m_dbManager.connect()) {
            std::cerr << "Failed to connect to database!" << std::endl;
            return false;
        }

        // Load existing data from database
        m_dbManager.loadTrains(m_trains);
        m_dbManager.loadStations(m_stations);

        // Initialize predefined routes if database is empty
        if (m_trains.empty() && m_stations.empty()) {
            // Create predefined routes
            Route route1(8, 30, 11, 45, 195, nullptr, nullptr, nullptr,
                {"Warsaw Central", "Lodz Widzew", "Czestochowa", "Krakow Main"});
            Route route2(9, 15, 13, 30, 255, nullptr, nullptr, nullptr,
                {"Gdansk Central", "Bydgoszcz", "Poznan", "Wroclaw Main"});
            Route route3(7, 0, 9, 30, 150, nullptr, nullptr, nullptr, 
                {"Warsaw Central", "Radom", "Kielce", "Krakow Main"});
            
            m_routes.push_back(route1);
            m_routes.push_back(route2);
            m_routes.push_back(route3);

            // Initialize predefined trains
            addTrain("Express_101", 160, 400, 1001, "Warsaw", "Krakow", 8);
            addTrain("InterCity_202", 140, 350, 1002, "Gdansk", "Wroclaw", 6);
            addTrain("Regional_303", 120, 250, 1003, "Poznan", "Lodz", 4);

            // Initialize predefined stations
            addStation(nullptr, 5, {}, nullptr, nullptr, "Warsaw Central");
            addStation(nullptr, 4, {}, nullptr, nullptr, "Krakow Main");
            addStation(nullptr, 3, {}, nullptr, nullptr, "Gdansk Central");
        }
        return true;
    }
}