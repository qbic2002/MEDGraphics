add_executable(MEDGraphics
        start.cpp
        utils/measureFps.cpp utils/measureFps.h
        utils/windowSize.cpp utils/windowSize.h
        utils/file.cpp utils/file.h
        math/vec3.cpp math/vec3.h utils/random.cpp utils/random.h
        stb_image.h
        pnm/utils/pnmUtil.cpp pnm/utils/pnmUtil.h pnm/PNMImage.h
        gl/ShaderProgram.cpp gl/ShaderProgram.h gl/Texture.cpp gl/Texture.h gl/utils.cpp gl/utils.h
        core/Context.cpp core/Context.h view/View.cpp view/View.h core/Assets.cpp core/Assets.h view/Style.h view/ViewGroup.cpp view/ViewGroup.h
        widget/BgRenderer.cpp widget/BgRenderer.h widget/ImageView.cpp widget/ImageView.h widget/WindowBar.cpp widget/WindowBar.h widget/RootView.cpp widget/RootView.h
        img/imageLoader.cpp img/imageLoader.h img/rgba.h img/AbstractRaster.h gl/FontRenderer.cpp gl/FontRenderer.h utils/ft_error_check.cpp utils/ft_error_check.h widget/TextView.cpp widget/TextView.h widget/DropDownView.cpp widget/DropDownView.h utils/explorerUtils.cpp utils/R.h widget/MessageView.cpp widget/MessageView.h
        utils/encoding.cpp utils/encoding.h utils/measureTime.cpp utils/measureTime.h utils/logging.cpp utils/logging.h core/ImageFile.cpp core/ImageFileStorage.cpp core/CyclicImageFileSpan.cpp img/PixelRGB8.h)
target_link_libraries(MEDGraphics ${LIBS})
add_dependencies(MEDGraphics copy_assets)

add_executable(MEDGraphicsPNM
        pnm/main.cpp
        pnm/utils/pnmUtil.cpp pnm/utils/pnmUtil.h
        pnm/PNMImage.h
        utils/file.cpp utils/explorerUtils.cpp)