add_library(lib
	Application.cpp
	Application.h
	Board.cpp
	Board.h
	Image.cpp
	Image.h
	Layer.h
	Player.cpp
	Player.h
	Status.h
	Status.cpp
	Tile.h
	Tile.cpp
)

target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
	lib
PUBLIC
	${GLEW_LIBRARIES}
	OpenGL::GL
	glfw
)

