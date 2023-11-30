#include <memory>
#include <chrono>
#include <functional>


#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp> // use for geometry
#include <rosbag2_cpp/writer.hpp>
#include <rclcpp/serialization.hpp>


using std::placeholders::_1;


class Vel_Sub : public rclcpp::Node
{
  public:
   Vel_Sub() : Node("vel_sub")
   {

      writer_ = std::make_unique<rosbag2_cpp::Writer>();

      writer_->open("bbaeeg");


      subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
          "turtle1/cmd_vel", 10, std::bind(&Vel_Sub::topic_callback, this, _1));  
    
   }

  private:
    void topic_callback(geometry_msgs::msg::Twist::SharedPtr msg)const
    {
      geometry_msgs::msg::Twist geo_msg = *msg;
      RCLCPP_INFO(this->get_logger(), " .X : '%.5f', .Y : '%.5f'", geo_msg.linear.x, geo_msg.angular.z);
      
      rclcpp::Time time_stamp = this->now();
      writer_->write(geo_msg,"turtle1/cmd_vel", time_stamp);

      

    }


    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
    std::unique_ptr<rosbag2_cpp::Writer> writer_;
};

int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 rclcpp::spin(std::make_shared<Vel_Sub>());

 rclcpp::shutdown();
 return 0;
}

