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

	src/gui/gui_sys/components/component.h
	src/gui/gui_sys/components/header.c
	src/gui/gui_sys/components/header.h
	src/gui/gui_sys/components/option.c
	src/gui/gui_sys/components/option.h
	src/gui/gui_sys/components/text.c
	src/gui/gui_sys/components/text.h
	src/gui/gui_sys/container.c
	src/gui/gui_sys/container.h
	src/gui/gui_sys/selector.c
	src/gui/gui_sys/selector.h
	src/gui/gui_sys/trig.c
	src/gui/gui_sys/trig.h
	src/gui/gui_sys/renderer.c
	src/gui/gui_sys/renderer.h
	src/gui/gui_sys/menu.c
	src/gui/gui_sys/menu.h
	src/gui/gui_sys/input.c
	src/gui/gui_sys/input.h
	src/gui/gui.c
	src/gui/gui.h
)

target_link_libraries(main m)
