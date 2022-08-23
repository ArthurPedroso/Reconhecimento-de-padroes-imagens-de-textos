cmake_minimum_required(VERSION 3.16)
project(ImgProcessingServer)

set (CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

file(GLOB Transmission "Transmission/*.cpp")
file(GLOB ImgProcessing "ImgProcessing/*.cpp")
file(GLOB MainSources "*.cpp")

add_executable( ImgProcessingServer ${MainSources} ${Transmission} ${ImgProcessing})

target_link_libraries(ImgProcessingServer ${OpenCV_LIBS} -llept -ltesseract -pthread)