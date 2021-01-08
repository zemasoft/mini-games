project(Videostop VERSION 1.0.0 LANGUAGES C)

option(VIDEOSTOP_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(UNIX AND NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
endif()

add_executable(Videostop
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
  src/Videostop.c
  src/World.c
  src/World.h
)

if(MSVC)
  target_compile_options(Videostop
    PRIVATE
      /W4
  )

  if(VIDEOSTOP_WARNINGS_AS_ERRORS)
    target_compile_options(Videostop
      PRIVATE
        /WX
    )
  endif()
else()
  target_compile_options(Videostop
    PRIVATE
      -std=c99
      -Wall
      -Wextra
      -Wpedantic
      -Wconversion
      -Wsign-conversion
  )

  if(VIDEOSTOP_WARNINGS_AS_ERRORS)
    target_compile_options(Videostop
      PRIVATE
        -Werror
    )
  endif()
endif()

target_include_directories(Videostop
  PRIVATE
    src
)

target_link_libraries(Videostop
  PRIVATE
    WhereAmI::WhereAmI
    m
    zge
)

if(WIN32)
  add_custom_command(
    TARGET
      Videostop
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Videostop>/assets
  )

  install(
    TARGETS
      Videostop
    DESTINATION
      mini-games/Videostop
    COMPONENT
      Videostop
  )
elseif(UNIX)
  set_target_properties(Videostop
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY bin
  )

  add_custom_command(
    TARGET
      Videostop
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Videostop>/../assets
  )

  install(
    TARGETS
      Videostop
    DESTINATION
      mini-games/Videostop/bin
    COMPONENT
      Videostop
  )
endif()

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
  DESTINATION
    mini-games/Videostop
  COMPONENT
    Videostop
)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/README.md
  DESTINATION
    mini-games/Videostop
  COMPONENT
    Videostop
)
