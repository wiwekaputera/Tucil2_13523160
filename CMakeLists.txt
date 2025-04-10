cmake_minimum_required(VERSION 3.10)

project(Tucil2)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set FreeImage paths manually
set(FREEIMAGE_INCLUDE_DIRS "/usr/include")
set(FREEIMAGE_LIBRARIES "-L/usr/lib -lfreeimage")

# Create executable and add source files
add_executable(tucil2_13523160
    src/main.cpp
    src/modules/imageUtils.cpp
    src/modules/quadTree.cpp
    src/modules/errorMethods.cpp
    src/modules/gif_maker.cpp
)

# Link FreeImage
target_include_directories(tucil2_13523160 PRIVATE ${FREEIMAGE_INCLUDE_DIRS})
target_link_libraries(tucil2_13523160 PRIVATE ${FREEIMAGE_LIBRARIES})