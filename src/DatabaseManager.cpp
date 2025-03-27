#include "../include/DatabaseManager.hpp"
#include "../include/Management.hpp"
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

    std::filesystem::path dbDir = std::filesystem::current_path() / "database";
    
    std::error_code ec;
    if (!std::filesystem::exists(dbDir)) {
        if (!std::filesystem::create_directory(dbDir, ec)) {
            std::cerr << "Error creating directory: " << ec.message() << std::endl;
            return false;
        }
    }

    std::string fullPath = (dbDir / "train_system.db").string();
    std::cout << "Attempting to create/open database at: " << fullPath << std::endl;

    int rc = sqlite3_open_v2(fullPath.c_str(), &m_db, 
                           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                           nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }

    m_isConnected = true;
    std::cout << "Database connected successfully to: " << fullPath << std::endl;
    
    if (!prepareDatabase()) {
        std::cerr << "Failed to initialize database tables" << std::endl;
        disconnect();
        return false;
    }
    
    return true;
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
    if (!m_isConnected) {
        std::cerr << "Database not connected" << std::endl;
        return false;
    }

    std::string createTrainsTable = 
        "CREATE TABLE IF NOT EXISTS trains ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "speed INTEGER NOT NULL,"
        "capacity INTEGER NOT NULL,"
        "wagon_count INTEGER NOT NULL"
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


    bool success = executeQuery(createStationsTable) &&
                  executeQuery(createTrainsTable) &&
                  executeQuery(createRoutesTable) &&
                  executeQuery(createRouteStopsTable) &&
                  executeQuery(createTrainRoutesTable);

    if (success) {
        std::cout << "Database tables created successfully!" << std::endl; 
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
                std::stringstream line;
                for (int i = 0; i < argc; i++) {
                    line << colNames[i] << ": " << (argv[i] ? argv[i] : "NULL");
                    
                    if (i < argc - 1) {
                        if (std::string(colNames[i]) == "id" || 
                            std::string(colNames[i]) == "name") {
                            line << std::string(8, ' ');
                        } else if (std::string(colNames[i]) == "speed") {
                            line << std::string(6, ' ');
                        } else if (std::string(colNames[i]) == "capacity" ||
                                 std::string(colNames[i]) == "wagon_count") {
                            line << std::string(3, ' ');
                        }
                    }
                }
                std::cout << line.str() << std::endl;
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
    std::string sql = "INSERT OR REPLACE INTO trains "
                     "(id, name, speed, capacity, wagon_count) VALUES ("
                     + std::to_string(train.getId()) + ", '"
                     + train.getTrainName() + "', "
                     + std::to_string(train.getSpeed()) + ", "
                     + std::to_string(train.getCapacity()) + ", "
                     + std::to_string(train.getWagonCount()) + ");";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::loadTrains(std::vector<Train>& trains) {
    const char* sql = "SELECT id, name, speed, capacity, wagon_count FROM trains;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int speed = sqlite3_column_int(stmt, 2);
        int capacity = sqlite3_column_int(stmt, 3);
        int wagonCount = sqlite3_column_int(stmt, 4);

        trains.emplace_back(name, speed, capacity, id, wagonCount);
    }

    sqlite3_finalize(stmt);
    return true;
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
    return saveTrain(train); 
}

bool DatabaseManager::getTrainById(int id, Train& train) {
    if (!m_isConnected) {
        return false;
    }
    
    std::stringstream query;
    query << "SELECT id, name, speed, capacity, wagon_count "
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
        int speed = std::max(1, sqlite3_column_int(stmt, 2));
        int capacity = std::max(1, sqlite3_column_int(stmt, 3));
        int wagonCount = std::max(1, sqlite3_column_int(stmt, 4));
        
        try {
            train = Train(name, speed, capacity, id, wagonCount);
            found = true;
        } catch (const std::exception& e) {
            std::cerr << "Error creating train object: " << e.what() << std::endl;
            found = false;
        }
    }
    
    sqlite3_finalize(stmt);
    return found;
}

std::string DatabaseManager::escapeString(const std::string& str) const {
    std::string escaped = str;
    size_t pos = 0;

    while ((pos = escaped.find('\'', pos)) != std::string::npos) {
        escaped.insert(pos, 1, '\'');
        pos += 2;
    }
    
    return escaped;
}

bool DatabaseManager::saveStation(const Station& station) {
    if (!m_isConnected) return false;

    std::stringstream checkQuery;
    checkQuery << "SELECT name FROM stations WHERE name COLLATE NOCASE = '" 
               << escapeString(station.getName()) << "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, checkQuery.str().c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Station '" + station.getName() + "' already exists");
    }
    sqlite3_finalize(stmt);

    std::stringstream query;
    query << "INSERT INTO stations (name, platform_count) VALUES ("
          << "'" << escapeString(station.getName()) << "', "
          << station.getPlatformCount() << ");";
    
    return executeQuery(query.str());
}

