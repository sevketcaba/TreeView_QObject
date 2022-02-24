set(COMMON_DIR ${COMMON_DIR} PARENT_SCOPE)
set(LIBS ${LIBS} PARENT_SCOPE)

set(LIBRARY_NAME TreeView_Model)
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/treemodel.cpp
)
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/treemodel.h
)

add_library(${LIBRARY_NAME}
    SHARED
${HEADER_FILES}
${SOURCE_FILES}
)

list(APPEND
    LIBS
    ${LIBRARY_NAME}
)

target_include_directories(
    ${LIBRARY_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${COMMON_DIR}
)

target_link_libraries(
    ${LIBRARY_NAME}
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
)

set(LIBS ${LIBS} PARENT_SCOPE)
