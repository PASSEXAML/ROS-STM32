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
#include "../include/ros_gui_n/main_window.hpp"

/***************************************************************************
** Namespaces
*****************************************************************************/

namespace ros_gui_n {

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(int argc, char** argv, QWidget *parent)
	: QMainWindow(parent)
	, qnode(argc,argv)
{
	ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
  ui.treeWidget->setHeaderLabels(QStringList()<<"key"<<"value");
  /*********************
  ** Auto Start
  **********************/
  if ( ui.checkbox_remember_settings->isChecked() ) {
      on_button_connect_clicked(true);
  }

  //connect
  QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application
  QObject::connect(&qnode, SIGNAL(rosShutdown()), this, SLOT(close()));
  connect(ui.horizontalSlider_linera,SIGNAL(valueChanged(int)),this,SLOT(slot_linera_value_change(int)));
  connect(ui.horizontalSlider_raw,SIGNAL(valueChanged(int)),this,SLOT(slot_raw_value_change(int)));
  connect(ui.pushButton_i,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_o,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_u,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_j,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_l,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_n,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_m,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(ui.pushButton_b,SIGNAL(clicked()),this,SLOT(slot_pushbtn_click()));
  connect(&qnode,SIGNAL(image_val(QImage)),this,SLOT(slto_update_image(QImage)));
  connect(ui.pushButton_image,SIGNAL(clicked()),this,SLOT(slot_sub_image()));
  connect(ui.btn_laser,SIGNAL(clicked()),this,SLOT(on_btn_connect_laser()));
  connect(ui.btn_laser_1,SIGNAL(clicked()),this,SLOT(on_btn_connect_laser_1()));
  connect(ui.urdf,SIGNAL(clicked()),this,SLOT(on_urdf()));
  connect(ui.camera_launch_btn,SIGNAL(clicked()),this,SLOT(on_camera()));

  //ui init
  speed_x_dashBoard=new CCtrlDashBoard(ui.widget_speed_x);
  speed_y_dashBoard=new CCtrlDashBoard(ui.widget_speed_y);
  speed_x_dashBoard->setGeometry(ui.widget_speed_x->rect());
  speed_y_dashBoard->setGeometry(ui.widget_speed_y->rect());
  speed_x_dashBoard->setValue(0);
  speed_y_dashBoard->setValue(0);
  ui.lineEdit_image->setText("/usb_cam/image_raw");
  //connect
  connect(&qnode,SIGNAL(speed_vel(float,float)),this,SLOT(slot_update_dashboard(float,float)));
    ReadSettings();
	setWindowIcon(QIcon(":/images/icon.png"));
	ui.tab_manager->setCurrentIndex(0); // ensure the first tab is showing - qt-designer should have this already hardwired, but often loses it (settings?).
  QTreeWidgetItem* Global=new QTreeWidgetItem(QStringList()<<"Global Options");
  Global->setIcon(0,QIcon("://1.jpg"));
  ui.treeWidget->addTopLevelItem(Global);
  Global->setExpanded(true);

  QTreeWidgetItem* Fixed_frame=new QTreeWidgetItem(QStringList()<<"Fixed Frame");
  fixed_box=new QComboBox();
  fixed_box->addItem("map");
  fixed_box->setMaximumWidth(150);
  fixed_box->setEditable(true);
  connect(fixed_box,SIGNAL(currentTextChanged(QString)),this,SLOT(slot_treeWidget_value_change(QString)));

  Global->addChild(Fixed_frame);
  ui.treeWidget->setItemWidget(Fixed_frame,1,fixed_box);

  //Grid
  QTreeWidgetItem* Grid=new QTreeWidgetItem(QStringList()<<"Grid");
  Grid->setIcon(0,QIcon("://2.jpg"));
  //checkbox
  QCheckBox* Grid_Check=new QCheckBox();
  connect(Grid_Check,SIGNAL(stateChanged(int)),this,SLOT(slot_display_grid(int)));
  ui.treeWidget->addTopLevelItem(Grid);
  ui.treeWidget->setItemWidget(Grid,1,Grid_Check);
  Grid->setExpanded(true);

  //Cell Count
  QTreeWidgetItem* Cell_Count=new QTreeWidgetItem(QStringList()<<"Plane Cell Count");
  Grid->addChild(Cell_Count);
  //add SpinBox
  Cell_Count_Box=new QSpinBox();
  Cell_Count_Box->setValue(13);
  ui.treeWidget->setItemWidget(Cell_Count,1,Cell_Count_Box);
  //add Color
  QTreeWidgetItem* Grid_Color=new QTreeWidgetItem(QStringList()<<"Color");
  Grid->addChild(Grid_Color);
  Grid_Color_Box=new QComboBox();
  Grid_Color_Box->addItem("160;160;160");
  Grid_Color_Box->setEditable(true);
  ui.treeWidget->setItemWidget(Grid_Color,1,Grid_Color_Box);

  //TF ui
  QTreeWidgetItem* TF=new QTreeWidgetItem(QStringList()<<"TF");
  TF->setIcon(0,QIcon("://3.jpg"));
  QCheckBox* TF_Check=new QCheckBox();
  connect(TF_Check,SIGNAL(stasteChanged(int)),this,SLOT(slot_display_tf(int)));
  ui.treeWidget->addTopLevelItem(TF);
  ui.treeWidget->setItemWidget(TF,1,TF_Check);

  //LaserScan
  QTreeWidgetItem* LaserScan=new QTreeWidgetItem(QStringList()<<"LaserScan");
  LaserScan->setIcon(0,QIcon("://4.jpg"));
  QCheckBox* Laser_Check=new QCheckBox();
  connect(Laser_Check,SIGNAL(stateChanged(int)),this,SLOT(slot_display_laser(int)));
  ui.treeWidget->addTopLevelItem(LaserScan);
  ui.treeWidget->setItemWidget(LaserScan,1,Laser_Check);
  QTreeWidgetItem* LaserTopic=new QTreeWidgetItem(QStringList()<<"Topic");
  Laser_Topic_box=new QComboBox();
  Laser_Topic_box->addItem("/scan");
  Laser_Topic_box->setEditable(true);
  LaserScan->addChild(LaserTopic);
  ui.treeWidget->setItemWidget(LaserTopic,1,Laser_Topic_box);

  //RobotModel
  QTreeWidgetItem* RobotModel=new QTreeWidgetItem(QStringList()<<"RobotModel");
  RobotModel->setIcon(0,QIcon("://5.jpg"));
  QCheckBox* RobotModel_Check=new QCheckBox();
  connect(RobotModel_Check,SIGNAL(stateChanged(int)),this,SLOT(slot_display_RobotModel(int)));
  ui.treeWidget->addTopLevelItem(RobotModel);
  ui.treeWidget->setItemWidget(RobotModel,1,RobotModel_Check);

  /*********************
   *
	** Logging
	**********************/
	ui.view_logging->setModel(qnode.loggingModel());
    QObject::connect(&qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));

}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/
void MainWindow::slot_pushbtn_click(){
  QPushButton* btn=qobject_cast<QPushButton*>(sender());
  char k=btn->text().toStdString()[0];
  bool is_all=ui.checkBox->isChecked();
  float linear=ui.label_linera->text().toFloat()*0.01;
  float angular=ui.label_raw->text().toFloat()*0.01;
  switch(k){
  case 'i':
    qnode.set_cmd_vel(is_all?'I':'i',linear,angular);
    break;
  case 'u':
    qnode.set_cmd_vel(is_all?'U':'u',linear,angular);
    break;
  case 'j':
    qnode.set_cmd_vel(is_all?'J':'j',linear,angular);
  case 'l':
    qnode.set_cmd_vel(is_all?'L':'l',linear,angular);
    break;
  case 'o':
    qnode.set_cmd_vel(is_all?'O':'o',linear,angular);
    break;
  case 'm':
    qnode.set_cmd_vel(is_all?'M':'m',linear,angular);
    break;
  case ',':
    qnode.set_cmd_vel(is_all?'<':',',linear,angular);
    break;
  case '.':
    qnode.set_cmd_vel(is_all?'>':'.',linear,angular);
    break;
  }
}

void MainWindow::slot_linera_value_change(int value){
  ui.label_linera->setText(QString::number(value));
}

void MainWindow::slot_raw_value_change(int value){
  ui.label_raw->setText(QString::number(value));
}

void MainWindow::slot_update_dashboard(float x,float y)
{
  ui.label_6->setText(y>0?"正向":"反向");
  ui.label_7->setText(x>0?"正向":"反向");
  speed_x_dashBoard->setValue(abs(x*100));
  speed_y_dashBoard->setValue(abs(y*100));
}

void MainWindow::slot_display_RobotModel(int state)
{
    bool enable=state>1?true:false;
    myrviz->Display_RobotModel(enable);
}

void MainWindow::slot_display_laser(int state)
{
    bool enable=state>1?true:false;
    myrviz->Display_LaserScan(Laser_Topic_box->currentText(),enable);
}

void MainWindow::slot_display_tf(int state)
{
    bool enable=state>1?true:false;
    myrviz->Display_TF(enable);
}

void MainWindow::slot_display_grid(int state)
{
  bool enable=state>1?true:false;
  QStringList qli=Grid_Color_Box->currentText().split(";");
  QColor color=QColor(qli[0].toInt(),qli[1].toInt(),qli[2].toInt());
  myrviz->Display_Grid(Cell_Count_Box->text().toInt(),color,enable);
}

void MainWindow::on_camera(){
  system("gnome-terminal -x bash -c '~/thrid.sh'&");
}

void MainWindow::on_urdf(){
  system("gnome-terminal -x bash -c 'roslaunch robot_description display_robot_urdf.launch'&");
}

void MainWindow::on_btn_connect_laser(){
  system("gnome-terminal -x bash -c '~/first.sh'&");
}

void MainWindow::on_btn_connect_laser_1(){
  system("gnome-terminal -x bash -c '~/second.sh'&");
}

void MainWindow::slto_update_image(QImage im)
{
  ui.label_image->setPixmap(QPixmap::fromImage(im));
}

void MainWindow::slot_sub_image()
{
  qnode.sub_image(ui.lineEdit_image->text());
}

void MainWindow::showNoMasterMessage() {
	QMessageBox msgBox;
	msgBox.setText("Couldn't find the ros master.");
	msgBox.exec();
    close();
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */

void MainWindow::on_button_connect_clicked(bool check) {
	if ( ui.checkbox_use_environment->isChecked() ) {
		if ( !qnode.init() ) {
			showNoMasterMessage();
      ui.treeWidget->setEnabled(false);
		} else {
			ui.button_connect->setEnabled(false);
      ui.treeWidget->setEnabled(true);
      myrviz=new qrviz(ui.layout_rviz);
		}
	} else {
		if ( ! qnode.init(ui.line_edit_master->text().toStdString(),
				   ui.line_edit_host->text().toStdString()) ) {
			showNoMasterMessage();
		} else {
			ui.button_connect->setEnabled(false);
			ui.line_edit_master->setReadOnly(true);
			ui.line_edit_host->setReadOnly(true);
			ui.line_edit_topic->setReadOnly(true);
      ui.treeWidget->setEnabled(true);
      myrviz=new qrviz(ui.layout_rviz);
		}
  }
}


void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
	bool enabled;
	if ( state == 0 ) {
		enabled = true;
	} else {
		enabled = false;
	}
	ui.line_edit_master->setEnabled(enabled);
	ui.line_edit_host->setEnabled(enabled);
	//ui.line_edit_topic->setEnabled(enabled);
}

/*****************************************************************************
** Implemenation [Slots][manually connected]
*****************************************************************************/
void MainWindow::slot_treeWidget_value_change(QString)
{
  myrviz->Set_FixedFrame(fixed_box->currentText());
}


/**
 * This function is signalled by the underlying model. When the model changes,
 * this will drop the cursor down to the last line in the QListview to ensure
 * the user can always see the latest log message.
 */
void MainWindow::updateLoggingView() {
        ui.view_logging->scrollToBottom();
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", "ros_gui_n");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    //QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    //ui.line_edit_topic->setText(topic_name);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
    	ui.line_edit_master->setEnabled(false);
    	ui.line_edit_host->setEnabled(false);
    	//ui.line_edit_topic->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", "ros_gui_n");
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    //settings.setValue("topic_name",ui.line_edit_topic->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));

}

void MainWindow::closeEvent(QCloseEvent *event)
{
	WriteSettings();
	QMainWindow::closeEvent(event);
}

}  // namespace ros_gui_n

