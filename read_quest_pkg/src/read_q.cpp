#include <string>
#include <memory>
#include <chrono>
#include <functional>
#include <iostream>
#include <fstream>

#include <geometry_msgs/msg/twist.hpp> // use for geometry
#include <rclcpp/rclcpp.hpp>
#include <rosbag2_cpp/reader.hpp>
#include <rclcpp/serialization.hpp>

#include <rosbag2_cpp/writer.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;


class Vel_Read : public rclcpp::Node
{
  public:
   Vel_Read(const std::string & bag_filename) //, const std::string &save_path)
        : Node("vel_read")
        
        // , FILEPATH(save_path)
   {

      read_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
      timer_ = this->create_wall_timer(100ms, std::bind(&Vel_Read::timer_callback, this));

      // subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10,
      //                                           std::bind(&Vel_Read::timer_callback2, this, _1));

      reader_.open(bag_filename);
      // writer_ = std::make_unique<rosbag2_cpp::Writer>();
      // bag file
      // std::string file_path = "../bbaeeg";
      // rcpputils::fs::path dir(file_path);
      // rcpputils::fs::remove_all(dir);
      // writer_->open(save_path);
      // reader_.seek(0);
   }

~Vel_Read(){
reader_.close();
}

  private:
    void timer_callback()
    {
      while (reader_.has_next()){
        rosbag2_storage::SerializedBagMessageSharedPtr msg = reader_.read_next();
        if (msg->topic_name != "/turtle1/cmd_vel"){
          continue;
        }
        // geometry_msgs::msg::Twist::SharedPtr smsg;
        rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
        geometry_msgs::msg::Twist::SharedPtr ros_msg = std::make_shared<geometry_msgs::msg::Twist>();
        serialization_.deserialize_message(&serialized_msg, ros_msg.get());

        geometry_msgs::msg::Twist geo_msg = *ros_msg;
        rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);
        // writer_->write(geo_msg, "turtle1/cmd_vel",time_stamp);

        read_publisher_->publish(*ros_msg);
        std::cout << '(' << ros_msg->linear.x << ", " << ros_msg->angular.z <<")\n";

        break;
      }

      // std::cout << "@@@@@@@@@@@@DBG PRINT@@@@@@@@@@@@@@@@@@@@" << std::endl;
    }

#if 0
    void timer_callback()
    {
     while (reader_.has_next()){
        rosbag2_storage::SerializedBagMessageSharedPtr msg = reader_.read_next();
       if (msg->topic_name != "turtle1/cmd_vel"){
           continue;
        }
        // geometry_msgs::msg::Twist::SharedPtr smsg;
        rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);

        // geometry_msgs::msg::Twist::SharedPtr ros_msg = std::make_shared<geometry_msgs::msg::Twist>();
        // serialization_.deserialize_message(&serialized_msg, ros_msg.get());

        // geometry_msgs::msg::Twist geo_msg = *ros_msg;
        rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);
        std::shared_ptr<rclcpp::SerializedMessage> p_serial_msg = std::make_shared<rclcpp::SerializedMessage>(serialized_msg);
        writer_->write(p_serial_msg, "turtle1/cmd_vel","geometry_msgs/msg/Twist",time_stamp);

        // writer_->write(*ros_msg, "turtle1/cmd_vel","geometry_msgs/msg/Twist",time_stamp);
        // writer_->write(geo_msg, "turtle1/cmd_vel","geometry_msgs/msg/Twist",time_stamp);
        // read_publisher_->publish(*ros_msg);S
        std::cout << '(' << ros_msg->linear.x << ", " << ros_msg->angular.z <<")\n";

        break;
       }
     }
#endif

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr read_publisher_;

    // std::string FILEPATH;
    // rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
    // std::unique_ptr<rosbag2_cpp::Writer> writer_;
    rclcpp::Serialization<geometry_msgs::msg::Twist> serialization_;
    rosbag2_cpp::Reader reader_;
};

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <bag> <save_path>" << std::endl;
    return 1;
  }

  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<Vel_Read>(argv[1])); 
  // , argv[2]));

  rclcpp::shutdown();

  return 0;
}