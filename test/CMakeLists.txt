file(
  GLOB CHILDREN
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/*)

include_directories("${CMAKE_SOURCE_DIR}/include")
foreach(CH ${CHILDREN})
  # 拼出绝对路径
  set(CH_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${CH}")
  # 如果是一个目录
  if(IS_DIRECTORY ${CH_PATH})
    # 过滤掉某些特殊目录
    if(NOT CH MATCHES "^\\.git$" AND NOT CH MATCHES "^build$")
      # 检查是否存在 CMakeLists.txt
      if(EXISTS "${CH_PATH}/CMakeLists.txt")
        message(STATUS "  ➜ 添加子目录：${CH}")
        add_subdirectory(${CH})
      else()
        message(STATUS "  ⚠️ 跳过子目录（缺少 CMakeLists.txt）：${CH}")
      endif()
    endif()
  endif()
endforeach()
