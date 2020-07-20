find_package(OpenGL MODULE REQUIRED)

set(FreeGLUT_ROOT
  $ENV{DEPS_DIR}
  CACHE
  PATH
  "FreeGLUT root directory"
)

find_library(FreeGLUT_LIBRARY
  NAMES
    freeglut
  PATHS
    ${FreeGLUT_ROOT}
  PATH_SUFFIXES
    lib
  NO_DEFAULT_PATH
)

mark_as_advanced(FreeGLUT_LIBRARY)

find_path(FreeGLUT_INCLUDE_DIR
  NAMES
    .
  PATHS
    ${FreeGLUT_ROOT}
  PATH_SUFFIXES
    include
  NO_DEFAULT_PATH
)

mark_as_advanced(FreeGLUT_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FreeGLUT
  DEFAULT_MSG
  FreeGLUT_LIBRARY
  FreeGLUT_INCLUDE_DIR
)

if(FreeGLUT_FOUND AND NOT TARGET FreeGLUT::FreeGLUT)
  add_library(FreeGLUT::FreeGLUT
    UNKNOWN
    IMPORTED
  )

  set_target_properties(FreeGLUT::FreeGLUT
    PROPERTIES
      IMPORTED_LINK_INTERFACE_LANGUAGES C
      INTERFACE_INCLUDE_DIRECTORIES "${OPENGL_INCLUDE_DIR};${FreeGLUT_INCLUDE_DIR}"
  )

  set_target_properties(FreeGLUT::FreeGLUT
    PROPERTIES
      IMPORTED_LOCATION ${FreeGLUT_LIBRARY}
      IMPORTED_LINK_INTERFACE_LIBRARIES ${OPENGL_LIBRARY}
  )
endif()
