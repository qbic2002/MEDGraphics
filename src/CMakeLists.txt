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
target_link_libraries(MEDGraphics view)
add_dependencies(MEDGraphics copy_assets)

add_executable(Experiment
        exp/experiment.cpp
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
        widget/MessageView.cpp widget/MessageView.h exp/ExpApp.cpp exp/ExpApp.h)
target_link_libraries(Experiment view)