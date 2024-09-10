#ifndef QRVIZ_HPP
#define QRVIZ_HPP

#include <QObject>
#include <ros/ros.h>
#include <rviz/visualization_manager.h>
#include <rviz/render_panel.h>
#include <rviz/display.h>
#include <rviz/tool_manager.h>
#include <rviz/tool.h>
//#include "grid_display.h"
#include <QVBoxLayout>


class qrviz
{
public:
  qrviz(QVBoxLayout* layout);
  void Set_FixedFrame(QString Frame_name);
  void Display_Grid(int Cell_Count,QColor color,bool enable);
  void Display_TF(bool enable);
  void Display_LaserScan(QString laser_topic,bool enable);
  void Display_RobotModel(bool enable);
private:
  rviz::RenderPanel* render_panel_=new rviz::RenderPanel();
  rviz::VisualizationManager* manager_;
  rviz::Display* Grid_=NULL;
  rviz::Display* TF_=NULL;
  rviz::Display* LaserScan_=NULL;
  rviz::Display* RobotModel_=NULL;
};

#endif // QRVIZ_HPP
