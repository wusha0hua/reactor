project(reactor)
aux_source_directory(. source_files)
set(CMAKE_BUILD_TYPE "Debug")
add_executable(reactor ${source_files})
