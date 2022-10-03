/**
 * @VehicleRouting.cpp
 *
 * Project:	Vehicle routing
 *
 * main function to find vehicle routes
 */

#include <iostream>
#include <cmath>
#include <limits.h>

#include "VehicleRouting.h"

using namespace std;

namespace VehicleRouting {

  constexpr auto M_PI = 3.14159265358979323846;
  constexpr auto EARTH_RADIUS = 6371; //km

  inline double radians(double degrees) {
    return (degrees * M_PI) / 180.0;
  }

  //distance
  inline double haversine(double lon1, double lat1, double lon2, double lat2) {
    lat1 = radians(lat1);
    lon1 = radians(lon1);
    lat2 = radians(lat2);
    lon2 = radians(lon2);
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * asin(sqrt(a));
    return c * EARTH_RADIUS;
  }


  std::tuple<vector<vector<Trip>>, vector<double>> findPathsGreedy(
    std::vector<Point>& customerPoints,
    std::unordered_map<std::string, Point>& pickUpPoints,
    const Vehicle& vehicles,
    const Location& startLocation

  ) {

    // all trips for all vehicles, separated by vehicle "id" - output data
    // *** I chose Trip instead of Trip* of shared_ptr<Trip> here, because 
    // *** struct is not big in memory,
    // *** the whole quantity of data is not big too
    

    vector<vector<Trip>> trips(vehicles.number, vector<Trip>());
    //add "Start" Point to all routes
    for (int i = 0; i < vehicles.number; i++) {
      trips[i].emplace_back(Trip("Start", "", startLocation.latitude, startLocation.longitude));
    }

    //temporary previous point storage
    vector<Point> previousPoints(vehicles.number, Point(startLocation, "Start", ""));

    size_t vectorCustomerPointsActiveSize = customerPoints.size();
    vector<double> distanceInKM(vehicles.number, 0.0);

    //create routes
    while (true) {
      if (vectorCustomerPointsActiveSize == 0) break;
      for (int i = 0; i < vehicles.number; i++) {
        if (vectorCustomerPointsActiveSize == 0) break;

        double minDistance = std::numeric_limits<double>::max();
        int minDistanceRelatedPointIndex = -1;

        // find index of the next customer with a shortest way to it
        for (int j = 0; j < vectorCustomerPointsActiveSize; j++) {
          auto p = customerPoints[j];

          // maybe it is possible to do optimisation here:
          // store ready calculated distances in unordered_map or ... something else
          // but it is necessary to compare times...

          double distanceLoad = haversine(
            previousPoints[i].longitude, previousPoints[i].latitude,
            pickUpPoints[p.parentPointId].longitude, pickUpPoints[p.parentPointId].latitude);

          double distanceOfLoad = haversine(
            pickUpPoints[p.parentPointId].longitude, pickUpPoints[p.parentPointId].latitude,
            p.longitude, p.latitude);

          double distance = distanceLoad + distanceOfLoad;


          if (distance < minDistance) {
            minDistance = distance;
            minDistanceRelatedPointIndex = j;
          }
        }

        //store pickUpPoint
        Point p = pickUpPoints[customerPoints[minDistanceRelatedPointIndex].parentPointId];
        trips[i].emplace_back(Trip(p.id, p.parentPointId, p.latitude, p.longitude));
        //store customer Point
        p = customerPoints[minDistanceRelatedPointIndex];
        trips[i].emplace_back(Trip(p.id, p.parentPointId, p.latitude, p.longitude));

        //set new previous point
        previousPoints[i] = customerPoints[minDistanceRelatedPointIndex];

        //remove added cusmomer Point
        customerPoints[minDistanceRelatedPointIndex] = customerPoints[vectorCustomerPointsActiveSize - 1];
        vectorCustomerPointsActiveSize--;
        distanceInKM[i] += minDistance;

      }
    }

    //TODO: balance the number of trips between vehicles if it is needed

    cout << "Hi" << endl;
    //return  std::make_tuple(trips, distanceInKM);
    return  { std::move(trips), std::move(distanceInKM) };
  }

}

