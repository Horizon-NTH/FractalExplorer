cmake_minimum_required(VERSION 3.12)

project(FractalExplorer VERSION 1.0.0)

add_subdirectory(deps/HorizonGUI)

file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

if(UNIX)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/
		COMMENT "Moving executable to ${CMAKE_CURRENT_SOURCE_DIR}"
	)
else()
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Debug/${PROJECT_NAME}.exe ${CMAKE_CURRENT_SOURCE_DIR}/
		COMMENT "Moving executable to ${CMAKE_CURRENT_SOURCE_DIR}"
	)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/HorizonGUI/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/HorizonGUI/bin)
target_link_libraries(${PROJECT_NAME} PRIVATE horizongui glfw freetype)
