# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)

project("Data Sealing Sample" LANGUAGES C CXX)

find_package(Threads REQUIRED)

find_package(OpenEnclave CONFIG REQUIRED)
add_executable(openenclave::oeedger8r IMPORTED)
set_target_properties(openenclave::oeedger8r PROPERTIES IMPORTED_LOCATION ${OE_BINDIR}/oeedger8r)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/common/datasealing_t.c
  COMMAND openenclave::oeedger8r --trusted ${CMAKE_SOURCE_DIR}/datasealing.edl
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/common)

# TODO: Move to subdirectories after refactoring

# Enclave 1
add_executable(enclave_a_v1
  enc1/ecalls.cpp
  common/dispatcher.cpp
  common/keys.cpp
  common/datasealing_t.c)

target_link_libraries(enclave_a_v1 openenclave::oeenclave openenclave::oelibcxx)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/enc1/private.pem ${CMAKE_SOURCE_DIR}/enc1/public.pem
  COMMAND openssl genrsa -out private.pem -3 3072
  COMMAND openssl rsa -in private.pem -pubout -out public.pem
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/enc1)

add_custom_command(OUTPUT enclave_a_v1.signed
  DEPENDS ${CMAKE_SOURCE_DIR}/enc1/private.pem
  COMMAND openenclave::oesign sign $<TARGET_FILE:enclave_a_v1> ${CMAKE_SOURCE_DIR}/enc1/data-sealing.conf ${CMAKE_SOURCE_DIR}/enc1/private.pem
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Enclave 2
add_executable(enclave_a_v2
  enc2/ecalls.cpp
  common/dispatcher.cpp
  common/keys.cpp
  common/datasealing_t.c)

target_link_libraries(enclave_a_v2 openenclave::oeenclave openenclave::oelibcxx)

add_custom_command(OUTPUT enclave_a_v2.signed
  DEPENDS ${CMAKE_SOURCE_DIR}/enc1/private.pem
  COMMAND openenclave::oesign sign $<TARGET_FILE:enclave_a_v2> ${CMAKE_SOURCE_DIR}/enc2/data-sealing.conf ${CMAKE_SOURCE_DIR}/enc1/private.pem
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Enclave 3
add_executable(enclave_b
  enc3/ecalls.cpp
  common/dispatcher.cpp
  common/keys.cpp
  common/datasealing_t.c)

target_link_libraries(enclave_b openenclave::oeenclave openenclave::oelibcxx)

add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/enc3/private.pem ${CMAKE_SOURCE_DIR}/enc3/public.pem
  COMMAND openssl genrsa -out private.pem -3 3072
  COMMAND openssl rsa -in private.pem -pubout -out public.pem
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/enc3)

add_custom_command(OUTPUT enclave_b.signed
  DEPENDS ${CMAKE_SOURCE_DIR}/enc3/private.pem
  COMMAND openenclave::oesign sign $<TARGET_FILE:enclave_b> ${CMAKE_SOURCE_DIR}/enc3/data-sealing.conf ${CMAKE_SOURCE_DIR}/enc3/private.pem
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

# Host
add_subdirectory(host)

add_custom_target(run
  DEPENDS data-sealing_host ${CMAKE_BINARY_DIR}/enclave_a_v1.signed ${CMAKE_BINARY_DIR}/enclave_a_v2.signed ${CMAKE_BINARY_DIR}/enclave_b.signed
  COMMAND data-sealing_host ${CMAKE_BINARY_DIR}/enclave_a_v1.signed ${CMAKE_BINARY_DIR}/enclave_a_v2.signed ${CMAKE_BINARY_DIR}/enclave_b.signed)
