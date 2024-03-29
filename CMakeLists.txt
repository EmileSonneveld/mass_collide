# CMake entry point
cmake_minimum_required (VERSION 3.10)
project (mass_collide_build_system)

find_package(OpenGL REQUIRED)

#cmake_policy(SET CMP0053 NEW)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc++11-extensions")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
	message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()





# Compile external dependencies ###########
SET(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "If the supplementary tools for Assimp are built in addition to the library.")
SET(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build your own zlib")

add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	#external/AntTweakBar-1.16/include/
	external/glfw-3.0.3/include/GLFW/
	external/glm-0.9.4.0/
	external/glew-1.9.0/include/
	external/assimp/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw #GLFW_303
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# mass_collide_test ####################################

file(GLOB mass_collide_project_SRC
	"mass_collide_project/*.h"
	"mass_collide_project/*.cpp"
	"mass_collide_project/*.c"
)

add_executable(mass_collide_test
	${mass_collide_project_SRC}
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
	common/texture.hpp
	common/controls.cpp
	common/controls.hpp
)

target_link_libraries(mass_collide_test
	${ALL_LIBS}
	assimp
)

# Xcode and Visual working directories
#set_target_properties(mass_collide_test PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mass_collide_project/")
#create_target_launcher(mass_collide_test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/mass_collide_project/")





# SDL ################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${mass_collide_build_system_SOURCE_DIR}/cmake")

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
	message(" * C++ compiler: Emscripten")
	set(CMAKE_CXX_COMPILER_ID "Emscripten")
	# no need to include SDL here, emskripten will do that for us
else()
	message(" * C++ cimpiler: ${CMAKE_CXX_COMPILER_ID}")
#
#	find_package(SDL2 REQUIRED)
#	find_package(SDL2_Image REQUIRED)
#	find_package(SDL2_ttf REQUIRED)
#
#	include_directories(${SDL2_INCLUDE_DIR}
#						${SDL2_IMAGE_INCLUDE_DIR}
#						${SDL2_TTF_INCLUDE_DIR})
#	target_link_libraries(mass_collide_test ${SDL2_LIBRARY}
#								 ${SDL2_IMAGE_LIBRARIES}
#								 ${SDL2_TTF_LIBRARIES})


endif()








SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION "(.*/.*shader$)|(*.glsl$)" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

add_custom_command(
   TARGET mass_collide_test POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/mass_collide_test${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/mass_collide_project/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

