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

void CLI::handleTrainOperations() {
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
                std::cout << "Enter start station: ";
                std::string startStation = getStringInput();
                std::cout << "Enter end station: ";
                std::string endStation = getStringInput();
                std::cout << "Enter wagon count: ";
                int wagonCount;
                getValidPositiveInt(wagonCount);

                CJ::Management::addTrain(name, speed, capacity, id, startStation, endStation,
                                      wagonCount, nullptr, nullptr, nullptr, nullptr);
                std::cout << "Train added successfully!\n";
                break;
            }
            case 2: {
                displayUsedObjects<Train>("Train IDs possible to delete:", CJ::Management::m_trains,
                    [](const Train& train) { return std::to_string(train.getId()); });

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
                displayUsedObjects<Train>("Train IDs possible to display:", CJ::Management::m_trains,
                    [](const Train& train) { return std::to_string(train.getId()); });

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

void CLI::handleStationOperations() {
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
                displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations,
                    [](const Station& station) { return station.getName(); });

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
                displayUsedObjects<Station>("Used Station Names:", CJ::Management::m_stations,
                    [](const Station& station) { return station.getName(); });

                std::cout << "Enter station name to display: ";
                std::string name = getStringInput();
                CJ::Management::displayStationInfo(name);
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void CLI::handleRouteOperations() {
    while (true) {
        std::cout << "\n=== Route Operations ===\n"
                  << "1. Display Route Information\n"
                  << "2. Add New Route\n"
                  << "3. Return to main menu\n"
                  << "Enter your choice (1-3): ";

        int choice;
        getIntInput(choice);

        switch (choice) {
            case 1:
                CJ::Management::displayAllRoutes();
                break;
            case 2:
                return;
            case 3:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void CLI::run() {
    while (true) {
        displayMainMenu();
        int choice;
        getIntInput(choice);

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

} // namespace CJ