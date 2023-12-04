#include <memory>
#include <chrono>
#include <iostream>
#include <fstream>

#include <rclcpp/rclcpp.hpp>
#include <rosbag2_cpp/reader.hpp>
#include <rclcpp/serialization.hpp>

#include <std_msgs/msg/string.hpp>
#include <can_msgs/msg/frame.hpp>


using namespace std::chrono_literals;
using std::placeholders::_1;


class Vel_Read : public rclcpp::Node
{
  public:
   Vel_Read(const std::string & bag_filename) //, const std::string &save_path)
        : Node("vel_read")
   {

      read_publisher_ = this->create_publisher<can_msgs::msg::Frame>("can_msg0", 10);

      timer_ = this->create_wall_timer(100ms, std::bind(&Vel_Read::timer_callback, this));

      reader_.open(bag_filename);
    
   }

  ~Vel_Read(){
    reader_.close();
  }

  private:
    void timer_callback()
    {

      int x=0;
      while (reader_.has_next()){
        rosbag2_storage::SerializedBagMessageSharedPtr msg = reader_.read_next();
        if (msg->topic_name != "can_msg0"){   // filtering topic name
          std::cout << "next\n";
          continue;
        }

        else if(msg->topic_name == "can_msg0"){                               //pick image_8
          auto message = std_msgs::msg::String();
          message.data = "can_msg cnt :  " + std::to_string(x);

          rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
          can_msgs::msg::Frame::SharedPtr ros_msg = std::make_shared<can_msgs::msg::Frame>();
          serialization_.deserialize_message(&serialized_msg, ros_msg.get());

          can_msgs::msg::Frame geo_msg = *ros_msg;
          rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);

          read_publisher_->publish(*ros_msg);
          x++;

          
        }
      



      }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr read_publisher_;


    rclcpp::Serialization<can_msgs::msg::Frame> serialization_;
    rosbag2_cpp::Reader reader_;
  };


int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <bag> " << std::endl;
    return 1;
  }

  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<Vel_Read>(argv[1])); 

  rclcpp::shutdown();

  return 0;
}
