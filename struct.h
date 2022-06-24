/**
 * @struct.h
 *
 * Project:	Vehicle routing
 *
 * common structs
 */


#pragma once

#include <string>

namespace VehicleRouting {

  struct Vehicle {
    std::string type;
    int number;
  };

  struct Location {
    double latitude;
    double longitude;
  };

  struct Trip {
    Trip() {}
    Trip( std::string pointId, std::string parentPointId, double latitude, double longitude) :
      pointId(pointId), parentPointId(parentPointId), latitude(latitude), longitude(longitude) {}

    std::string pointId;
    std::string parentPointId;
    double latitude;
    double longitude;
  };

  struct Point {
    Point() {}
    Point(const Location& loc, std::string id, std::string parentPointId) :
      latitude(loc.latitude), longitude(loc.longitude), id(id), parentPointId(parentPointId) {}
    std::string id;
    std::string parentPointId;
    double latitude;
    double longitude;
    bool isOffloading;
  };

  struct Summary {
    size_t distanceInMeter;
  };
}