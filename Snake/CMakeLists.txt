project(Snake VERSION 1.0.0 LANGUAGES C)

option(SNAKE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
endif()

add_executable(Snake
  src/Audio.c
  src/Audio.h
  src/Config.c
  src/Config.h
  src/Graphics.c
  src/Graphics.h
  src/Input.c
  src/Input.h
  src/Logic.c
  src/Logic.h
  src/Snake.c
  src/Tools.c
  src/Tools.h
  src/World.c
  src/World.h
)

if(MSVC)
  target_compile_options(Snake
    PRIVATE
      /W4
  )

  if(SNAKE_WARNINGS_AS_ERRORS)
    target_compile_options(Snake
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(Snake
    PRIVATE
      -std=c99
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(SNAKE_WARNINGS_AS_ERRORS)
    target_compile_options(Snake
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(Snake
  PRIVATE
    src
)

target_link_libraries(Snake
  PRIVATE
    WhereAmI::WhereAmI
    m
    zge
)

if(WIN32)
  add_custom_command(
    TARGET
      Snake
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Snake>/assets
  )

  install(
    TARGETS
      Snake
    DESTINATION
      mini-games/Snake
  )
elseif(UNIX)
  set_target_properties(Snake
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY bin
  )

  add_custom_command(
    TARGET
      Snake
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Snake>/../assets
  )

  install(
    TARGETS
      Snake
    DESTINATION
      mini-games/Snake/bin
  )
endif()

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
  DESTINATION
    mini-games/Snake
)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  DESTINATION
    mini-games/Snake
)
