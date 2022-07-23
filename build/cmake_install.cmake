# Install script for directory: D:/cvws2122/framework/CG_allgroups/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/CG_allgroups")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE DIRECTORY FILES "D:/cvws2122/framework/CG_allgroups/src/../../resources")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/cvws2122/framework/CG_allgroups/src/build/02_Dragon_Fractal/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/03_LineClipping/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/04_MovingThingsPractice/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/04_Moving_Fir/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/05_Transform/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/06_Graph/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/07_PerspectiveViewport/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/08_Toon_Example/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/09_Texture_Example/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/09_Transparency_Example/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/10_BezierSurface_Example/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/11_SunEarthMoon_Example/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/shaders/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/CG1_Tools/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/CVK_1_Minimal/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/Eigen/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/SimpleImage/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/glad/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/glfw3/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/glm/cmake_install.cmake")
  include("D:/cvws2122/framework/CG_allgroups/src/build/stb/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/cvws2122/framework/CG_allgroups/src/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
