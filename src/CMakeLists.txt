add_executable(MEDGraphics
        start.cpp
        utils/measureFps.cpp utils/measureFps.h
        utils/windowSize.cpp utils/windowSize.h
        )
target_link_libraries(MEDGraphics ${LIBS})