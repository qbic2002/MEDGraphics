include_directories(../view-lib/include)

add_executable(MEDGraphics
        start.cpp
        core/ImageFile.cpp
        core/ImageFileStorage.cpp
        core/CyclicImageFileSpan.cpp
        core/MyApp.cpp core/MyApp.h
        math/vec3.cpp math/vec3.h
        core/R.h
        widget/BgRenderer.cpp widget/BgRenderer.h
        widget/ImageView.cpp widget/ImageView.h
        widget/RootView.cpp widget/RootView.h
        widget/WindowBar.cpp widget/WindowBar.h
        widget/TextView.cpp widget/TextView.h
        widget/DropDownView.cpp widget/DropDownView.h
        widget/MessageView.cpp widget/MessageView.h)

target_link_libraries(view ${LIBS})
target_link_libraries(MEDGraphics view)
target_link_libraries(MEDGraphics ${LIBS})
add_dependencies(MEDGraphics copy_assets)

#add_executable(MEDGraphicsPNM
#        pnm/main.cpp
#        pnm/pnmUtils.cpp pnm/pnmUtils.h
#        pnm/PNMImage.h
#        utils/file.cpp utils/explorer_utils.cpp pnm/PNMImage.cpp)