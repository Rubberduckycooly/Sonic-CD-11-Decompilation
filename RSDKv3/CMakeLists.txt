# pkg-config is inconvenient on Windows, perhaps we could use vcpkg there?
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(VORBIS REQUIRED vorbis)
pkg_search_module(VORBISFILE REQUIRED vorbisfile)
pkg_search_module(THEORADEC REQUIRED theoradec)

set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(THEORAPLAY_DIR "${DEPENDENCIES_DIR}/all/theoraplay")
set(GHC_FILESYSTEM_DIR "${DEPENDENCIES_DIR}/all/filesystem")

# Theoraplay
add_library(
	theoraplay
	STATIC
	"${THEORAPLAY_DIR}/theoraplay_cvtrgb.h"
	"${THEORAPLAY_DIR}/theoraplay.h"
	"${THEORAPLAY_DIR}/theoraplay.c"
)
target_include_directories(theoraplay PUBLIC "${THEORAPLAY_DIR}")
target_link_libraries(
	theoraplay
	PRIVATE Threads::Threads
	PRIVATE ${VORBIS_LIBRARIES}
	PRIVATE ${THEORADEC_LIBRARIES}
)

# Gulrak Filesystem
add_subdirectory(
	"${GHC_FILESYSTEM_DIR}"
	"${GHC_FILESYSTEM_DIR}/build"
)

# RSDKv3
add_executable(
	soniccd
	Animation.hpp Animation.cpp
	Audio.hpp Audio.cpp
	Collision.hpp Collision.cpp
	Debug.hpp Debug.cpp
	Drawing.hpp Drawing.cpp
	fcaseopen.h fcaseopen.c
	Ini.hpp Ini.cpp
	Input.hpp Input.cpp
	main.cpp
	Math.hpp Math.cpp
	Object.hpp Object.cpp
	Palette.hpp Palette.cpp
	Player.hpp Player.cpp
	Reader.hpp Reader.cpp
	resource.h
	RetroEngine.hpp RetroEngine.cpp
	Scene3D.hpp Scene3D.cpp
	Scene.hpp Scene.cpp
	Script.hpp Script.cpp
	Sprite.hpp Sprite.cpp
	String.hpp String.cpp
	Text.hpp Text.cpp
	Userdata.hpp Userdata.cpp
	Video.hpp Video.cpp
)
target_include_directories(soniccd PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(
	soniccd
	PRIVATE theoraplay
	PRIVATE ghc_filesystem
	PRIVATE ${SDL2_LIBRARIES}
	PRIVATE ${VORBISFILE_LIBRARIES}
)