bool DatabaseManager::loadStations(std::vector<Station>& stations) {
    if (!m_isConnected) {
        return false;
    }

    stations.clear();

    const char* query = "SELECT name, platform_count FROM stations;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int platformCount = sqlite3_column_int(stmt, 1);
        
        stations.emplace_back(nullptr, platformCount, std::vector<std::shared_ptr<Route>>{}, nullptr, nullptr, name);
    }
    
    sqlite3_finalize(stmt);
    return rc != SQLITE_ERROR;
}

bool DatabaseManager::updateStation(const Station& station) {
    return saveStation(station); 
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
    if (!m_isConnected) return false;

    std::stringstream query;
    query << "SELECT * FROM stations WHERE name COLLATE NOCASE = '" 
          << escapeString(name) << "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query.str().c_str(), -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            station = Station(
                nullptr,
                sqlite3_column_int(stmt, 1),  
                {},
                nullptr,
                nullptr,
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))  
            );
            sqlite3_finalize(stmt);
            return true;
        }
    }
    
    sqlite3_finalize(stmt);
    return false;
}

bool DatabaseManager::loadRoutes(std::vector<Route>& routes) {
    if (!m_isConnected) {
        return false;
    }

    const char* query = "SELECT identifier, dep_hour, dep_minute, arr_hour, "
                       "arr_minute, duration FROM routes;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return false;
    }

    routes.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string identifier = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int depHour = sqlite3_column_int(stmt, 1);
        int depMin = sqlite3_column_int(stmt, 2);
        int arrHour = sqlite3_column_int(stmt, 3);
        int arrMin = sqlite3_column_int(stmt, 4);
        int duration = sqlite3_column_int(stmt, 5);

        // Get stops for this route
        std::vector<std::string> stops;
        std::stringstream stopsQuery;
        stopsQuery << "SELECT station_name FROM route_stops WHERE route_id = '" 
                  << escapeString(identifier) << "' ORDER BY stop_order;";

        sqlite3_stmt* stopsStmt;
        rc = sqlite3_prepare_v2(m_db, stopsQuery.str().c_str(), -1, &stopsStmt, nullptr);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stopsStmt) == SQLITE_ROW) {
                const char* stationName = reinterpret_cast<const char*>(
                    sqlite3_column_text(stopsStmt, 0));
                stops.push_back(stationName);
            }
            sqlite3_finalize(stopsStmt);

            routes.emplace_back(depHour, depMin, arrHour, arrMin, duration, 
                              nullptr, nullptr, nullptr, stops);
        }
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::saveRoute(const Route& route) {
    if (!m_isConnected) return false;

    // Generate identifier from stops
    std::string identifier = generateRouteIdentifier(route.getIntermediateStops());

    // Check if route already exists
    std::stringstream checkQuery;
    checkQuery << "SELECT identifier FROM routes WHERE identifier COLLATE NOCASE = '" 
               << escapeString(identifier) << "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, checkQuery.str().c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Route from '" + route.getIntermediateStops().front() + 
                               "' to '" + route.getIntermediateStops().back() + "' already exists");
    }
    sqlite3_finalize(stmt);

    // Start transaction
    if (!executeQuery("BEGIN TRANSACTION;")) {
        return false;
    }

    // Insert the route
    std::stringstream routeQuery;
    routeQuery << "INSERT INTO routes (identifier, dep_hour, dep_minute, arr_hour, arr_minute, duration) "
               << "VALUES ('" << escapeString(identifier) << "', "
               << route.getDepartureTimeHour() << ", "
               << route.getDepartureTimeMinute() << ", "
               << route.getArrivalTimeHour() << ", "
               << route.getArrivalTimeMinute() << ", "
               << route.getDuration() << ");";

    if (!executeQuery(routeQuery.str())) {
        executeQuery("ROLLBACK;");
        return false;
    }

    // Insert stops
    const auto& stops = route.getIntermediateStops();
    for (size_t i = 0; i < stops.size(); ++i) {
        std::stringstream stopQuery;
        stopQuery << "INSERT INTO route_stops (route_id, station_name, stop_order) VALUES ('"
                 << escapeString(identifier) << "', '"
                 << escapeString(stops[i]) << "', "
                 << i << ");";

        if (!executeQuery(stopQuery.str())) {
            executeQuery("ROLLBACK;");
            return false;
        }
    }

    return executeQuery("COMMIT;");
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

}