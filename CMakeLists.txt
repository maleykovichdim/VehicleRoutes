
cmake_minimum_required (VERSION 3.8)

# Todo: add other compilers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

project ("VehicleRouting")
add_executable (VehicleRouting "VehicleRouting.cpp" "VehicleRouting.h" "main.cpp" "struct.h")
target_link_libraries(VehicleRouting nlohmann_json::nlohmann_json)

