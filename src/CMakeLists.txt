add_executable(MEDGraphics
        start.cpp
        utils/measureFps.cpp utils/measureFps.h
        utils/windowSize.cpp utils/windowSize.h
        math/vec3.cpp math/vec3.h utils/random.cpp utils/random.h
        stb_image.h
        pnm/utils/fileUtil.cpp
        pnm/utils/pnmUtil.cpp pnm/utils/pnmUtil.h
        pnm/PNMImage.h utils/file.cpp utils/file.h gl/ShaderProgram.cpp gl/ShaderProgram.h
        view/Context.cpp view/Context.h
        view/BgRenderer.cpp view/BgRenderer.h view/ImageView.cpp view/ImageView.h view/View.cpp view/View.h img/imageLoader.cpp img/imageLoader.h img/rgba.h img/AbstractRaster.h gl/Texture.cpp gl/Texture.h gl/utils.cpp gl/utils.h)
target_link_libraries(MEDGraphics ${LIBS})
add_dependencies(MEDGraphics copy_assets)

add_executable(process_assets
        process_assets.cpp stb_image.h stb_image_write.h
        utils/file.cpp utils/file.h)

add_executable(MEDGraphicsPNM
        pnm/main.cpp
        pnm/utils/fileUtil.cpp
        pnm/utils/pnmUtil.cpp pnm/utils/pnmUtil.h
        pnm/PNMImage.h
        utils/file.cpp)

add_executable(TextureTest
        textureTest.cpp
        utils/measureFps.cpp utils/measureFps.h
        utils/windowSize.cpp utils/windowSize.h
        math/vec3.cpp math/vec3.h utils/random.cpp utils/random.h
        stb_image.h)
target_link_libraries(TextureTest ${LIBS})