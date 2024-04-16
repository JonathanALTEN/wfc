# CMakeLists.txt in src/wfc

# Define the library target
add_library(wfc INTERFACE)

# Include the directory containing the header file
target_include_directories(wfc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/wfc)
