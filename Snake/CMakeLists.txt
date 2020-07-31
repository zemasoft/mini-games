project(Snake VERSION 1.0.0 LANGUAGES C)

option(SNAKE_USE_FREEALUT "Use FreeALUT" ON)
option(SNAKE_USE_FREEGLUT "Use FreeGLUT" OFF)
option(SNAKE_USE_GLFW "Use GLFW" ON)
option(SNAKE_USE_SDL2 "Use SDL2" OFF)

option(SNAKE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

find_package(WhereAmI MODULE REQUIRED)

if(SNAKE_USE_FREEALUT)
  find_package(FreeALUT MODULE REQUIRED)
endif()

if(SNAKE_USE_FREEGLUT)
  find_package(FreeGLUT MODULE REQUIRED)
endif()

if(SNAKE_USE_GLFW)
  find_package(GLFW MODULE REQUIRED)
endif()

if(SNAKE_USE_SDL2)
  find_package(SDL2 MODULE REQUIRED)
endif()

if(UNIX)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../../lib)
endif()

add_executable(Snake
  src/Config.c
  src/Graphics.c
  src/Input.c
  src/Logic.c
  src/Snake.c
  src/Sound.c
  src/Tools.c
  src/World.c

  src/Config.h
  src/Graphics.h
  src/Input.h
  src/Logic.h
  src/Sound.h
  src/Tools.h
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
)

if(WIN32)
  add_custom_command(
    TARGET
      Snake
    PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Snake>/assets
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
    PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Snake>/../assets
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

if(SNAKE_USE_FREEALUT)
  target_compile_definitions(Snake
    PRIVATE
      USE_FREEALUT
  )

  target_link_libraries(Snake
    PRIVATE
      FreeALUT::FreeALUT
  )

  if(WIN32)
    install(
      FILES
        ${FreeALUT_LIBRARY}
      DESTINATION
        mini-games/Snake
    )
  elseif(UNIX)
    file(GLOB FreeALUT_LIBRARY_AND_LINKS "${FreeALUT_LIBRARY}.*")

    install(
      FILES
        ${FreeALUT_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
endif()

if(SNAKE_USE_FREEGLUT)
  target_compile_definitions(Snake
    PRIVATE
      USE_FREEGLUT
  )

  target_link_libraries(Snake
    PRIVATE
      FreeGLUT::FreeGLUT
  )

  if(WIN32)
    install(
      FILES
        ${FreeGLUT_LIBRARY}
      DESTINATION
        mini-games/Snake
    )
  elseif(UNIX)
    file(GLOB FreeGLUT_LIBRARY_AND_LINKS "${FreeGLUT_LIBRARY}.*")

    install(
      FILES
        ${FreeGLUT_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
endif()

if(SNAKE_USE_GLFW)
  target_compile_definitions(Snake
    PRIVATE
      USE_GLFW
  )

  target_link_libraries(Snake
    PRIVATE
      GLFW::GLFW
  )

  if(WIN32)
    install(
      FILES
        ${GLFW_LIBRARY}
      DESTINATION
        mini-games/Snake
    )
  elseif(UNIX)
    file(GLOB GLFW_LIBRARY_AND_LINKS "${GLFW_LIBRARY}.*")

    install(
      FILES
        ${GLFW_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
endif()

if(SNAKE_USE_SDL2)
  target_compile_definitions(Snake
    PRIVATE
      USE_SDL2
  )

  target_link_libraries(Snake
    PRIVATE
      SDL2::SDL2
  )

  if(WIN32)
    install(
      FILES
        ${SDL2_LIBRARY}
      DESTINATION
        mini-games/Snake
    )
  elseif(UNIX)
    file(GLOB SDL2_LIBRARY_AND_LINKS "${SDL2_LIBRARY}.*")

    install(
      FILES
        ${SDL2_LIBRARY_AND_LINKS}
      DESTINATION
        mini-games/lib
    )
  endif()
endif()
