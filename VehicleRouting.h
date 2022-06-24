/**
 * @VehicleRouting.h
 *
 * Project:	Vehicle routing
 *
 * main function to find vehicle routes
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <memory>


#include "struct.h"

using namespace std;

namespace VehicleRouting {

  //Input:
  // vector<Point>& customerPoints - all customer points to visit
  // unordered_map<std::string, Point>& pickUpPoints - all points to visit for loading of goods
  // const Vehicle& vehicles - vehicles number and description
  // const Location& startLocation - start location
  //Output:
  // vector<vector<Trip>> - assigned trips arrays for every vehicles
  // vector<double> - array of distances for every vehicle
  tuple<vector<vector<Trip>>, vector<double>> findPathsGreedy(
    vector<Point>& customerPoints,
    unordered_map<std::string, Point>& pickUpPoints,
    const Vehicle& vehicles,
    const Location& startLocation
  );

}

