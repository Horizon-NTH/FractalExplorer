cmake_minimum_required(VERSION 3.12)

option(BUILD_EXTERNAL_LIB "Choose to build or no external api" OFF)

project(FractalExplorer VERSION 1.0.0)

if(BUILD_EXTERNAL_LIB)
	add_subdirectory(deps/HorizonGUI)
endif()

file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB_RECURSE SHADER_FILES assets/shaders/*.gsls)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

if(UNIX)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/
		COMMENT "Moving executable to ${CMAKE_CURRENT_SOURCE_DIR}"
	)
	target_link_libraries(${PROJECT_NAME} PRIVATE horizongui glfw freetype)
else()
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.exe ${CMAKE_CURRENT_SOURCE_DIR}/
		COMMENT "Moving executable to ${CMAKE_CURRENT_SOURCE_DIR}"
	)
	target_link_libraries(${PROJECT_NAME} PRIVATE horizongui glfw3 freetyped)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/HorizonGUI/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/HorizonGUI/bin)
