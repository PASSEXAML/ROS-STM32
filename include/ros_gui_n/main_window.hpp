/**
 * @file /include/ros_gui_n/main_window.hpp
 *
 * @brief Qt based gui for ros_gui_n.
 *
 * @date November 2010
 **/
#ifndef ros_gui_n_MAIN_WINDOW_H
#define ros_gui_n_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QComboBox>
#include "qrviz.hpp"
#include <QSpinBox>
#include "CCtrlDashBoard.h"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace ros_gui_n {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void on_actionAbout_triggered();
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);
  void on_btn_connect_laser();
  void on_btn_connect_laser_1();
  void on_urdf();
  void on_camera();

    /******************************************
    ** Manual connections
    *******************************************/
    void updateLoggingView(); // no idea why this can't connect automatically
    void slto_update_image(QImage);
    void slot_sub_image();
    void slot_treeWidget_value_change(QString);
    void slot_display_grid(int);
    void slot_display_tf(int);
    void slot_display_laser(int);
    void slot_display_RobotModel(int);
    void slot_display_Map(int);
    void slot_display_Path(int);
    void slot_update_dashboard(float,float);
    void slot_linera_value_change(int);
    void slot_raw_value_change(int);
    void slot_pushbtn_click();
private:
	Ui::MainWindowDesign ui;
  CCtrlDashBoard* speed_x_dashBoard;
  CCtrlDashBoard* speed_y_dashBoard;
	QNode qnode;
  qrviz* myrviz;
  QComboBox* fixed_box;
  QComboBox* Grid_Color_Box;
  QSpinBox* Cell_Count_Box;
  QComboBox* Laser_Topic_box;
  QComboBox* Map_Topic_box;
  QComboBox* Map_Color_Scheme_box;
  QComboBox* Path_Color_box;
  QComboBox* Path_Topic_box;
};

}  // namespace ros_gui_n

#endif // ros_gui_n_MAIN_WINDOW_H
