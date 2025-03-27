#include "../include/Management.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>

namespace CJ {
    Management* Management::instance = nullptr;
    std::vector<Train> Management::m_trains;
    std::vector<Station> Management::m_stations;
    std::vector<Route> Management::m_routes;
    DatabaseManager Management::m_dbManager;

    void Management::addRoute(int depHour, int depMin, int arrHour, int arrMin,
                      Train& train, int duration,
                      const std::vector<std::string>& intermediateStops) {
    // Calculate total minutes for departure and arrival
    int depTime = depHour * 60 + depMin;
    int arrTime = arrHour * 60 + depMin;
    
    // Calculate duration automatically
    int calculatedDuration = arrTime - depTime;
    
    if (calculatedDuration <= 0) {
        throw std::runtime_error("Arrival time must be after departure time");
    }

    auto trainPtr = std::make_shared<Train>(train);
    Route newRoute(depHour, depMin, arrHour, arrMin,
                calculatedDuration, trainPtr, nullptr, nullptr, intermediateStops);

    if (m_dbManager.saveRoute(newRoute)) {
        m_routes.push_back(newRoute);
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

    bool Management::addTrain(const std::string& trainName, int speed, int capacity, 
                        int id, int wagonCount) {
    try {
        Train newTrain(trainName, speed, capacity, id, wagonCount);
        if (m_dbManager.saveTrain(newTrain)) {
            m_trains.push_back(newTrain);
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error adding train: " << e.what() << std::endl;
    }
    return false;
}

    bool Management::deleteTrain(int id) {

        if (m_dbManager.deleteTrain(id)) {

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
            std::cout << "Train with ID " << id << " not found.\n";
            return;
        }
    
        std::cout << "Train Information:\n"
                  << "Name: " << train.getTrainName() << "\n"
                  << "ID: " << train.getId() << "\n"
                  << "Speed: " << train.getSpeed() << " km/h\n"
                  << "Capacity: " << train.getCapacity() << " passengers\n"
                  << "Wagon Count: " << train.getWagonCount() << "\n";
    
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
    try {
        // Format station name
        std::string formattedName = formatStationName(name);
        
        // Create new station with validated platform count
        Station newStation(trainName, std::max(1, platformCount), intermediateStops,
                         startStation, endStation, formattedName);
        
        if (m_dbManager.saveStation(newStation)) {
            m_stations.push_back(newStation);
        } else {
            throw std::runtime_error("Failed to save station to database");
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to create station: " + std::string(e.what()));
    }
}

    bool Management::removeStation(const std::string& name) {

        if (m_dbManager.deleteStation(name)) {

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
        if (name.empty()) {
            std::cout << "Error: Station name cannot be empty\n";
            return;
        }

        auto it = std::find_if(m_stations.begin(), m_stations.end(),
                              [&name](const Station& s) {
                                  return compareStationNames(s.getName(), name);
                              });

        if (it == m_stations.end()) {
            std::cout << "Station '" << name << "' not found.\n";
            return;
        }

        const Station& station = *it;
        std::cout << "\nStation Information:\n"
                  << "Name: " << station.getName() << "\n"
                  << "Platform Count: " << station.getPlatformCount() << "\n";
    }

    bool Management::initializeSystem() {
        try {
            if (!m_dbManager.connect()) {
                std::cerr << "Failed to connect to database!" << std::endl;
                m_dbManager.cleanupDatabase();
                return false;
            }

            m_dbManager.loadTrains(m_trains);
            m_dbManager.loadStations(m_stations);

            if (m_trains.empty() && m_stations.empty()) {
                try {
                    // Add stations first
                    addStation(nullptr, 5, {}, nullptr, nullptr, "Warsaw Central");
                    addStation(nullptr, 4, {}, nullptr, nullptr, "Krakow Main");
                    addStation(nullptr, 3, {}, nullptr, nullptr, "Gdansk Central");

                    // Add trains
                    Train express("Express_101", 160, 400, 1001, 8);
                    Train intercity("InterCity_202", 140, 350, 1002, 6);
                    Train regional("Regional_303", 120, 250, 1003, 4);
                    
                    addTrain("Express_101", 160, 400, 1001, 8);
                    addTrain("InterCity_202", 140, 350, 1002, 6);
                    addTrain("Regional_303", 120, 250, 1003, 4);

                    // Create and save routes with calculated durations
                    std::vector<std::string> route1Stops = {"Warsaw Central", "Lodz Widzew", "Czestochowa", "Krakow Main"};
                    std::vector<std::string> route2Stops = {"Gdansk Central", "Bydgoszcz", "Poznan", "Wroclaw Main"};
                    std::vector<std::string> route3Stops = {"Warsaw Central", "Radom", "Kielce", "Krakow Main"};

                    try {
                        // Duration will be calculated automatically
                        addRoute(8, 30, 11, 45, express, 0, route1Stops);
                        addRoute(9, 15, 13, 30, intercity, 0, route2Stops);
                        addRoute(7, 0, 9, 30, regional, 0, route3Stops);
                    } catch (const std::runtime_error& e) {
                        std::cerr << "Failed to initialize default routes: " << e.what() << std::endl;
                        return false;
                    }
                } catch (const std::runtime_error& e) {
                    std::cerr << "Failed to initialize default data: " << e.what() << std::endl;
                    m_dbManager.cleanupDatabase();
                    return false;
                }
            }
            return true;
        } catch (const std::exception& e) {
            std::cerr << "System initialization failed: " << e.what() << std::endl;
            m_dbManager.cleanupDatabase();
            return false;
        }
    }

    std::string Management::formatStationName(const std::string& name) {
        if (name.empty()) return name;
        
        std::string formattedName;
        std::istringstream iss(name);
        std::string word;
        
        while (iss >> word) {
            if (!formattedName.empty()) {
                formattedName += " ";
            }
            
            // Convert word to lowercase
            std::transform(word.begin(), word.end(), word.begin(),
                          [](unsigned char c) { return std::tolower(c); });
            
            // Capitalize first letter
            if (!word.empty()) {
                word[0] = std::toupper(word[0]);
            }
            
            formattedName += word;
        }
        
        return formattedName;
    }

    bool Management::compareStationNames(const std::string& name1, const std::string& name2) {
        std::string formattedName1 = formatStationName(name1);
        std::string formattedName2 = formatStationName(name2);
        return formattedName1 == formattedName2;
    }
}