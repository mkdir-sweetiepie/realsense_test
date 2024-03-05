/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date February 2011
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/realsense_test/main_window.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace realsense_test
{
using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget* parent) : QMainWindow(parent), qnode(argc, argv)
{
  ui.setupUi(this);  // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.

  setWindowIcon(QIcon(":/images/icon.png"));

  qnode.init();

  QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
  QObject::connect(&qnode, SIGNAL(sigRcvImg()), this, SLOT(slotUpdateImg()));
}

MainWindow::~MainWindow()
{
}

/*****************************************************************************
** Functions
*****************************************************************************/
void MainWindow::slotUpdateImg()
{
  clone_mat = qnode.imgRaw->clone();                                            // 원본 이미지 복사
  cv::resize(clone_mat, clone_mat, cv::Size(640, 360), 0, 0, cv::INTER_CUBIC);  // 이미지 크기 조정
  // cv::cvtColor(clone_mat, gray_clone, cv::COLOR_BGR2GRAY);  // BGR 이미지를 그레이스케일로 변환

  QImage RGB_im((const unsigned char*)(clone_mat.data), clone_mat.cols, clone_mat.rows, QImage::Format_RGB888);
  ui.label->setPixmap(QPixmap::fromImage(RGB_im));

  delete qnode.imgRaw;  // 동적 할당된 원본 이미지 메모리 해제
  qnode.imgRaw = NULL;
  qnode.isreceived = false;  // 이미지 수신 플래그 재설정
}
}  // namespace realsense_test
