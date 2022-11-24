include_directories(../include)
add_library(view STATIC
        img/imageLoader.cpp
        img/pnmUtils.cpp
        img/PNMImage.cpp
        view/core/Assets.cpp
        view/core/Context.cpp view/core/ContextCallbacks.cpp
        view/core/GLFWWindowWrapper.cpp
        view/core/GLFWWindowProvider.cpp
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
        ../include/utils/String.h
        utils/ImageFormats.cpp ../include/utils/ImageFormats.h
        img/colorModelConverter.cpp ../include/img/colorModelConverter.h
        utils/pixelTypeMethods.cpp ../include/utils/pixelTypeMethods.h img/PixelGray8.cpp img/PixelHSL.cpp img/PixelRGB8.cpp img/PixelRGBA8.cpp)