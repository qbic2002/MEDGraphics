add_executable(MEDGraphics
        start.cpp
        utils/measureFps.cpp utils/measureFps.h
        utils/windowSize.cpp utils/windowSize.h
        math/vec3.cpp math/vec3.h utils/random.cpp utils/random.h)

add_executable(MEDGraphicsPNM
        pnm/main.cpp
        pnm/utils/fileUtil.cpp pnm/utils/pnmUtil.cpp pnm/utils/pnmUtil.h)

target_link_libraries(MEDGraphics ${LIBS})