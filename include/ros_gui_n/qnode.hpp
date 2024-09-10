/**
 * @file /include/ros_gui_n/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef ros_gui_n_QNODE_HPP_
#define ros_gui_n_QNODE_HPP_

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
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <QImage>
/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ros_gui_n {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	bool init(const std::string &master_url, const std::string &host_url);
  void sub_image(QString  topic_name);
  void set_cmd_vel(char k,float linear,float angular);
  void run();

	/*********************
	** Logging
	**********************/
	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);

Q_SIGNALS:
	void loggingUpdated();
    void rosShutdown();
    void image_val(QImage);
    void speed_vel(float,float);

private:
	int init_argc;
	char** init_argv;
	ros::Publisher chatter_publisher;
  ros::Publisher cmd_vel_pub;
    QStringListModel logging_model;
    image_transport::Subscriber image_sub;
    void image_callback(const sensor_msgs::ImageConstPtr &msg);
    QImage Mat2QImage(cv::Mat const& src);
    ros::Subscriber odom_sub;
    void odom_callback(const nav_msgs::Odometry &msg);
};

}  // namespace ros_gui_n

#endif /* ros_gui_n_QNODE_HPP_ */
