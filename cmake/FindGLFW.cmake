find_package(OpenGL MODULE REQUIRED)

set(GLFW_ROOT
  $ENV{DEPS_DIR}
  CACHE
  PATH
  "GLFW root directory"
)

find_library(GLFW_LIBRARY
  NAMES
    glfw
  PATHS
    ${GLFW_ROOT}
  PATH_SUFFIXES
    lib
  NO_DEFAULT_PATH
)

mark_as_advanced(GLFW_LIBRARY)

find_path(GLFW_INCLUDE_DIR
  NAMES
    .
  PATHS
    ${GLFW_ROOT}
  PATH_SUFFIXES
    include
  NO_DEFAULT_PATH
)

mark_as_advanced(GLFW_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW
  DEFAULT_MSG
  GLFW_LIBRARY
  GLFW_INCLUDE_DIR
)

if(GLFW_FOUND AND NOT TARGET GLFW::GLFW)
  add_library(GLFW::GLFW
    UNKNOWN
    IMPORTED
  )

  set_target_properties(GLFW::GLFW
    PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES C
      INTERFACE_INCLUDE_DIRECTORIES "${OPENGL_INCLUDE_DIR};${GLFW_INCLUDE_DIR}"
  )

  set_target_properties(GLFW::GLFW
    PROPERTIES
      IMPORTED_LOCATION ${GLFW_LIBRARY}
      IMPORTED_LINK_INTERFACE_LIBRARIES ${OPENGL_LIBRARY}
  )
endif()
