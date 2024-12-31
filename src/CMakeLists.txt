find_package(fmt CONFIG REQUIRED)
# imgui links glfw
find_package(imgui CONFIG REQUIRED) 
find_package(glad CONFIG REQUIRED)

set(EXE_NAME main)

add_executable(${EXE_NAME}
  main.cpp
)

set_target_properties(${EXE_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_EXTENSIONS OFF
)

function(copy_shaders_to_target_dir target)
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:${target},SOURCE_DIR>/shaders"
        "$<TARGET_FILE_DIR:${target}>/shaders"
    )
endfunction()

copy_shaders_to_target_dir(${EXE_NAME})

target_link_libraries(${EXE_NAME} PRIVATE
  fmt::fmt
  imgui::imgui
  glad::glad
)
