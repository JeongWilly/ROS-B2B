#include <memory>
#include <chrono>
#include <functional>
#include <string>


#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp> 
#include <rosbag2_cpp/writer.hpp>
#include <rclcpp/serialization.hpp>
#include <rosbag2_cpp/reader.hpp>



using std::placeholders::_1;


class Vel_Sub : public rclcpp::Node
{
  public:
   Vel_Sub() : Node("vel_sub")
   {

      subscription_ = this->create_subscription<can_msgs::msg::Frame>(
          "can_msg0", 10, std::bind(&Vel_Sub::topic_callback, this, _1));  
    
   }

  private:
    void topic_callback(const can_msgs::msg::Frame::SharedPtr msg)
    {
      std::cout << "Received can_msg0: " << msg->id << std::endl;
    }

    rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr subscription_;
};


int main(int argc, char * argv[])
{
 rclcpp::init(argc, argv);
 rclcpp::spin(std::make_shared<Vel_Sub>());

 rclcpp::shutdown();
 return 0;
}

