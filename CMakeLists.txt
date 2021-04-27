# Project name
project(dt060g)

# Minimum required cmake version
cmake_minimum_required(VERSION 3.2)

# Compile with c++ 11 support
set(CMAKE_CXX_STANDARD 11)

# Static inclusion of dependencies required for the executable to be independent
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Save name of directories in variables for reuse
set(PROJECT "Project")

# Add source directories for the project
aux_source_directory(${PROJECT}/src PROJECT_SRC)

# Add executables for all configurations!
add_executable(${PROJECT_NAME}_${PROJECT} ${PROJECT_SRC} Project/src/vehicle.cpp Project/include/vehicle.h Project/src/vehicle_engine.cpp Project/include/vehicle_engine.h Project/src/train.cpp Project/include/train.h Project/src/sim_time.cpp Project/include/sim_time.h Project/src/train_station.cpp Project/include/train_station.h Project/src/train_event.cpp Project/include/train_event.h Project/src/simulation_menu.cpp Project/include/simulation_menu.h Project/src/simulation.cpp Project/include/simulation.h Project/src/sim_logger.cpp Project/include/sim_logger.h Project/include/train_route.h)


# In order to avoid '../../../' semantics in include paths (relative), we need to add
# target directories to respective run configuration

target_include_directories(${PROJECT_NAME}_${PROJECT} PRIVATE "./${PROJECT}/include")