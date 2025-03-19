#include "../include/DatabaseManager.hpp"
#include <iostream>
#include <sstream>
#include <filesystem>

namespace CJ {

DatabaseManager::DatabaseManager() : m_db(nullptr), m_isConnected(false) {
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect(const std::string& dbPath) {
    if (m_isConnected) {
        return true;
    }

    // Create database directory if it doesn't exist
    std::filesystem::path dbDir = std::filesystem::absolute("./database");
    if (!std::filesystem::exists(dbDir)) {
        std::filesystem::create_directory(dbDir);
    }

    // Use database directory for the db file
    std::string fullPath = (dbDir / "train_system.db").string();

    int rc = sqlite3_open(fullPath.c_str(), &m_db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }

    m_isConnected = true;
    std::cout << "Database connected successfully to: " << fullPath << std::endl;
    return prepareDatabase();
}

bool DatabaseManager::disconnect() {
    if (!m_isConnected) {
        return true;
    }

    int rc = sqlite3_close(m_db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error closing database: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    m_db = nullptr;
    m_isConnected = false;
    return true;
}

bool DatabaseManager::isConnected() const {
    return m_isConnected;
}

int DatabaseManager::callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* result = static_cast<std::vector<std::string>*>(data);
    
    for (int i = 0; i < argc; i++) {
        result->push_back(argv[i] ? argv[i] : "NULL");
    }
    
    return 0;
}

bool DatabaseManager::executeQuery(const std::string& query) {
    if (!m_isConnected) {
        std::cerr << "Database not connected" << std::endl;
        return false;
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool DatabaseManager::prepareDatabase() {
    // Create tables if they don't exist
    std::string createTrainsTable = 
        "CREATE TABLE IF NOT EXISTS trains ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "speed INTEGER NOT NULL,"
        "capacity INTEGER NOT NULL,"
        "wagon_count INTEGER NOT NULL,"
        "start_station TEXT NOT NULL,"
        "end_station TEXT NOT NULL"
        ");";
    
    std::string createStationsTable = 
        "CREATE TABLE IF NOT EXISTS stations ("
        "name TEXT PRIMARY KEY,"
        "platform_count INTEGER NOT NULL"
        ");";
    
    std::string createRoutesTable = 
        "CREATE TABLE IF NOT EXISTS routes ("
        "identifier TEXT PRIMARY KEY,"
        "dep_hour INTEGER NOT NULL,"
        "dep_minute INTEGER NOT NULL,"
        "arr_hour INTEGER NOT NULL,"
        "arr_minute INTEGER NOT NULL,"
        "duration INTEGER NOT NULL"
        ");";
    
    std::string createRouteStopsTable = 
        "CREATE TABLE IF NOT EXISTS route_stops ("
        "route_id TEXT,"
        "stop_name TEXT,"
        "stop_order INTEGER,"
        "PRIMARY KEY (route_id, stop_order),"
        "FOREIGN KEY (route_id) REFERENCES routes(identifier),"
        "FOREIGN KEY (stop_name) REFERENCES stations(name)"
        ");";
    
    std::string createTrainRoutesTable = 
        "CREATE TABLE IF NOT EXISTS train_routes ("
        "train_id INTEGER,"
        "route_id TEXT,"
        "PRIMARY KEY (train_id, route_id),"
        "FOREIGN KEY (train_id) REFERENCES trains(id),"
        "FOREIGN KEY (route_id) REFERENCES routes(identifier)"
        ");";

    std::string createTrainStationTrigger = 
        "CREATE TRIGGER IF NOT EXISTS check_train_stations "
        "BEFORE INSERT ON trains "
        "BEGIN "
        "  SELECT CASE "
        "    WHEN NEW.start_station NOT IN (SELECT name FROM stations) "
        "    THEN RAISE(ABORT, 'Start station does not exist') "
        "    WHEN NEW.end_station NOT IN (SELECT name FROM stations) "
        "    THEN RAISE(ABORT, 'End station does not exist') "
        "  END; "
        "END;";

    bool success = executeQuery(createStationsTable) &&
                  executeQuery(createTrainsTable) &&
                  executeQuery(createRoutesTable) &&
                  executeQuery(createRouteStopsTable) &&
                  executeQuery(createTrainRoutesTable) &&
                  executeQuery(createTrainStationTrigger);


    if (success) {
        std::cout << "Database tables created successfully!" << std::endl; // Debug message
    } else {
        std::cerr << "Failed to create database tables!" << std::endl;
    }

    return success;
}

bool DatabaseManager::displayDatabaseContents() {
    if (!m_isConnected) {
        std::cerr << "Not connected to database" << std::endl;
        return false;
    }

    const char* queries[] = {
        "SELECT * FROM trains;",
        "SELECT * FROM stations;",
        "SELECT * FROM routes;"
    };

    for (const auto& query : queries) {
        std::cout << "\nExecuting: " << query << std::endl;
        char* errMsg = nullptr;
        int rc = sqlite3_exec(m_db, query, 
            [](void*, int argc, char** argv, char** colNames) {
                for (int i = 0; i < argc; i++) {
                    std::cout << colNames[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\t";
                }
                std::cout << std::endl;
                return 0;
            }, 
            nullptr, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
    }
    return true;
}

std::string DatabaseManager::generateRouteIdentifier(const std::vector<std::string>& stops) const {
    if (stops.empty()) {
        return "";
    }
    
    std::stringstream ss;
    ss << stops.front() << "_to_" << stops.back();
    
    return ss.str();
}

bool DatabaseManager::saveTrain(const Train& train) {
    if (!m_isConnected) return false;
    
    // First ensure stations exist
    std::stringstream checkQuery;
    checkQuery << "SELECT COUNT(*) FROM stations WHERE name IN ('"
               << train.getStartStation() << "', '"
               << train.getEndStation() << "');";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, checkQuery.str().c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count < 2) {
            sqlite3_finalize(stmt);
            std::cerr << "Start or end station does not exist\n";
            return false;
        }
    }
    sqlite3_finalize(stmt);
    
    // If stations exist, save train
    std::stringstream query;
    query << "INSERT OR REPLACE INTO trains "
          << "(id, name, speed, capacity, wagon_count, start_station, end_station) VALUES ("
          << train.getId() << ", "
          << "'" << train.getTrainName() << "', "
          << train.getSpeed() << ", "
          << train.getCapacity() << ", "
          << train.getWagonCount() << ", "
          << "'" << train.getStartStation() << "', "
          << "'" << train.getEndStation() << "');";
    
    return executeQuery(query.str());
}

bool DatabaseManager::loadTrains(std::vector<Train>& trains) {
    if (!m_isConnected) {
        return false;
    }
    
    // Clear existing trains
    trains.clear();
    
    // Prepare query
    const char* query = "SELECT id, name, speed, capacity, start_station, end_station, wagon_count FROM trains;";
    
    // Execute query
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    // Process results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int speed = sqlite3_column_int(stmt, 2);
        int capacity = sqlite3_column_int(stmt, 3);
        std::string startStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string endStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int wagonCount = sqlite3_column_int(stmt, 6);
        
        trains.emplace_back(name, speed, capacity, id, startStation, endStation, wagonCount);
    }
    
    sqlite3_finalize(stmt);
    return rc != SQLITE_ERROR;
}

bool DatabaseManager::deleteTrain(int id) {
    if (!m_isConnected) {
        return false;
    }
    
    std::stringstream query;
    query << "DELETE FROM trains WHERE id = " << id << ";";
    
    return executeQuery(query.str());
}

bool DatabaseManager::updateTrain(const Train& train) {
    return saveTrain(train); // saveTrain already handles updates
}

bool DatabaseManager::getTrainById(int id, Train& train) {
    if (!m_isConnected) {
        return false;
    }
    
    std::stringstream query;
    query << "SELECT id, name, speed, capacity, start_station, end_station, wagon_count "
          << "FROM trains WHERE id = " << id << ";";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    bool found = false;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int speed = std::max(1, sqlite3_column_int(stmt, 2));  // Ensure positive value
        int capacity = std::max(1, sqlite3_column_int(stmt, 3));  // Ensure positive value
        std::string startStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        std::string endStation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        int wagonCount = std::max(1, sqlite3_column_int(stmt, 6));  // Ensure positive value
        
        try {
            train = Train(name, speed, capacity, id, startStation, endStation, wagonCount);
            found = true;
        } catch (const std::exception& e) {
            std::cerr << "Error creating train object: " << e.what() << std::endl;
            found = false;
        }
    }
    
    sqlite3_finalize(stmt);
    return found;
}

bool DatabaseManager::saveStation(const Station& station) {
    if (!m_isConnected) return false;

    // First check if station exists (case insensitive)
    std::stringstream checkQuery;
    checkQuery << "SELECT name FROM stations WHERE name COLLATE NOCASE = '" 
               << station.getName() << "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, checkQuery.str().c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        std::cerr << "Station '" << station.getName() << "' already exists\n";
        return false;
    }
    sqlite3_finalize(stmt);

    // If not exists, insert new station
    std::stringstream query;
    query << "INSERT INTO stations (name, platform_count) VALUES ("
          << "'" << station.getName() << "', "
          << station.getPlatformCount() << ");";
    
    return executeQuery(query.str());
}

bool DatabaseManager::loadStations(std::vector<Station>& stations) {
    if (!m_isConnected) {
        return false;
    }
    
    // Clear existing stations
    stations.clear();
    
    // Prepare query
    const char* query = "SELECT name, platform_count FROM stations;";
    
    // Execute query
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    // Process results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int platformCount = sqlite3_column_int(stmt, 1);
        
        stations.emplace_back(nullptr, platformCount, std::vector<std::shared_ptr<Route>>{}, nullptr, nullptr, name);
    }
    
