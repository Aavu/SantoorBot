add_executable(${PROJECT_NAME} src/main.cpp src/EposController.cpp src/SantoorBot.cpp src/Slider.cpp src/Striker.cpp src/rtpmidi.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include ${CMAKE_SOURCE_DIR}/3rdparty/rtpmidid/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty/rtpmidid/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC -lEposCmd -lrtpmidid -lfmt -lavahi-common -lavahi-client -lpthread)