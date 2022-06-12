cmake_minimum_required(VERSION 3.19)
project(durak
	VERSION 0.0.1)

find_library(DEALER_LIBRARY
	NAMES libdealer.a dealer
	PATHS ${CMAKE_CURRENT_LIST_DIR}
	REQUIRED
	NO_DEFAULT_PATH
)


set(SOURCES
	main.cpp
	player.cpp
	#dealer.cpp
)

set(CMAKE_CXX_FLAGS_Debug -g)


if (UNIX) 
	set(LINKER_FLAGS "-no-pie")
	message(WARNING "Using durak with this libdurak is dangerous for your terminal. Please pipe output into | cat -v")
endif(UNIX)

if (WIN32)
	set(LINKER_FLAGS "")
endif(WIN32)

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED TRUE)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PROJECT_NAME} ${DEALER_LIBRARY} ${LINKER_FLAGS}) 