    sqlite3_finalize(stmt);
    return rc != SQLITE_ERROR;
}

bool DatabaseManager::updateStation(const Station& station) {
    return saveStation(station); // saveStation already handles updates
}

bool DatabaseManager::deleteStation(const std::string& name) {
    if (!m_isConnected) {
        return false;
    }
    
    std::stringstream query;
    query << "DELETE FROM stations WHERE name = '" << name << "';";
    
    return executeQuery(query.str());
}

bool DatabaseManager::getStationByName(const std::string& name, Station& station) {
    if (!m_isConnected) {
        return false;
    }
    
    std::stringstream query;
    query << "SELECT name, platform_count FROM stations WHERE name = '" << name << "';";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    bool found = false;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int platformCount = sqlite3_column_int(stmt, 1);
        
        station = Station(nullptr, platformCount, std::vector<std::shared_ptr<Route>>{}, nullptr, nullptr, name);
        found = true;
    }
    
    sqlite3_finalize(stmt);
    return found;
}

bool DatabaseManager::loadRoutes(std::vector<Route>& routes) {
    if (!m_isConnected) {
        return false;
    }
    
    routes.clear();
    
    // First get all routes
    const char* query = 
        "SELECT identifier, dep_hour, dep_minute, arr_hour, arr_minute, duration "
        "FROM routes;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string identifier = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int depHour = sqlite3_column_int(stmt, 1);
        int depMin = sqlite3_column_int(stmt, 2);
        int arrHour = sqlite3_column_int(stmt, 3);
        int arrMin = sqlite3_column_int(stmt, 4);
        int duration = sqlite3_column_int(stmt, 5);
        
        // Get stops for this route
        std::vector<std::string> stops;
        std::stringstream stopsQuery;
        stopsQuery << "SELECT stop_name FROM route_stops WHERE route_id = '" 
                  << identifier << "' ORDER BY stop_order;";
        
        sqlite3_stmt* stopsStmt;
        int stopsRc = sqlite3_prepare_v2(m_db, stopsQuery.str().c_str(), -1, &stopsStmt, nullptr);
        if (stopsRc == SQLITE_OK) {
            while ((stopsRc = sqlite3_step(stopsStmt)) == SQLITE_ROW) {
                std::string stopName = reinterpret_cast<const char*>(sqlite3_column_text(stopsStmt, 0));
                stops.push_back(stopName);
            }
            sqlite3_finalize(stopsStmt);
        }
        
        // Create route object
        routes.emplace_back(depHour, depMin, arrHour, arrMin, duration,
                          nullptr, nullptr, nullptr, stops);
    }
    
    sqlite3_finalize(stmt);
    return rc != SQLITE_ERROR;
}

