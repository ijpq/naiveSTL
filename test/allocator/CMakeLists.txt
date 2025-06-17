# =======================================================
# 子目录 CMakeLists.txt 模板（放在例：example1/ 或 example2/ 中）
# =======================================================

# ----------------------------------------------------------------------------
# 1. 获取当前子目录名，作为目标名字（TARGET_NAME）
# ----------------------------------------------------------------------------
# CMAKE_CURRENT_SOURCE_DIR 是该 CMakeLists.txt 所在的绝对路径 get_filename_component(... NAME)
# 会把最后一级目录名提取出来，比如 "example1"
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# message(WARNING "${TARGET_NAME}")
# ----------------------------------------------------------------------------
# 1. 收集当前目录下的所有 .cpp 源文件（不递归子文件夹）
# ----------------------------------------------------------------------------
# file(GLOB SOURCES *.cpp) 会把本目录下的所有后缀为 .cpp 的文件都收集到 SOURCES 变量里
file(GLOB SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# ----------------------------------------------------------------------------
# 1. 如果当前目录下根本没有 .cpp 文件，就什么都不做
# ----------------------------------------------------------------------------
if(SOURCES)
  # 生成一个可执行文件，名字为 ${TARGET_NAME}，源文件列表为 ${SOURCES}
  add_executable(${TARGET_NAME} ${SOURCES})
  # ----------------------------------------------------------------------------
  # 1. （可选）为该可执行文件指定额外的 include 路径、链接库、编译选项等
  # ----------------------------------------------------------------------------
  # 例如，如果该子模块需要链接某个第三方库，比如 pthread： target_link_libraries(${TARGET_NAME} PRIVATE pthread)
  #
  # 如果需要为该目标单独添加 include 路径（相对于项目根目录）： target_include_directories(${TARGET_NAME} PRIVATE
  # ${CMAKE_SOURCE_DIR}/some_common_include)
  #
  # 如果希望为该子模块开启某些编译警告或优化： target_compile_options(${TARGET_NAME} PRIVATE -Wall -O2)
  #
  # 若要指定 C++ 标准与根目录保持一致，可以显式写： set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17
  # CXX_STANDARD_REQUIRED ON )

  # ----------------------------------------------------------------------------
  # 1. 也可以根据需求，把该子目录编译出来的可执行文件放到指定输出目录
  # ----------------------------------------------------------------------------
  # 例如，把所有子目录生成的可执行都放到根目录的 bin/ 下： set_target_properties(${TARGET_NAME} PROPERTIES
  # RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
endif()
