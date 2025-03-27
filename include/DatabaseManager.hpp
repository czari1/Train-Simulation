
#pragma once
#include <vector>
#include <string>
#include <sqlite3.h>
#include <memory>
#include "Train.hpp"
#include "Station.hpp"
#include "Route.hpp"

namespace CJ {
    class DatabaseManager{
    private:
        sqlite3* m_db;
        bool m_isConnected;

        static int callback(void* data, int argc, char** argv, char** azColName); 
        bool executeQuery(const std::string& query);
        bool prepareDatabase();

        std::string generateRouteIdentifier(const std::vector<std::string>& stops) const;

        std::string escapeString(const std::string& str) const;

    public:
        DatabaseManager();
        ~DatabaseManager();

        bool connect(const std::string& dbPath = "train_system.db");
        bool disconnect();
        bool isConnected() const;
        bool displayDatabaseContents();
        void cleanupDatabase();
        
        bool saveTrain(const Train& train);
        bool loadTrains(std::vector<Train>& trains);
        bool deleteTrain(int id);
        bool updateTrain(const Train& train);
        bool getTrainById(int id, Train& train);

        bool saveStation(const Station& station);
        bool loadStations(std::vector<Station>& stations);
        bool updateStation(const Station& station);
        bool deleteStation(const std::string& name);
        bool getStationByName(const std::string& name, Station& station);

        bool saveRoute(const Route& route);
        bool loadRoutes(std::vector<Route>& routes);

        bool assignTrainToRoute(int trainId, const std::vector<std::string>& routeStops);
        bool getTrainsForRoute(const std::vector<std::string>& routeStops, std::vector<int>& trainIds);
        bool getRoutesForTrain(int trainId, std::vector<std::vector<std::string>>& routes);
    };

}
