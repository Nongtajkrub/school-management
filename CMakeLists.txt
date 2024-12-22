cmake_minimum_required(VERSION 3.20)

Project(json-gen)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
    main

	include/vector.c
	include/vector.h

	src/main.c

	src/network/packet.c
	src/network/packet.h
	src/network/networkio.c
	src/network/networkio.h

	src/network/server/server.c
	src/network/server/server.h
	src/network/client/client.c
	src/network/client/client.h
)
