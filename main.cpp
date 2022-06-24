/**
 * @VehicleRouting.cpp
 *
 * Project:	Vehicle routing
 *
 * function main: json parsing/saving
 */

#include <iostream>
#include <charconv>//
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "struct.h"
#include "VehicleRouting.h"

using namespace std;
using namespace nlohmann;
using namespace VehicleRouting;

const string inputPath = "C:/work/CPP/VehicleRouting/input.json";
const string outputPath = "output.json";

int main()
{

  std::cout << std::setprecision(17);

  std::vector<Point> customerPoints;
  std::unordered_map<std::string, Point> pickUpPoints;
  Vehicle vehicles;
  Location startLocation;

  //parsing json
  try {
    ifstream inp(inputPath);
    json input;
    inp >> input;

    for (auto p : input["points"]) {
      Point point;
      p.at("parentPointId").get_to(point.parentPointId);
      p.at("id").get_to(point.id);
      p.at("lat").get_to(point.latitude);
      p.at("lng").get_to(point.longitude);
      p.at("isOffloading").get_to(point.isOffloading);
      if (point.isOffloading) {
        customerPoints.push_back(std::move(point));
      }
      else {
        pickUpPoints[point.id] = point;
      }     
    }
    //TODO: solve for several types of vehicles, not for only one as now !
    for (auto v : input["vehicles"]) {
      v.at("Number").get_to(vehicles.number);
      v.at("Type").get_to(vehicles.type);
    }
    input.at("startLocation").at("Lat").get_to(startLocation.latitude);
    input.at("startLocation").at("Lng").get_to(startLocation.longitude);
  }
  catch (...) {
    cout << "Exception of parsing process" << endl;
    return -1;
  }

  vector<double> distanceInKM;
  vector<vector<Trip>> trips;

  //main process
  tie(trips, distanceInKM) = findPathsGreedy(customerPoints, pickUpPoints, vehicles, startLocation);

  //saving
  int numVehicles = distanceInKM.size();
  string vehicleType = vehicles.type;

  ofstream o(outputPath);
  for (int i = 0; i < numVehicles; i++) {
    json output;
    output["planId"] = std::to_string(i);
    json tripsJsonArray = json::array();
    json tripsJson;
    tripsJson["tripId"] = "T" + std::to_string(i);
    tripsJson["vehicleType"] = vehicleType;
    tripsJson["summary"]["distanceInMeter"] = static_cast<int>(distanceInKM[i]*1000);
    json tripJson = json::array();
    for (auto& t : trips[i]) {
      json trip;
      trip["pointId"] = t.pointId;
      trip["lat"] = t.latitude;
      trip["lng"] = t.longitude;
      trip["parentPointId"] = t.parentPointId;
      tripJson.push_back(trip);
    }
    tripsJson["trip"] = tripJson;
    tripsJsonArray.push_back(tripsJson);
    output["trips"] = tripsJsonArray;

    cout << "result Json: " << output << endl;//todo remove
    o << setw(4) << output << endl;//to file
  }

	return 0;
}