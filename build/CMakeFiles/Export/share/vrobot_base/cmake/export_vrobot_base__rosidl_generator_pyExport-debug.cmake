#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vrobot_base::vrobot_base__rosidl_generator_py" for configuration "Debug"
set_property(TARGET vrobot_base::vrobot_base__rosidl_generator_py APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(vrobot_base::vrobot_base__rosidl_generator_py PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libvrobot_base__rosidl_generator_py.so"
  IMPORTED_SONAME_DEBUG "libvrobot_base__rosidl_generator_py.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS vrobot_base::vrobot_base__rosidl_generator_py )
list(APPEND _IMPORT_CHECK_FILES_FOR_vrobot_base::vrobot_base__rosidl_generator_py "${_IMPORT_PREFIX}/lib/libvrobot_base__rosidl_generator_py.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
