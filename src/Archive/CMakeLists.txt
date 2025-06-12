add_library(Archive
        Archive.cpp
        Archive.h
)

target_include_directories(Archive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Archive PRIVATE opencv4::opencv4)