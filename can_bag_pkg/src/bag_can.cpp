#include <memory>
#include <chrono>
#include <iostream>
#include <fstream>

#include <rclcpp/rclcpp.hpp>
#include <rosbag2_cpp/reader.hpp>
#include <rosbag2_cpp/writer.hpp>
#include <rclcpp/serialization.hpp>

#include <can_msgs/msg/frame.hpp>


using namespace std::chrono_literals;

class Vel_Read : public rclcpp::Node
{
  public:
   Vel_Read(const std::string & bag_filename) //, const std::string &save_path)
        : Node("vel_read")
   {
      // std::cout << "DBG Print\n";

      reader_.open(bag_filename);
      writer_ = std::make_unique<rosbag2_cpp::Writer>();
      writer_->open("compare_canfile");

      read_publisher_1 = this->create_publisher<can_msgs::msg::Frame> ("can_msg0",10);
      //read_publisher_2 = this->create_publisher<sensor_msgs::msg::Image> ("image_7",10);

      timer_ = this->create_wall_timer(100ms, std::bind(&Vel_Read::timer_callback, this));

     
   }

  ~Vel_Read(){
    reader_.close();
  }

  private:
    void timer_callback()
    {
      int n=0; int m=0; int x=0; 
      // int y=0;
      while (reader_.has_next()){
        std::cout << "while can find scope "<<m<<std::endl; m++;
        rosbag2_storage::SerializedBagMessageSharedPtr msg = reader_.read_next();
        if (msg->topic_name != "can_msg0"){
		//&& msg->topic_name != "image_7"){   // filtering topic name
          std::cout << "other " <<n<<std::endl; n++;
          continue;
        }
      // else if(msg->topic_name == "image_8"){                               //pick image_8
          std::cout << "can_msg0 check  " <<x<<std::endl; x++;
          rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
          can_msgs::msg::Frame::SharedPtr ros_msg1 = std::make_shared<can_msgs::msg::Frame>();
          serialization_.deserialize_message(&serialized_msg, ros_msg1.get());

          can_msgs::msg::Frame geo_msg = *ros_msg1;
          rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);

          read_publisher_1->publish(*ros_msg1);
          writer_->write(geo_msg, "can_msg0", time_stamp);

      //  }
      //  else if(msg->topic_name == "image_7"){                              //pick image_7
      //    std::cout << "image_8 check  " <<y<<std::endl; y++;
      //    rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
      //    sensor_msgs::msg::Image::SharedPtr ros_msg2 = std::make_shared<sensor_msgs::msg::Image>();
      //    serialization_.deserialize_message(&serialized_msg, ros_msg2.get());

      //    sensor_msgs::msg::Image geo_msg = *ros_msg2;
      //    rclcpp::Time time_stamp = rclcpp::Time(msg->time_stamp);

      //    read_publisher_2->publish(*ros_msg2);
      //    writer_->write(geo_msg, "image_7", time_stamp);
      //   }
        
      }
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr read_publisher_1;
   // rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr read_publisher_2;

    rclcpp::Serialization<can_msgs::msg::Frame> serialization_;
    std::unique_ptr<rosbag2_cpp::Writer> writer_;
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
