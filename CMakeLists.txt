cmake_minimum_required(VERSION 3.20)

Project(json-gen)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
    main

	include/list.c
	include/list.h
	include/vector.c
	include/vector.h
	include/array.c
	include/array.h

	src/main.c

	src/network/packet/packet.c
	src/network/packet/packet.h
	src/network/packet/ping.c
	src/network/packet/ping.h
	src/network/packet/req_resp_balance.c
	src/network/packet/req_resp_balance.h

	src/network/networkio.c
	src/network/networkio.h

	src/network/server/server.c
	src/network/server/server.h
	src/network/client/client.c
	src/network/client/client.h

	src/ui/ui/ui.c
	src/ui/ui/ui.h
	src/ui/ui/renderer.c
	src/ui/ui/renderer.h
	src/ui/ui/controller.c
	src/ui/ui/controller.h
	src/ui/ui_main.c
	src/ui/ui_main.h
)
