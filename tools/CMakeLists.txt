add_executable(test_window test_window.cpp)
add_executable(test_smart_pointers test_smart_pointers.cpp)
add_executable(test_encoding test_string.cpp ../src/utils/encoding.h ../src/utils/encoding.cpp)
add_executable(test_list_supported_encodings test_list_supported_encodings.cpp)
add_executable(test_locale test_locale.cpp)
add_executable(test_file test_file.cpp ../src/utils/encoding.h ../src/utils/encoding.cpp)

add_executable(test_texture test_texture.cpp ../src/stb_image.h)
target_link_libraries(test_texture ${LIBS})

add_executable(process_assets
        process_assets.cpp ../src/stb_image.h ../src/stb_image_write.h
        ../src/utils/file.cpp ../src/utils/file.h)