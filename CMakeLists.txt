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
	include/fix_string.c
	include/fix_string.h
	include/keyboardio.c
	include/keyboardio.h

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

	src/ui/ui/components/component.h
	src/ui/ui/components/header.c
	src/ui/ui/components/header.h
	src/ui/ui/components/option.c
	src/ui/ui/components/option.h
	src/ui/ui/components/text.c
	src/ui/ui/components/text.h
	src/ui/ui/container.c
	src/ui/ui/container.h
	src/ui/ui/selector.c
	src/ui/ui/selector.h
	src/ui/ui/trig.c
	src/ui/ui/trig.h
	src/ui/ui/renderer.c
	src/ui/ui/renderer.h
	src/ui/ui_main.c
	src/ui/ui_main.h
)

target_link_libraries(main m)
