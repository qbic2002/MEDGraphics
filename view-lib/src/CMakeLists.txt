include_directories(../include)
add_library(view STATIC
        img/imageLoader.cpp
        img/pnmUtils.cpp
        img/PNMImage.cpp
        view/core/Assets.cpp
        view/core/Context.cpp view/core/ContextCallbacks.cpp
        view/core/GLFWWindowWrapper.cpp
        view/core/GLFWWindowProvider.cpp
        view/core/RootViewManager.cpp
        view/View.cpp
        view/ViewGroup.cpp
        view/Style.cpp
        view/widget/LinearLayout.cpp
        utils/encoding.cpp utils/logging.cpp utils/OsLock.cpp
        utils/FontRenderer.cpp
        utils/ShaderProgram.cpp
        utils/Texture.cpp
        utils/gl_utils.cpp
        utils/explorer_utils.cpp
        utils/file.cpp
        utils/ft_error_check.cpp
        utils/measureFps.cpp
        utils/measureTime.cpp
        utils/random.cpp
        utils/ImageFormats.cpp
        img/colorModelConverter.cpp
        utils/pixelTypeMethods.cpp
        img/PixelGray8.cpp
        img/PixelHSL.cpp
        img/PixelRGB8.cpp
        img/PixelRGBA8.cpp
        img/PixelHSV.cpp
        img/PixelYCbCr601.cpp
        img/PixelYCbCr709.cpp
        img/PixelYCoCg.cpp
        img/PixelCMY8.cpp)
target_link_libraries(view ${LIBS})