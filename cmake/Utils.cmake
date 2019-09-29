function(set_target_compile_options target warnings_as_errors)
  if(MSVC)
    target_compile_options(${target}
      PRIVATE
        /W4
    )

    if(warnings_as_errors)
      target_compile_options(${target}
        PRIVATE
          /WX
      )
    endif()
  else()
    target_compile_options(${target}
      PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
    )

    if(warnings_as_errors)
      target_compile_options(${target}
        PRIVATE
          -Werror
      )
    endif()
  endif()
endfunction()
