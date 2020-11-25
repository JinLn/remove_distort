#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class DistortRemove
{
    public:
        DistortRemove();
        int exec();
    private:
        cv::Mat cameraMatrix = cv::Mat::zeros(3, 3, CV_64F);
        cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
        ros::NodeHandle nh;
        ros::Subscriber image_sub;
        ros::Publisher image_pub;
        void RemoveCallback(const sensor_msgs::ImageConstPtr &msg);
};

DistortRemove::DistortRemove()
{
    //内参矩阵
  cameraMatrix.at<double>(0, 0) = 8.5966840000000002e+02;
  cameraMatrix.at<double>(0, 2) = 6.3998080000000004e+02;
  cameraMatrix.at<double>(1, 1) = 8.6134119999999996e+02;
  cameraMatrix.at<double>(1, 2) = 4.8090039999999999e+02;
  cameraMatrix.at<double>(2, 2) = 1;
  //畸变参数
  distCoeffs.at<double>(0, 0) = -3.2250000000000001e-01;
  distCoeffs.at<double>(1, 0) = 9.9699999999999997e-02;
  image_pub = nh.advertise<sensor_msgs::Image>("/remove_distort_image", 1, true);
  image_sub = nh.subscribe("/01/pylon_camera/image_raw", 1, &DistortRemove::RemoveCallback, this);  //订阅图片
}

void DistortRemove::RemoveCallback(const sensor_msgs::ImageConstPtr &msg)
{
    cv::Mat image;
    try      //获取图像msg
    {
        image = cv_bridge::toCvShare(msg, "bgr8")->image;
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv::resize(image, image, cv::Size(image.cols*0.47, image.rows*0.47));
    cv::imshow("1",image);
    cv::waitKey(1);
    //cv::Mat view, rview, map1, map2;
    //cv::Size imageSize;
    //imageSize = image.size();
    // initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
    // cameraMatrix,imageSize, CV_16SC2, map1, map2);
    //remap(image, image, map1, map2, cv::INTER_LINEAR); //重新标定获取到的图像
     sensor_msgs::ImagePtr image_msg = cv_bridge::CvImage(std_msgs::Header(),"bgr8",image).toImageMsg();
     image_pub.publish(image_msg);
}

int DistortRemove::exec()
{
    ros::spin();
    return 0;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rm_distort");
  DistortRemove remove_image_distort;
  remove_image_distort.exec();
}
