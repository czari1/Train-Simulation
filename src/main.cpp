#include <iostream>
#include <stdexcept>
#include "../include/Train.hpp"
#include "../include/Route.hpp"
#include "../include/Station.hpp"
#include "../include/CLI.hpp"
#include "../include/Management.hpp"
#include "../include/DatabaseManager.hpp"

int main() {
    try {
        // Initialize the system using Management class
        CJ::Management management;
        management.initializeSystem();
        
        // Test database operations with debug output
        std::cout << "Attempting to create database connection..." << std::endl;
        CJ::DatabaseManager dbManager;
        
        if (dbManager.connect()) {
            std::cout << "Database connection successful" << std::endl;
            dbManager.displayDatabaseContents();
            
            // Test saving a train
            CJ::Train testTrain("TestTrain", 100, 200, 999, "StartStation", "EndStation", 5);
            if (dbManager.saveTrain(testTrain)) {
                std::cout << "Train saved successfully!" << std::endl;
            } else {
                std::cerr << "Failed to save train!" << std::endl;
            }
        } else {
            std::cerr << "Failed to connect to database!" << std::endl;
        }
        
        // Run the command-line interface
        CJ::CLI Cli;
        Cli.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}