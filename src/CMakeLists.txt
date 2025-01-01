find_package(fmt CONFIG REQUIRED)
# imgui links glfw
find_package(imgui CONFIG REQUIRED) 
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)

set(EXE_NAME main)

add_executable(${EXE_NAME}
  main.cpp
  stb.cpp
)

set_target_properties(${EXE_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_EXTENSIONS OFF
)

function(copy_to_target_dir target resource)
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:${target},SOURCE_DIR>/${resource}"
        "$<TARGET_FILE_DIR:${target}>/${resource}"
    )
endfunction()

copy_to_target_dir(${EXE_NAME} shaders)
copy_to_target_dir(${EXE_NAME} res)

target_include_directories(${EXE_NAME} PRIVATE
  ${Stb_INCLUDE_DIR}
)

target_link_libraries(${EXE_NAME} PRIVATE
  fmt::fmt
  imgui::imgui
  glad::glad
)