bool DatabaseManager::saveRoute(const Route& route) {
    if (!m_isConnected) {
        return false;
    }
    
    std::string identifier = generateRouteIdentifier(route.getIntermediateStops());
    
    // Save route details
    std::stringstream query;
    query << "INSERT OR REPLACE INTO routes "
          << "(identifier, dep_hour, dep_minute, arr_hour, arr_minute, duration) VALUES ("
          << "'" << identifier << "', "
          << route.getDepartureTimeHour() << ", "
          << route.getDepartureTimeMinute() << ", "
          << route.getArrivalTimeHour() << ", "
          << route.getArrivalTimeMinute() << ", "
          << route.getDuration() << ");";
    
    if (!executeQuery(query.str())) {
        return false;
    }
    
    // Save route stops
    const auto& stops = route.getIntermediateStops();
    for (size_t i = 0; i < stops.size(); ++i) {
        std::stringstream stopQuery;
        stopQuery << "INSERT OR REPLACE INTO route_stops "
                 << "(route_id, stop_name, stop_order) VALUES ("
                 << "'" << identifier << "', "
                 << "'" << stops[i] << "', "
                 << i << ");";
        
        if (!executeQuery(stopQuery.str())) {
            return false;
        }
    }
    
    return true;
}

bool DatabaseManager::assignTrainToRoute(int trainId, const std::vector<std::string>& routeStops) {
    if (!m_isConnected || routeStops.empty()) {
        return false;
    }
    
    std::string routeId = generateRouteIdentifier(routeStops);
    
    std::stringstream query;
    query << "INSERT OR REPLACE INTO train_routes (train_id, route_id) VALUES ("
          << trainId << ", '" << routeId << "');";
    
    return executeQuery(query.str());
}

