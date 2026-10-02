cmake_minimum_required(VERSION 3.16)

project(mon-calamari-server LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point these at your vendored middleware, or let the toolchain file set them.
if(NOT DEFINED FREERTOS_DIR)
  set(FREERTOS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../FreeRTOS-Kernel" CACHE PATH "FreeRTOS kernel root")
endif()
if(NOT DEFINED LWIP_DIR)
  set(LWIP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lwip" CACHE PATH "LwIP root")
endif()
if(NOT DEFINED PORT_DIR)
  set(PORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/port" CACHE PATH "Board port directory (startup, syscalls, linker script)")
endif()

file(GLOB FIRMWARE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(firmware ${FIRMWARE_SOURCES})

target_include_directories(firmware PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${PORT_DIR}"
  "${FREERTOS_DIR}/include"
  "${LWIP_DIR}/src/include"
)

target_compile_options(firmware PRIVATE
  -Wall -Wextra -Wshadow -Wundef
  $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
)

# LwIP buffers are handed to the FreeRTOS heap allocator.
target_compile_definitions(firmware PRIVATE
  MEMP_MEM_MALLOC=1
  MEM_ALIGNMENT=4
)

set_target_properties(firmware PROPERTIES SUFFIX ".elf")

# ---- optional native host build used for unit testing the helpers ----
option(MON_CALAMARI_HOST_TEST "Build the .cpp sources against host stubs" OFF)
if(MON_CALAMARI_HOST_TEST)
  add_subdirectory(test)
endif()
