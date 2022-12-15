include_directories(../view-lib/include)

add_executable(test_window test_window.cpp)
add_executable(test_smart_pointers test_smart_pointers.cpp)
add_executable(test_encoding test_string.cpp ../view-lib/include/utils/encoding.h ../view-lib/src/utils/encoding.cpp)
add_executable(test_list_supported_encodings test_list_supported_encodings.cpp)
add_executable(test_locale test_locale.cpp)
add_executable(test_file test_file.cpp ../view-lib/include/utils/encoding.h ../view-lib/src/utils/encoding.cpp ../view-lib/src/utils/file.cpp)
add_executable(test_static_objects
        test_static_objects/main.cpp
        test_static_objects/other_file.cpp
        test_static_objects/static_object.cpp)

add_executable(test_texture test_texture.cpp ../include/stb_image.h)
target_link_libraries(test_texture ${LIBS})

add_executable(test_compute_shader test_compute_shader.cpp)
target_link_libraries(test_compute_shader view)
add_dependencies(test_compute_shader copy_assets)

add_executable(process_assets
        process_assets.cpp ../include/stb_image.h ../include/stb_image_write.h
        ../view-lib/src/utils/file.cpp ../view-lib/include/utils/file.h test_static_objects/static_object.h)
