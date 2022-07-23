cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake CACHE PATH "Project specific path. Set manually if it was not found.")
set(FRAMEWORK_BASE_PATH "${CMAKE_SOURCE_DIR}/../..")
set(CMAKE_BASE_PATH "${FRAMEWORK_BASE_PATH}/cmake")

include(macros)

NameFromDir(${CMAKE_SOURCE_DIR} ProjectId)
project(${ProjectId})

include(ProjectSetup)