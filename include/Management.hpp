#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "Train.hpp"
#include "Station.hpp"
#include "Route.hpp"

namespace CJ {

class Management {
private:
    friend class CLI;
    inline static std::vector<Train> m_trains;
    inline static std::vector<Station> m_stations;
    inline static std::vector<Route> m_routes;

public:

    static inline void displayAllRoutes() {
        if (m_routes.empty()) {
            std::cout << "No routes available.\n";
            return;
        }

        std::cout << "\nAll Routes:\n";
        for (size_t i = 0; i < m_routes.size(); ++i) {
            const auto& route = m_routes[i];
            std::cout << "\nRoute #" << (i + 1) << ":\n"
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

    static inline void addTrain(const std::string& trainName, int speed, int capacity, int id,
                        const std::string& startStation, const std::string& endStation,
                        int wagonCount, Route* depHour, Route* depMin,
                        Route* arrHour, Route* arrMin) {
        Train newTrain(trainName, speed, capacity, id, startStation, endStation,
                      wagonCount, depHour, depMin, arrHour, arrMin);
        m_trains.push_back(newTrain);
    }

    static inline bool deleteTrain(int id) {
        auto it = std::find_if(m_trains.begin(), m_trains.end(),
                              [id](const Train& train) { return train.getId() == id; });
        if (it != m_trains.end()) {
            m_trains.erase(it);
            return true;
        }
        return false;
    }

    static inline void displayTrainInfo(int id) {
        auto it = std::find_if(m_trains.begin(), m_trains.end(),
                              [id](const Train& train) { return train.getId() == id; });
        if (it != m_trains.end()) {
            std::cout << "Train Information:\n"
                     << "Name: " << it->getTrainName() << "\n"
                     << "ID: " << it->getId() << "\n"
                     << "Speed: " << it->getSpeed() << " km/h\n"
                     << "Capacity: " << it->getCapacity() << " passengers\n"
                     << "Start Station: " << it->getStartStation() << "\n"
                     << "End Station: " << it->getEndStation() << "\n"
                     << "Wagon Count: " << it->getWagonCount() << "\n";
        } else {
            std::cout << "Train with ID " << id << " not found.\n";
        }
    }

    static inline void addStation(Train* trainName, int platformCount,
                         const std::vector<Route*>& intermediateStops,
                         Train* startStation, Train* endStation,
                         const std::string& name) {
        Station newStation(trainName, platformCount, intermediateStops,
                         startStation, endStation, name);
        m_stations.push_back(newStation);
    }

    static inline bool removeStation(const std::string& name) {
        auto it = std::find_if(m_stations.begin(), m_stations.end(),
                              [&name](const Station& station) {
                                  return station.getName() == name;
                              });
        if (it != m_stations.end()) {
            m_stations.erase(it);
            return true;
        }
        return false;
    }

    static inline void displayStationInfo(const std::string& name) {
        auto it = std::find_if(m_stations.begin(), m_stations.end(),
                              [&name](const Station& station) {
                                  return station.getName() == name;
                              });
        if (it != m_stations.end()) {
            std::cout << "Station Information:\n"
                     << "Name: " << it->getName() << "\n"
                     << "Platform Count: " << it->getPlatformCount() << "\n"
                     << "Intermediate Stops Count: "
                     << it->getIntermediateStops().size() << "\n";
        } else {
            std::cout << "Station with name " << name << " not found.\n";
        }
    }

    

    static inline void initializeSystem() {
        // Initialize predefined routes
        Route route1(8, 30, 11, 45, nullptr, nullptr, nullptr, 195,
            {"Warsaw Central", "Lodz Widzew", "Czestochowa", "Krakow Main"});
        Route route2(9, 15, 13, 30, nullptr, nullptr, nullptr, 255,
            {"Gdansk Central", "Bydgoszcz", "Poznan", "Wroclaw Main"});
        Route route3(7, 0, 9, 30, nullptr, nullptr, nullptr, 150,
            {"Warsaw Central", "Radom", "Kielce", "Krakow Main"});
        
        m_routes.push_back(route1);
        m_routes.push_back(route2);
        m_routes.push_back(route3);

        // Initialize predefined trains
        addTrain("Express_101", 160, 400, 1001, "Warsaw", "Krakow", 8, nullptr, nullptr, nullptr, nullptr);
        addTrain("InterCity_202", 140, 350, 1002, "Gdansk", "Wroclaw", 6, nullptr, nullptr, nullptr, nullptr);
        addTrain("Regional_303", 120, 250, 1003, "Poznan", "Lodz", 4, nullptr, nullptr, nullptr, nullptr);

        // Initialize predefined stations
        addStation(nullptr, 5, {}, nullptr, nullptr, "Warsaw Central");
        addStation(nullptr, 4, {}, nullptr, nullptr, "Krakow Main");
        addStation(nullptr, 3, {}, nullptr, nullptr, "Gdansk Central");
    }
};

} // namespace CJ