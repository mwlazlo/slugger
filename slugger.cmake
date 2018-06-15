

# --- Because we are using cmake 2.6 we have to derive CMAKE_CURRENT_LIST_DIR from CMAKE_CURRENT_LIST_SRC
STRING(LENGTH ${CMAKE_CURRENT_LIST_FILE} strlen)
STRING(LENGTH "slugger.cmake" strlen_postfix)
math(EXPR strlen_postfix ${strlen}-${strlen_postfix})
#message("${CMAKE_CURRENT_LIST_FILE} ${strlen_postfix} ${strlen}")
STRING(SUBSTRING ${CMAKE_CURRENT_LIST_FILE} 0 ${strlen_postfix} CMAKE_CURRENT_LIST_DIR)
#message("${ttclasses_dir}")
# ===

include_directories ("${CMAKE_CURRENT_LIST_DIR}")

FILE(GLOB slugger_log_src "${CMAKE_CURRENT_LIST_DIR}/slugger/*.cpp" )

ADD_LIBRARY( slugger_log STATIC EXCLUDE_FROM_ALL ${slugger_log_src} )
ADD_LIBRARY( slugger_config STATIC EXCLUDE_FROM_ALL ${slugger_config_src} )
