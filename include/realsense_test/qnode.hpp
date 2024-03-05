/**
 * @file /include/realsense_test/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef realsense_test_QNODE_HPP_
#define realsense_test_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>
#include <QString>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace realsense_test
{
/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread
{
  Q_OBJECT
public:
  QNode(int argc, char** argv);
  virtual ~QNode();
  bool init();
  void run();

  cv::Mat* imgRaw = NULL;                                         // 원본 이미지를 가리키는 포인터
  bool isreceived = false;                                        // 수신 여부를 나타내는 플래그
  image_transport::Subscriber subImage;                           // 서브스크라이버
  void callbackImage(const sensor_msgs::ImageConstPtr& msg_img);  // 이미지 콜백 함수 선언

Q_SIGNALS:
  void rosShutdown();
  void sigRcvImg();
  
private:
  int init_argc;
  char** init_argv;
};

}  // namespace realsense_test

#endif /* realsense_test_QNODE_HPP_ */
