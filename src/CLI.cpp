#include "../include/CLI.hpp"
#include <algorithm>

namespace CJ {

void CLI::getIntInput(int& input) {
    while (!(std::cin >> input)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string CLI::getStringInput() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void CLI::getValidIntInput(int min, int max, int& input) {
    while(true) {
        if (std::cin >> input && input >= min && input <= max) {
            break;
        }
        std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void CLI::getValidPositiveInt(int& input) {
    while (true) {
        if (std::cin >> input && input >= 0) {
            break;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a positive number: ";
    }
}

void CLI::getNewValidId(int& id) {
    while (true) {
        if (std::cin >> id && id > 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            auto it = std::find_if(CJ::Management::m_trains.begin(), CJ::Management::m_trains.end(),
                                [id](const Train& train) { return train.getId() == id; });
            if (it == CJ::Management::m_trains.end()) {
                break;
            }
            std::cout << "Train with ID " << id << " already exists. Please enter a new ID: ";
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a positive number: " << std::endl;
        }
    }
}

std::string CLI::formatStationName(const std::string& name) {
    if (name.empty()) return name;
    
    std::string formattedName = name;
    bool newWord = true;
    
    for (size_t i = 0; i < formattedName.length(); ++i) {
        if (newWord) {
            formattedName[i] = std::toupper(formattedName[i]);
            newWord = false;
        } else {
            formattedName[i] = std::tolower(formattedName[i]);
        }
        if (formattedName[i] == ' ') {
            newWord = true;
        }
    }
    return formattedName;
}

bool CLI::isStationNameTaken(const std::string& name) {
    std::string formattedName = formatStationName(name);
    return std::any_of(CJ::Management::m_stations.begin(), CJ::Management::m_stations.end(),
        [&formattedName](const Station& station) {
            return formatStationName(station.getName()) == formattedName;
        });
}

template <typename T>
bool CLI::displayUsedObjects(const std::string& prompt, 
                           const std::vector<T>& collection,
                           std::function<std::string(const T&)> getIdFunc) {
    if (collection.empty()) {
        std::cout << "No items available to display.\n";
        return false;
    }

    std::cout << prompt << std::endl;
    for (const auto& item : collection) {
        std::cout << formatStationName(getIdFunc(item)) << "\n";
    }
    return true;
}

bool CLI::compareStationNames(const std::string& name1, const std::string& name2) {
    return formatStationName(name1) == formatStationName(name2);
}

void CLI::displayMainMenu() {
    std::cout << "\n=== Train Management System ===\n"
              << "1. Train Operations\n"
              << "2. Station Operations\n"
              << "3. Route Operations\n"
              << "4. Exit\n"
              << "Enter your choice (1-4): ";
}

void CLI::handleTrainOperations(DatabaseManager& db) {
    while (true) {
        std::cout << "\n=== Train Operations ===\n"
                  << "1. Add New Train\n"
                  << "2. Delete Train\n"
                  << "3. Display Train Information\n"
                  << "4. Return to Main Menu\n"
                  << "Enter your choice (1-4): ";

        int choice;
        getIntInput(choice);

        switch (choice) {
            case 1: {
                displayUsedObjects<Train>("Used Train IDs:", CJ::Management::m_trains,
                    [](const Train& train) { return std::to_string(train.getId()); });

                std::cout << "Enter train name: ";
                std::string name = getStringInput();
                std::cout << "Enter train speed (km/h): ";
                int speed;
                getValidPositiveInt(speed);
                std::cout << "Enter train capacity: ";
                int capacity;
                getValidPositiveInt(capacity);
                std::cout << "Enter train ID: ";
                int id;
                getNewValidId(id);
                std::cout << "Enter wagon count: ";
                int wagonCount;
                getValidPositiveInt(wagonCount);

                CJ::Management::addTrain(name, speed, capacity, id, wagonCount);
                                        
                std::cout << "Train added successfully!\n";
                break;
            }
            case 2: {
                if (!displayUsedObjects<Train>("Train IDs possible to delete:", CJ::Management::m_trains,
                    [](const Train& train) { return std::to_string(train.getId()); })) {
                    break;
                }

                std::cout << "Enter train ID to delete: ";
                int id;
                getIntInput(id);
                if (CJ::Management::deleteTrain(id)) {
                    std::cout << "Train deleted successfully!\n";
                } else {
                    std::cout << "Train not found.\n";
                }
                break;
            }
            case 3: {
                if (!displayUsedObjects<Train>("Train IDs possible to display:", CJ::Management::m_trains,
                    [](const Train& train) { return std::to_string(train.getId()); })) {
                    break;
                }

                std::cout << "Enter train ID to display: ";
                int id;
                getIntInput(id);
                CJ::Management::displayTrainInfo(id);
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void CLI::handleStationOperations(DatabaseManager& db) {
    while (true) {
        std::cout << "\n=== Station Operations ===\n"
                  << "1. Add New Station\n"
                  << "2. Remove Station\n"
                  << "3. Display Station Information\n"
                  << "4. Return to Main Menu\n"
                  << "Enter your choice (1-4): ";

        int choice;
        getIntInput(choice);

        switch (choice) {
            case 1: {
                displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations,
                    [](const Station& station) { return station.getName(); });

                std::cout << "Enter station name: ";
                std::string name = getStringInput();

                if (isStationNameTaken(name)) {
                    std::cout << "Station name already taken. Please enter a different name.\n";
                    break;
                }

                std::cout << "Enter platform count: ";
                int platformCount;
                getValidPositiveInt(platformCount);

                CJ::Management::addStation(nullptr, platformCount, {}, nullptr, nullptr, name);
                std::cout << "Station added successfully!\n";
                break;
            }
            case 2: {
                if (!displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations,
                    [](const Station& station) { return station.getName(); })) {
                    std::cout << "No stations available to remove.\n";
                    break;
                }

                std::cout << "Enter station name to remove: ";
                std::string name = getStringInput();

                if (CJ::Management::removeStation(name)) {
                    std::cout << "Station removed successfully!\n";
                } else {
                    std::cout << "Station not found.\n";
                }
                break;
            }
            case 3: {
                if (!displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations,
                    [](const Station& station) { return station.getName(); })) {
                    std::cout << "No stations available to display.\n";
                    break;
                }

                std::cout << "Enter station name to display: ";
                std::string name = getStringInput();
                name = formatStationName(name); // Format the input name
                
                // Find the station in the Management's stations
                auto it = std::find_if(CJ::Management::m_stations.begin(), 
                                      CJ::Management::m_stations.end(),
                                      [&name](const Station& s) {
                                          return compareStationNames(s.getName(), name);
                                      });
                
                if (it == CJ::Management::m_stations.end()) {
                    std::cout << "Station '" << name << "' not found.\n";
                } else {
                    CJ::Management::displayStationInfo(it->getName());
                }
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void CLI::handleRouteOperations(DatabaseManager& db) {
    while (true) {
        std::cout << "\nRoute Operations:\n";
        std::cout << "1. Add Route\n";
        std::cout << "2. List Routes\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Choose an option: ";

        int choice;
        getIntInput(choice);

        switch (choice) {
            case 1: {
                if (CJ::Management::m_stations.empty()) {
                    std::cout << "No stations available. Please add stations first.\n";
                    break;
                }

                std::vector<std::string> stops;
                std::cout << "Enter number of stops: ";
                int numStops;
                getValidIntInput(2, 10, numStops);

                // Display available stations
                std::cout << "\nAvailable stations:\n";
                for (const auto& station : CJ::Management::m_stations) {
                    std::cout << "- " << station.getName() << "\n";
                }
                std::cout << "\n";

                // Collect station names
                for (int i = 0; i < numStops; i++) {
                    while (true) {
                        std::cout << "Enter station name for stop " << (i + 1) << ": ";
                        std::string stationName = getStringInput();
                        if (!stationName.empty()) {
                            stationName = CJ::Management::formatStationName(stationName);
                            
                            // Check if station exists
                            auto it = std::find_if(CJ::Management::m_stations.begin(), 
                                                 CJ::Management::m_stations.end(),
                                                 [&stationName](const Station& s) {
                                                     return CJ::Management::compareStationNames(s.getName(), stationName);
                                                 });
                            
                            if (it != CJ::Management::m_stations.end()) {
                                stops.push_back(it->getName()); // Use the exact name from the database
                                break;
                            }
                            std::cout << "Station '" << stationName << "' not found. Please try again.\n";
                        } else {
                            std::cout << "Station name cannot be empty. Please try again.\n";
                        }
                    }
                }

                std::cout << "Enter departure hour (0-23): ";
                int depHour;
                getValidIntInput(0, 23, depHour);

                std::cout << "Enter departure minute (0-59): ";
                int depMin;
                getValidIntInput(0, 59, depMin);

                std::cout << "Enter arrival hour (0-23): ";
                int arrHour;
                getValidIntInput(0, 23, arrHour);

                std::cout << "Enter arrival minute (0-59): ";
                int arrMin;
                getValidIntInput(0, 59, arrMin);

                int depTime = depHour * 60 + depMin;
                int arrTime = arrHour * 60 + arrMin;
                if (arrTime <= depTime) {
                    std::cout << "Arrival time must be after departure time!\n";
                    break;
                }

                // Calculate duration automatically
                int duration = arrTime - depTime;
                std::cout << "Calculated route duration: " << duration << " minutes\n";

                // Create and save the route
                Route newRoute(depHour, depMin, arrHour, arrMin, duration, 
                             nullptr, nullptr, nullptr, stops);
                
                try {
                    if (db.saveRoute(newRoute)) {
                        std::cout << "Route added successfully!\n";
                    } else {
                        std::cout << "Failed to save route. Please check the input values.\n";
                    }
                } catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 2: {
                std::vector<Route> routes;
                if (db.loadRoutes(routes)) {
                    if (routes.empty()) {
                        std::cout << "No routes found.\n";
                    } else {
                        std::cout << "\nCurrent Routes:\n";
                        for (const auto& route : routes) {
                            std::cout << "Route: ";
                            const auto& routeStops = route.getIntermediateStops();
                            for (size_t i = 0; i < routeStops.size(); ++i) {
                                std::cout << routeStops[i];
                                if (i < routeStops.size() - 1) std::cout << " -> ";
                            }
                            std::cout << "\nDeparture: " << route.getDepartureTimeHour() 
                                    << ":" << route.getDepartureTimeMinute()
                                    << " Arrival: " << route.getArrivalTimeHour() 
                                    << ":" << route.getArrivalTimeMinute() 
                                    << " Duration: " << route.getDuration() << " minutes\n\n";
                        }
                    }
                } else {
                    std::cout << "Failed to load routes.\n";
                }
                break;
            }
            case 3:
                return;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}

void CLI::run() {
    DatabaseManager& db = Management::getInstance().getDatabase();
    
    while (true) {
        displayMainMenu();
        int choice;
        getIntInput(choice);

        switch (choice) {
            case 1:
                handleTrainOperations(db);
                break;
            case 2:
                handleStationOperations(db);
                break;
            case 3:
                handleRouteOperations(db);
                break;
            case 4:
                std::cout << "Thank you for using the Train Management System!\n";
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

}