
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${OneBitRender_SOURCE_DIR}/include/OneBitRender/*.h")

add_library(onebitrender renderer.cpp ${HEADER_LIST})

target_include_directories(onebitrender PUBLIC ../include)

target_compile_features(onebitrender PUBLIC cxx_std_17)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)