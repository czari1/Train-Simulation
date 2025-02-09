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
    static int getIntInput() {
        int input;
        while (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }

    static std::string getStringInput() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }
    
    static void getValidIntInput(int min, int max, int& input) {
        while(true){
            if (std::cin >> input && input>=min && input<=max) {
                break;
            } else {
                std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    static int getValidPositiveInt() {
        int input;
        while (true) {
            if (std::cin >> input && input >= 0) {
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a positive number: ";
            }
        }
        return input;
    }

    template <typename T>
        static void displayUsedObjects(const std::string& prompt, const std::vector<T>& collection, std::function<std::string(const T&)> getIdFunc) {
        std::cout << prompt << std::endl;
            for (const auto& item : collection) {
                std::cout << formatStationName(getIdFunc(item)) << "\n";
            }
    }

    static int getNewValidId() {
        int id;
        while (true) {
            if (std::cin >> id && id > 0) {
                auto it = std::find_if(CJ::Management::m_trains.begin(), CJ::Management::m_trains.end(),
                                    [id](const Train& train) { return train.getId() == id; });
                if (it == CJ::Management::m_trains.end()) {
                    break;
                } else {
                    std::cout << "Train with ID " << id << " already exists. Please enter a new ID: ";
                }
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a positive number: ";
            }
        }
        return id;
    }

    static std::string formatStationName(const std::string& name) {
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

    static bool isStationNameTaken(const std::string& name) {
        std::string formattedName = formatStationName(name);
        return std::any_of(CJ::Management::m_stations.begin(), CJ::Management::m_stations.end(),
            [&formattedName](const Station& station) {
                return formatStationName(station.getName()) == formattedName;
            });
    }

    static bool compareStationNames(const std::string& name1, const std::string& name2) {
        return formatStationName(name1) == formatStationName(name2);
    }

public:
    static void displayMainMenu() {
        std::cout << "\n=== Train Management System ===\n"
                 << "1. Train Operations\n"
                 << "2. Station Operations\n"
                 << "3. Route Operations\n"
                 << "4. Exit\n"
                 << "Enter your choice (1-4): ";
    }

    static void handleTrainOperations() {
        while (true) {
            std::cout << "\n=== Train Operations ===\n"
                     << "1. Add New Train\n"
                     << "2. Delete Train\n"
                     << "3. Display Train Information\n"
                     << "4. Return to Main Menu\n"
                     << "Enter your choice (1-4): ";

            int choice = getIntInput();
            switch (choice) {
                case 1: {
                    displayUsedObjects<Train>("Used Train IDs:", CJ::Management::m_trains, 
                    [](const Train& train) { return std::to_string(train.getId()); });

                    std::cout << "Enter train name: ";
                    std::string name = getStringInput();
                    std::cout << "Enter train speed (km/h): ";
                    int speed = getValidPositiveInt();
                    std::cout << "Enter train capacity: ";
                    int capacity = getValidPositiveInt();
                    std::cout << "Enter train ID: ";
                    int id = getNewValidId();
                    std::cout << "Enter start station: ";
                    std::string startStation = getStringInput();
                    std::cout << "Enter end station: ";
                    std::string endStation = getStringInput();
                    std::cout << "Enter wagon count: ";
                    int wagonCount = getValidPositiveInt();

                    CJ::Management::addTrain(name, speed, capacity, id, startStation, endStation, 
                                          wagonCount, nullptr, nullptr, nullptr, nullptr);
                    std::cout << "Train added successfully!\n";
                    break;
                }
                case 2: {
                    displayUsedObjects<Train>("Train IDs possible do delete:", CJ::Management::m_trains, 
                    [](const Train& train) { return std::to_string(train.getId()); });

                    std::cout << "Enter train ID to delete: ";
                    int id = getIntInput();
                    if (CJ::Management::deleteTrain(id)) {
                        std::cout << "Train deleted successfully!\n";
                    } else {
                        std::cout << "Train not found.\n";
                    }
                    break;
                }
                case 3: {
                    displayUsedObjects<Train>("Train IDs possible to display:", CJ::Management::m_trains, 
                    [](const Train& train) { return std::to_string(train.getId()); });

                    std::cout << "Enter train ID to display: ";
                    int id = getIntInput();
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

    static void handleStationOperations() {
        while (true) {
            std::cout << "\n=== Station Operations ===\n"
                     << "1. Add New Station\n"
                     << "2. Remove Station\n"
                     << "3. Display Station Information\n"
                     << "4. Return to Main Menu\n"
                     << "Enter your choice (1-4): ";

            int choice = getIntInput();
            switch (choice) {
                case 1: {
                    displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations, [](const Station& station) 
                    { return station.getName(); });
                    
                    std::cout << "Enter station name: ";
                    std::string name = getStringInput();

                    if (isStationNameTaken(name)) {
                        std::cout << "Station name already taken. Please enter a different name.\n";
                        break;
                    }

                    std::cout << "Enter platform count: ";
                    int platformCount = getValidPositiveInt();

                    CJ::Management::addStation(nullptr, platformCount, {}, nullptr, nullptr, name);
                    std::cout << "Station added successfully!\n";
                    break;
                }
                case 2: {
                    displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations, [](const Station& station) 
                    { return station.getName(); });

                    std::cout << "Enter station name to remove: ";
                    std::string name = getStringInput();

                    auto it = std::find_if(CJ::Management::m_stations.begin(), 
                                         CJ::Management::m_stations.end(),
                                         [&name](const Station& station) {
                                             return compareStationNames(station.getName(), name);
                                         });
                                         
                    if (it != CJ::Management::m_stations.end()) {
                        CJ::Management::m_stations.erase(it);
                        std::cout << "Station removed successfully!\n";
                    } else {
                        std::cout << "Station not found.\n";
                    }
                    break;
                }
                case 3: {
                    displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations, 
                        [](const Station& station) { return station.getName(); });
                    
                    std::cout << "Enter station name to display: ";
                    std::string name = getStringInput();
                    
                    auto it = std::find_if(CJ::Management::m_stations.begin(), 
                                         CJ::Management::m_stations.end(),
                                         [&name](const Station& station) {
                                             return compareStationNames(station.getName(), name);
                                         });
                                         
                    if (it != CJ::Management::m_stations.end()) {
                        std::cout << "Station Information:\n"
                                 << "Name: " << it->getName() << "\n"
                                 << "Platform Count: " << it->getPlatformCount() << "\n"
                                 << "Intermediate Stops Count: "
                                 << it->getIntermediateStops().size() << "\n";
                    } else {
                        std::cout << "Station not found.\n";
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

    static void handleRouteOperations() {
        while (true) {
            std::cout << "\n=== Route Operations ===\n"
                     << "1. Display Route Information\n"
                     << "2. Return to Main Menu\n"
                     << "Enter your choice (1-2): ";

            int choice = getIntInput();
            switch (choice) {
                case 1: {
                    CJ::Management::displayAllRoutes();
                    break;
                }
                case 2:
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    static void run() {
        while (true) {
            displayMainMenu();
            int choice = getIntInput();

            switch (choice) {
                case 1:
                    handleTrainOperations();
                    break;
                case 2:
                    handleStationOperations();
                    break;
                case 3:
                    handleRouteOperations();
                    break;
                case 4:
                    std::cout << "Thank you for using the Train Management System!\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

} // namespace CJ