bool DatabaseManager::getTrainsForRoute(const std::vector<std::string>& routeStops, std::vector<int>& trainIds) {
    if (!m_isConnected || routeStops.empty()) {
        return false;
    }
    
    trainIds.clear();
    
    std::string routeId = generateRouteIdentifier(routeStops);
    
    std::stringstream query;
    query << "SELECT train_id FROM train_routes WHERE route_id = '" << routeId << "';";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query.str().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int trainId = sqlite3_column_int(stmt, 0);
        trainIds.push_back(trainId);
    }
    
    sqlite3_finalize(stmt);
    return rc != SQLITE_ERROR;
}

bool DatabaseManager::getRoutesForTrain(int trainId, std::vector<std::vector<std::string>>& routes) {
    if (!m_isConnected) {
        return false;
    }
    
    routes.clear();
    
    // First, get all route_ids for this train
    std::stringstream routeQuery;
    routeQuery << "SELECT route_id FROM train_routes WHERE train_id = " << trainId << ";";
    
    sqlite3_stmt* routeStmt;
    int rc = sqlite3_prepare_v2(m_db, routeQuery.str().c_str(), -1, &routeStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    
    std::vector<std::string> routeIds;
    while ((rc = sqlite3_step(routeStmt)) == SQLITE_ROW) {
        std::string routeId = reinterpret_cast<const char*>(sqlite3_column_text(routeStmt, 0));
        routeIds.push_back(routeId);
    }
    
    sqlite3_finalize(routeStmt);
    
    // Now, for each route_id, get the stops
    for (const auto& routeId : routeIds) {
        std::stringstream stopsQuery;
        stopsQuery << "SELECT stop_name FROM route_stops WHERE route_id = '" << routeId 
                   << "' ORDER BY stop_order;";
        
        sqlite3_stmt* stopsStmt;
        rc = sqlite3_prepare_v2(m_db, stopsQuery.str().c_str(), -1, &stopsStmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
            return false;
        }
        
        std::vector<std::string> stops;
        while ((rc = sqlite3_step(stopsStmt)) == SQLITE_ROW) {
            std::string stopName = reinterpret_cast<const char*>(sqlite3_column_text(stopsStmt, 0));
            stops.push_back(stopName);
        }
        
        sqlite3_finalize(stopsStmt);
        
        if (!stops.empty()) {
            routes.push_back(stops);
        }
    }
    
    return true;
}

} // namespace CJ