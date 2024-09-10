#include "../include/ros_gui_n/qrviz.hpp"
#include "QDebug"

qrviz::qrviz(QVBoxLayout* layout)
{
  manager_=new rviz::VisualizationManager(render_panel_);
  layout->addWidget(render_panel_);
  ROS_ASSERT(manager_!=NULL);
  render_panel_->initialize(manager_->getSceneManager(),manager_);
  manager_->initialize();
  manager_->startUpdate();
  manager_->removeAllDisplays();


 }

void qrviz::Set_FixedFrame(QString Frame_name)
{
   manager_->setFixedFrame(Frame_name);
   qDebug()<<manager_->getFixedFrame();
}

void qrviz::Display_Grid(int Cell_Count,QColor color,bool enable)
{
  if(Grid_!=NULL)
  {
    delete Grid_;
    Grid_=NULL;
  }
  Grid_=manager_->createDisplay("rviz/Grid","myGrid",enable);
  Grid_->subProp("Plane Cell Count")->setValue(Cell_Count);
  Grid_->subProp("Color")->setValue(color);
  ROS_ASSERT(Grid_!=NULL);
}

void qrviz::Display_TF(bool enable)
{
    if(TF_!=NULL)
    {
        delete TF_;
        TF_=NULL;
    }
    TF_=manager_->createDisplay("rviz/TF","myTF",enable);
    ROS_ASSERT(TF_!=NULL);
}

void qrviz::Display_LaserScan(QString laser_topic,bool enable)
{
  if(LaserScan_!=NULL)
  {
    delete LaserScan_;
    LaserScan_=NULL;
  }
  LaserScan_=manager_->createDisplay("rviz/LaserScan","myLaserScan",enable);
  LaserScan_->subProp("Topic")->setValue(laser_topic);
  ROS_ASSERT(LaserScan_!=NULL);
}

void qrviz::Display_RobotModel(bool enable)
{
  if(RobotModel_!=NULL)
  {
    delete RobotModel_;
    RobotModel_=NULL;
  }
  RobotModel_=manager_->createDisplay("rviz/RobotModel","myRobotModel",enable);
  ROS_ASSERT(RobotModel_!=NULL);
}



