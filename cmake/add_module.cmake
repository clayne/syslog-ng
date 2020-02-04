#############################################################################
# Copyright (c) 2020 One Identity
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# As an additional exemption you are allowed to compile & link against the
# OpenSSL libraries as published by the OpenSSL project. See the file
# COPYING for details.
#
#############################################################################


include(CMakeParseArguments)

function (add_module)

  cmake_parse_arguments(ADD_MODULE "" "TARGET" "GRAMMAR;SOURCES;DEPENDS;INCLUDES" ${ARGN})

  if (ADD_MODULE_GRAMMAR)
    generate_y_from_ym("modules/${ADD_MODULE_TARGET}/${ADD_MODULE_GRAMMAR}")
    bison_target(${ADD_MODULE_TARGET}Grammar}
      ${CMAKE_CURRENT_BINARY_DIR}/${ADD_MODULE_GRAMMAR}.y
      ${CMAKE_CURRENT_BINARY_DIR}/${ADD_MODULE_GRAMMAR}.c
      COMPILE_FLAGS ${BISON_FLAGS}
      )
  endif()

  add_library(${ADD_MODULE_TARGET} SHARED ${ADD_MODULE_SOURCES})
  target_include_directories(${ADD_MODULE_TARGET}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE ${ADD_MODULE_INCLUDES})
  target_link_libraries(${ADD_MODULE_TARGET} PRIVATE ${ADD_MODULE_DEPENDS} syslog-ng)
  install(TARGETS ${ADD_MODULE_TARGET} LIBRARY DESTINATION lib/syslog-ng COMPONENT ${ADD_MODULE_TARGET})

endfunction ()

