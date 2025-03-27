#pragma once
#include <string>
#include <vector>
#include <memory>
 
namespace CJ {
 
class Route;
class Station;

class Train {
private:
    std::string m_trainName;
    int m_speed;
    int m_capacity;
    int m_id;
    int m_wagonCount;
    std::shared_ptr<Route> m_departureTimeHour;
    std::shared_ptr<Route> m_departureTimeMinute;
    std::shared_ptr<Route> m_arrivalTimeHour;
    std::shared_ptr<Route> m_arrivalTimeMinute;
 
public:
     
    Train(const std::string& trainName = "", 
            int speed = 0, 
            int capacity = 0, 
            int id = 0, 
            int wagonCount = 0, 
            std::shared_ptr<Route> departureTimeHour = nullptr,
            std::shared_ptr<Route> departureTimeMinute = nullptr, 
            std::shared_ptr<Route> arrivalTimeHour = nullptr,
            std::shared_ptr<Route> arrivalTimeMinute = nullptr);

    void setTrainName(const std::string& trainName);
    void setSpeed(int speed);
    void setCapacity(int capacity);
    void setId(int id);
    void setWagonCount(int wagonCount);
    void setDepartureTimeHour(std::shared_ptr<Route> departureTimeHour);
    void setDepartureTimeMinute(std::shared_ptr<Route> departureTimeMinute);
    void setArrivalTimeHour(std::shared_ptr<Route> arrivalTimeHour);
    void setArrivalTimeMinute(std::shared_ptr<Route> arrivalTimeMinute);
 

    std::string getTrainName() const;
    int getSpeed() const;
    int getCapacity() const;
    int getId() const;
    int getWagonCount() const;
    std::shared_ptr<Route> getDepartureTimeHour() const;
    std::shared_ptr<Route> getDepartureTimeMinute() const;
    std::shared_ptr<Route> getArrivalTimeHour() const;
    std::shared_ptr<Route> getArrivalTimeMinute() const;
 };
 
 } 