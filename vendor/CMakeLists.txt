project(vendor C CXX)

set(spdlog_PATH "${CMAKE_SOURCE_DIR}/vendor/spdlog")
if(NOT TARGET spdlog)
  message("-- adding spdlog")
  add_subdirectory("${spdlog_PATH}")
endif()

