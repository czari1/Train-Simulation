#pragma once
#include <string>
#include <vector>
#include <memory>
 
namespace CJ {
 
class Train;
class Station;
 
class Route {
    private:
        int m_departureTimeHour;
        int m_departureTimeMinute;
        int m_arrivalTimeHour;
        int m_arrivalTimeMinute;
        int m_duration;
        std::shared_ptr<Train> m_assignedTrain;
        std::shared_ptr<Station> m_startStation;
        std::shared_ptr<Station> m_endStation;
        std::vector<std::string> m_intermediateStops;
    
    public:
        Route(int depHour, int depMin, int arrHour, int arrMin,int duration,
              std::shared_ptr<Train> trainPtr, std::shared_ptr<Station> startStation,
              std::shared_ptr<Station> endStation,
              const std::vector<std::string>& intermediateStops);

    void setDepartureTimeHour(int departureTimeHour);
    void setDepartureTimeMinute(int departureTimeMinute);
    void setArrivalTimeHour(int arrivalTimeHour);
    void setArrivalTimeMinute(int arrivalTimeMinute);
    void setDuration(int duration);
    void setIntermediateStops(const std::vector<std::string>& intermediateStops);
    void setTrainAssignment(std::shared_ptr<Train> train);
 

    int getDepartureTimeHour() const;
    int getDepartureTimeMinute() const;
    int getArrivalTimeHour() const;
    int getArrivalTimeMinute() const;
    int getDuration() const;
    std::shared_ptr<Train> getAssignedTrain() const;
    const std::vector<std::string>& getIntermediateStops() const;
 

    void addIntermediateStop(const std::string& stationName);
    int calculateTravelTime() const;
    std::string getStartStation() const;
    std::string getEndStation() const;
 
};
 
} 