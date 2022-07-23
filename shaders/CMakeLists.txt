project(SHADERS)

FILE(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
SET(names "")

set(ALL_GLSL "")
set(ALL_VERT "")
set(ALL_TCS "")
set(ALL_TES "")
set(ALL_GEOM "")
set(ALL_FRAG "")
set(ALL_COMP "")
FOREACH(child ${children})
	file(GLOB_RECURSE GLSL "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.glsl")
	file(GLOB_RECURSE VERT "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.vert" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.vertex" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.vs")
	file(GLOB_RECURSE TCS "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.tcs" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.cont" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.t_cont" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.tesc")
	file(GLOB_RECURSE TES "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.tes" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.eval" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.t_eval" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.tese")
	file(GLOB_RECURSE GEOM "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.geom" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.geo" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.geometry *.gs")
	file(GLOB_RECURSE FRAG "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.frag" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.fragment" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.fs" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.ps")
	file(GLOB_RECURSE COMP "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.comp" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.compute" "${CMAKE_CURRENT_SOURCE_DIR}/${child}/*.cs")
	
	SOURCE_GROUP("${child}\\GLSL-Shaders" FILES ${GLSL})
	SOURCE_GROUP("${child}\\Vertex-Shaders" FILES ${VERT})
	SOURCE_GROUP("${child}\\Tessellation-Control-Shaders" FILES ${TCS})
	SOURCE_GROUP("${child}\\Tessellation-Evaluation-Shaders" FILES ${TES})
	SOURCE_GROUP("${child}\\Geometry-Shaders" FILES ${GEOM})
	SOURCE_GROUP("${child}\\Fragment-Shaders" FILES ${FRAG})
	SOURCE_GROUP("${child}\\Compute-Shaders" FILES ${COMP})
	
	set(ALL_GLSL ${ALL_GLSL} ${GLSL})
	set(ALL_VERT ${ALL_VERT} ${VERT})
	set(ALL_TCS ${ALL_TCS} ${TCS})
	set(ALL_TES ${ALL_TES} ${TES})
	set(ALL_GEOM ${ALL_GEOM} ${GEOM})
	set(ALL_FRAG ${ALL_FRAG} ${FRAG})
	set(ALL_COMP ${ALL_COMP} ${COMP})
ENDFOREACH()

# non directory shaders
file(GLOB GLSL "${CMAKE_CURRENT_SOURCE_DIR}/*.glsl")
file(GLOB VERT "${CMAKE_CURRENT_SOURCE_DIR}/*.vert" "${CMAKE_CURRENT_SOURCE_DIR}/*.vertex" "${CMAKE_CURRENT_SOURCE_DIR}/*.vs")
file(GLOB TCS "${CMAKE_CURRENT_SOURCE_DIR}/*.tcs" "${CMAKE_CURRENT_SOURCE_DIR}/*.cont" "${CMAKE_CURRENT_SOURCE_DIR}/*.t_cont")
file(GLOB TES "${CMAKE_CURRENT_SOURCE_DIR}/*.tes" "${CMAKE_CURRENT_SOURCE_DIR}/*.eval" "${CMAKE_CURRENT_SOURCE_DIR}/*.t_eval")
file(GLOB GEOM "${CMAKE_CURRENT_SOURCE_DIR}/*.geom" "${CMAKE_CURRENT_SOURCE_DIR}/*.geo" "${CMAKE_CURRENT_SOURCE_DIR}/*.geometry *.gs")
file(GLOB FRAG "${CMAKE_CURRENT_SOURCE_DIR}/*.frag" "${CMAKE_CURRENT_SOURCE_DIR}/*.fragment" "${CMAKE_CURRENT_SOURCE_DIR}/*.fs" "${CMAKE_CURRENT_SOURCE_DIR}/*.ps")
file(GLOB COMP "${CMAKE_CURRENT_SOURCE_DIR}/*.comp" "${CMAKE_CURRENT_SOURCE_DIR}/*.compute" "${CMAKE_CURRENT_SOURCE_DIR}/*.cs")

SOURCE_GROUP("default\\GLSL-Shaders" FILES ${GLSL})
SOURCE_GROUP("default\\Vertex-Shaders" FILES ${VERT})
SOURCE_GROUP("default\\Tessellation-Control-Shaders" FILES ${TCS})
SOURCE_GROUP("default\\Tessellation-Evaluation-Shaders" FILES ${TES})
SOURCE_GROUP("default\\Geometry-Shaders" FILES ${GEOM})
SOURCE_GROUP("default\\Fragment-Shaders" FILES ${FRAG})
SOURCE_GROUP("default\\Compute-Shaders" FILES ${COMP})
set(ALL_GLSL ${ALL_GLSL} ${GLSL})
set(ALL_VERT ${ALL_VERT} ${VERT})
set(ALL_TCS ${ALL_TCS} ${TCS})
set(ALL_TES ${ALL_TES} ${TES})
set(ALL_GEOM ${ALL_GEOM} ${GEOM})
set(ALL_FRAG ${ALL_FRAG} ${FRAG})
set(ALL_COMP ${ALL_COMP} ${COMP})

if(MSVC)
	add_custom_target(shaders_sources SOURCES ${ALL_GLSL} ${ALL_VERT} ${ALL_TCS} ${ALL_TES} ${ALL_GEOM} ${ALL_FRAG} ${ALL_COMP})
endif()

add_library(shaders INTERFACE)

install(TARGETS  shaders
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION lib/static)
	# Include dirs for headers
	install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION bin
			FILES_MATCHING 
			PATTERN "*"
			PATTERN "*.txt" EXCLUDE)