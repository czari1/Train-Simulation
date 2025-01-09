/**
 * @file CLI.hpp
 * @brief Header file for the Command Line Interface (CLI) class for managing trains, stations, and routes.
 * 
 * This file provides the declaration of the `CLI` class, which serves as the entry point
 * for the Train Management System. The `CLI` class provides menus and functionality for
 * performing operations on trains, stations, and routes.
 * 
 * @author [Cezary Jaros]
 * @date [09.01.2025]
 */

#pragma once
#include <string>
#include <iostream>
#include <limits>
#include "Train.hpp"
#include "Station.hpp"
#include "Route.hpp"

namespace CJ {
/**
 * @brief Predefined route instances.
 */
extern Route routeInstance1;
extern Route routeInstance2;
extern Route routeInstance3;

/**
 * @class CLI
 * @brief Provides a command-line interface for train, station, and route management.
 * 
 * This class handles user input and delegates operations to other modules such as Train,
 * Station, and Route. It includes menus and methods for handling user interactions.
 */

class CLI {
private:

    /**
     * @brief Reads an integer input from the user.
     * 
     * Prompts the user until a valid integer input is entered.
     * @return Validated integer input from the user.
     */

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

    /**
     * @brief Reads a string input from the user.
     * 
     * Captures a line of text entered by the user.
     * @return String input from the user.
     */

    static std::string getStringInput() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

public:

    /**
     * @brief Displays the main menu of the application.
     * 
     * Provides options for train operations, station operations, route operations, 
     * and exiting the application.
     */

    static void displayMainMenu() {
        std::cout << "\n=== Train Management System ===\n"
                 << "1. Train Operations\n"
                 << "2. Station Operations\n"
                 << "3. Route Operations\n"
                 << "4. Exit\n"
                 << "Enter your choice (1-4): ";
    }

    /**
     * @brief Handles train-related operations.
     * 
     * Allows the user to add, delete, and display information about trains.
     */

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
                    std::cout << "Enter train name: ";
                    std::string name = getStringInput();
                    std::cout << "Enter train speed (km/h): ";
                    int speed = getIntInput();
                    std::cout << "Enter train capacity: ";
                    int capacity = getIntInput();
                    std::cout << "Enter train ID: ";
                    int id = getIntInput();
                    std::cout << "Enter start station: ";
                    std::string startStation = getStringInput();
                    std::cout << "Enter end station: ";
                    std::string endStation = getStringInput();
                    std::cout << "Enter wagon count: ";
                    int wagonCount = getIntInput();

                    Train::addTrain(name, speed, capacity, id, startStation, endStation, 
                                  wagonCount, nullptr, nullptr, nullptr, nullptr);
                    std::cout << "Train added successfully!\n";
                    break;
                }
                case 2: {
                    std::cout << "Enter train ID to delete: ";
                    int id = getIntInput();
                    if (Train::deleteTrain(id)) {
                        std::cout << "Train deleted successfully!\n";
                    } else {
                        std::cout << "Train not found.\n";
                    }
                    break;
                }
                case 3: {
                    std::cout << "Enter train ID to display: ";
                    int id = getIntInput();
                    Train::displayTrainInfo(id);
                    break;
                }
                case 4:
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    /**
     * @brief Handles station-related operations.
     * 
     * Allows the user to add, remove, and display information about stations.
     */

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
                    std::cout << "Enter station name: ";
                    std::string name = getStringInput();
                    std::cout << "Enter platform count: ";
                    int platformCount = getIntInput();

                    Station::addStation(nullptr, platformCount, {}, nullptr, nullptr, name);
                    std::cout << "Station added successfully!\n";
                    break;
                }
                case 2: {
                    std::cout << "Enter station name to remove: ";
                    std::string name = getStringInput();
                    if (Station::removeStation(name)) {
                        std::cout << "Station removed successfully!\n";
                    } else {
                        std::cout << "Station not found.\n";
                    }
                    break;
                }
                case 3: {
                    std::cout << "Enter station name to display: ";
                    std::string name = getStringInput();
                    Station::displayStationInfo(name);
                    break;
                }
                case 4:
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    /**
     * @brief Handles route-related operations.
     * 
     * Provides options for displaying route information.
     */

    static void handleRouteOperations() {
        while (true) {
            std::cout << "\n=== Route Operations ===\n"
                     << "1. Display Route Information\n"
                     << "2. Return to Main Menu\n"
                     << "Enter your choice (1-2): ";

            int choice = getIntInput();
            switch (choice) {
                case 1: {
                    std::cout << "\nAvailable Routes:\n";
                    routeInstance1.displayRoute();
                    routeInstance2.displayRoute();
                    routeInstance3.displayRoute();
                    break;
                }
                case 2:
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    /**
     * @brief Starts the main execution loop of the CLI.
     * 
     * Displays the main menu and handles user input until the user chooses to exit.
     */
    

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

}