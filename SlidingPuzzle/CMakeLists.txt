project(SlidingPuzzle VERSION 1.0.0 LANGUAGES C)

option(SLIDINGPUZZLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
endif()

add_executable(SlidingPuzzle
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
  src/SlidingPuzzle.c
  src/World.c
  src/World.h
)

if(MSVC)
  target_compile_options(SlidingPuzzle
    PRIVATE
      /W4
  )

  if(SLIDINGPUZZLE_WARNINGS_AS_ERRORS)
    target_compile_options(SlidingPuzzle
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(SlidingPuzzle
    PRIVATE
      -std=c99
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(SLIDINGPUZZLE_WARNINGS_AS_ERRORS)
    target_compile_options(SlidingPuzzle
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(SlidingPuzzle
  PRIVATE
    src
)

target_link_libraries(SlidingPuzzle
  PRIVATE
    WhereAmI::WhereAmI
    zge
)

if(WIN32)
  add_custom_command(
    TARGET
      SlidingPuzzle
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SlidingPuzzle>/assets
  )

  install(
    TARGETS
      SlidingPuzzle
    DESTINATION
      mini-games/SlidingPuzzle
  )
elseif(UNIX)
  set_target_properties(SlidingPuzzle
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY bin
  )

  add_custom_command(
    TARGET
      SlidingPuzzle
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:SlidingPuzzle>/../assets
  )

  install(
    TARGETS
      SlidingPuzzle
    DESTINATION
      mini-games/SlidingPuzzle/bin
  )
endif()

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
  DESTINATION
    mini-games/SlidingPuzzle
)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  DESTINATION
    mini-games/SlidingPuzzle
)
