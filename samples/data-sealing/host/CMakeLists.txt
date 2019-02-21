# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/datasealing_u.c
  COMMAND openenclave::oeedger8r --untrusted ${CMAKE_SOURCE_DIR}/datasealing.edl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(data-sealing_host
  host.cpp
  datasealing_u.c)

target_link_libraries(data-sealing_host openenclave::oehostapp)
