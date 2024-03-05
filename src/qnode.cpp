/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date February 2011
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include "../include/realsense_test/qnode.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace realsense_test
{
/*****************************************************************************
** Implementation
*****************************************************************************/

QNode::QNode(int argc, char** argv) : init_argc(argc), init_argv(argv)
{
}

QNode::~QNode()
{
  if (ros::isStarted())
  {
    ros::shutdown();  // explicitly needed since we use ros::start();
    ros::waitForShutdown();
  }
  wait();
}

bool QNode::init()
{
  ros::init(init_argc, init_argv, "realsense_test");
  if (!ros::master::check())
  {
    return false;
  }
  ros::start();  // explicitly needed since our nodehandle is going out of scope.
  ros::NodeHandle n;

  // Add your ros communications here.
  image_transport::ImageTransport image(n);  // 이미지 전송을 위한 ImageTransport 객체 생성
  subImage = image.subscribe("/camera/color/image_raw", 1, &QNode::callbackImage, this);  // 서브스크라이버
  
  start();
  return true;
}

void QNode::run()
{
  ros::Rate loop_rate(33);
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
  std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
  Q_EMIT rosShutdown();  // used to signal the gui for a shutdown (useful to roslaunch)
}

void QNode::callbackImage(const sensor_msgs::ImageConstPtr& msg_img)
{
  if (imgRaw == NULL && !isreceived)  // imgRaw -> NULL, isreceived -> false
  {
    // ROS 이미지 메시지를 OpenCV Mat 형식으로 변환, 이미지 객체에 할당
    imgRaw = new cv::Mat(cv_bridge::toCvCopy(msg_img, sensor_msgs::image_encodings::RGB8)->image);

    if (imgRaw != NULL)  // imgRaw 변환 성공
    {
      Q_EMIT sigRcvImg();  // 이미지 수신을 알리는 시그널 발생
      isreceived = true;
    }
  }
}

}  // namespace realsense_test
