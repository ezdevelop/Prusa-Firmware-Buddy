add_library(WUI INTERFACE)

target_sources(
  WUI
  INTERFACE wui.c
            wui_REST_api.c
            netdev.c
            alsockets.c
            wui_api.c
            http/fs.c
            http/handler.c
            http_handler_default.c
            http_lifetime.c
            http/httpd.c
            http/httpd_post.c
            http/multipart_parser.c
            ethernetif.c
            sntp/sntp_client.c
            sntp/sntp.c
  )

set(WEB_FILE "fsdata_wui_local.c")

file(GLOB WEB_FILES ${PROJECT_ROOT_DIR}/lib/WUI/resources/src_local/*.*)
add_custom_target(
  wui-genfile
  DEPENDS ${WEB_FILES}
  COMMAND ${Python3_EXECUTABLE} ${PROJECT_ROOT_DIR}/utils/wui.py ${WEB_FILES} >
          ${CMAKE_BINARY_DIR}/${WEB_FILE}
  COMMENT "Generating fsdata_wui_local raw data from WUI resources."
  )

add_dependencies(WUI wui-genfile)

target_include_directories(
  WUI
  INTERFACE
  INTERFACE ${CMAKE_SOURCE_DIR}/lib/WUI ${CMAKE_SOURCE_DIR}/lib/WUI/http
            ${CMAKE_SOURCE_DIR}/lib/WUI/sntp ${CMAKE_SOURCE_DIR}/lib/WUI/resources
            ${CMAKE_BINARY_DIR}
  )

target_compile_definitions(WUI INTERFACE HTTPD_FSDATA_FILE="${WEB_FILE}")
target_link_libraries(WUI INTERFACE LwIP)
