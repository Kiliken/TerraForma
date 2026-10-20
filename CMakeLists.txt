cmake_minimum_required(VERSION 3.15)
project(TerraForma)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Defines
add_compile_definitions(
    RELEASE
)



# Check File to Embed
file(GLOB RESOURCE_OBJECTS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/res/embed/*.o"
)

# Check for Main Files
file(GLOB MAIN_FILES
    src/*.cpp
)

# Executable Files
add_executable(TerraForma	
	# Main
	${MAIN_FILES}
)

# Include Directories
target_include_directories(TerraForma PRIVATE 
    src 
    dep/include
)

# Link Directories
target_link_directories(TerraForma PRIVATE 
    src 
    dep/lib
)

# Link Libraries
target_link_libraries(TerraForma PRIVATE 
    opengl32
	user32
	shell32
	gdi32
	kernel32
	advapi32
	ole32
	oleaut32
	uuid
	odbc32
	odbccp32
)


# Embedding objects
target_sources(TerraForma PRIVATE
    ${RESOURCE_OBJECTS}
)


