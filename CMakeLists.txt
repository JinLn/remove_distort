cmake_minimum_required(VERSION 2.8.3)
project(remove_distort)
add_compile_options(-std=c++11)


find_package(catkin REQUIRED COMPONENTS
        roscpp 
        sensor_msgs
        cv_bridge
        )
find_package(OpenCV REQUIRED)
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES local_map_publisher
#  CATKIN_DEPENDS compute custom_msgs pcl_ros roscpp sensor_msgs visualization_msgs
#  DEPENDS system_lib
)

include_directories( ${catkin_INCLUDE_DIRS}  ${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}_node src/main.cpp
                        )
add_dependencies(${PROJECT_NAME}_node ${catkin_EXPORTED_TARGETS})        
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} 
  )