cmake_minimum_required(VERSION 2.8)
project( CamSurveillance )
find_package( OpenCV REQUIRED )
find_package(PkgConfig) #finding pkg-config is a helper tool
find_package(Threads REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)

SET ( CMAKE_CXX_FLAGS "-Wall" )

include_directories( ${OpenCV_INCLUDE_DIRS} 
                     ${GLIB_INCLUDE_DIRS}
                     ${GSTREAMER_INCLUDE_DIRS}
                     ${GSTRTSP_INCLUDE_DIRS}
                    )
                     
add_executable( centralized main.cpp )
add_executable( server server.cpp )
add_executable( client client.cpp )


target_link_libraries( centralized ${OpenCV_LIBS} )
# target_link_libraries( centralized ${GSTRTSP_LIBRARIES})
target_link_libraries( server ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries( client ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

