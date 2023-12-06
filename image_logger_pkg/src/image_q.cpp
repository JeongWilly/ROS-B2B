#include <memory>
#include <chrono>
#include <iostream>
#include <fstream>

#include <rclcpp/rclcpp.hpp>
#include <rosbag2_cpp/reader.hpp>
#include <rclcpp/serialization.hpp>

#include <sensor_msgs/msg/image.hpp>


using namespace std::chrono_literals;
using std::placeholders::_1;


class Vel_Read : public rclcpp::Node
{
  public:
   Vel_Read(const std::string & bag_filename) //, const std::string &save_path)
        : Node("vel_read")
   {

      read_publisher_1 = this->create_publisher<sensor_msgs::msg::Image>("image_8", 10);
      read_publisher_2 = this->create_publisher<sensor_msgs::msg::Image>("image_7", 10);
      
      timer_ = this->create_wall_timer(100ms, std::bind(&Vel_Read::timer_callback, this));

      reader_.open(bag_filename);
    
   }

  ~Vel_Read(){
    reader_.close();
  }

  private:
    void timer_callback()
    {
      while (reader_.has_next()){
        rosbag2_storage::SerializedBagMessageSharedPtr msg = reader_.read_next();
        if (msg->topic_name != "image_8"&& msg->topic_name != "image_7"){   // filtering topic name
          std::cout << "next\n";
          continue;
        }

        else if(msg->topic_name == "image_8"){                               //pick image_8

          std::cout << "I`m in image_8\n";
          rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
          sensor_msgs::msg::Image::SharedPtr ros_msg1 = std::make_shared<sensor_msgs::msg::Image>();
          serialization_.deserialize_message(&serialized_msg, ros_msg1.get());

          sensor_msgs::msg::Image geo_msg = *ros_msg1;
          rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);

          read_publisher_1->publish(*ros_msg1);


        }
        else if(msg->topic_name == "image_7"){                              //pick image_7
          std::cout << "I`m in image_7\n";
          rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
          sensor_msgs::msg::Image::SharedPtr ros_msg2 = std::make_shared<sensor_msgs::msg::Image>();
          serialization_.deserialize_message(&serialized_msg, ros_msg2.get());

          sensor_msgs::msg::Image geo_msg = *ros_msg2;
          rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);

          read_publisher_2->publish(*ros_msg2);

        }
        
      }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr read_publisher_1;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr read_publisher_2;

    rclcpp::Serialization<sensor_msgs::msg::Image> serialization_;
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
