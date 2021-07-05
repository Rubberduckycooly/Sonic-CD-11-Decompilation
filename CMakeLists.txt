# Debian Buster is still stuck on CMake 3.13, so let's target that.
cmake_minimum_required(VERSION 3.13)

project(
	RSDKv3
	DESCRIPTION "A full decompilation of RSDKv3 & Sonic CD 2011."
	VERSION 1.1.0
	LANGUAGES C CXX
)

add_subdirectory("RSDKv3")
