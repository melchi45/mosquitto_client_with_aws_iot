cmake_minimum_required(VERSION 2.8)

set ( PROJECT_NAME mosquitto_client_with_aws_iot )

project( ${PROJECT_NAME} )
set ( ${PROJECT_NAME}_HOME ${CMAKE_CURRENT_SOURCE_DIR} )
message("SourceDir: " ${${PROJECT_NAME}_HOME} )

# set where to find additional cmake modules if any
# comment it out if not required
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
message ( "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )

if ( CMAKE_COMPILER_IS_GNUCXX )
   set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive" )
endif ()

# set compiler support for C++11 standard
option(USE_CXX11_STD "Product should be build with C++11 compiler option enabled" ON)

if(USE_CXX11_STD)
    set(CMAKE_CXX_STANDARD 11)
endif()

if(MSVC)
	add_definitions(-D_CONSOLE)
else()
    # GCC or Clang
endif()

find_package(MOSQUITTO REQUIRED)
if(MOSQUITTO_FOUND)
message("MOSQUITTO_LIBRARIES = ${MOSQUITTO_LIBRARIES} ")
message("MOSQUITTO_INCLUDE_DIRS = ${MOSQUITTO_INCLUDE_DIRS} ")
else( MOSQUITTO_FOUND )
    message(FATAL_ERROR "mosquitto library not found")
endif( MOSQUITTO_FOUND )

# use standard file endings - rename to cxx if needed
set(CXX_SRC_EXT_PATTERN ".cpp")
set(C_SRC_EXT_PATTERN ".c")
set(HDR_EXT_PATTERN ".h")

# get all *.cpp files from src to build target
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*${CXX_SRC_EXT_PATTERN})
file(GLOB HDR_FILES ${PROJECT_SOURCE_DIR}/src/*${HDR_EXT_PATTERN})
# makes source and header files group
source_group("Source Files" FILES ${SRC_FILES})
source_group("Header Files" FILES ${HDR_FILES})

# build target
add_executable( ${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES} )

target_include_directories( ${PROJECT_NAME} 
	PRIVATE 
		${MOSQUITTO_INCLUDE_DIRS} 
)

target_link_libraries( ${PROJECT_NAME} 
	PRIVATE
		${MOSQUITTO_LIBRARIES}
)

add_custom_command (
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${PROJECT_NAME}
    COMMENT "Execute ${PROJECT_NAME}"
)