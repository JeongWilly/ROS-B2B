#include <string> 

#include <memory> 
#include <chrono> 
#include <functional> 
#include <iostream> 
#include <fstream> 
#include <can_msgs/msg/frame.hpp> // use for can_msgs 
#include <rclcpp/rclcpp.hpp> 
#include <rosbag2_cpp/reader.hpp> 
#include <rclcpp/serialization.hpp> 
#include <rosbag2_cpp/writer.hpp>



using namespace std::chrono_literals;  

class Vel_Read : public rclcpp::Node 
{ 
  public: 
   Vel_Read(const std::string & bag_filename)  
        : Node("vel_read")
   {   

      reader_.open(bag_filename);


      filewriter.open("compare_can_bag2");
      read_publisher_ = this->create_publisher<can_msgs::msg::Frame>("can_msg0", 10);
      timer_ = this->create_wall_timer(100ms, std::bind(&Vel_Read::timer_callback, this));



   }
   ~Vel_Read()
   {
	   reader_.close();
	   filewriter.close();
   }


  private:
    void timer_callback()
    {   
     while (reader_.has_next()){
        rosbag2_storage::SerializedBagMessageSharedPtr msg = reader_.read_next();



      if (msg->topic_name != "can_msg0"){
           continue;
        }            
      else if(msg->topic_name == "can_msg0"){
        rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
        can_msgs::msg::Frame::SharedPtr ros_msg = std::make_shared<can_msgs::msg::Frame>();
        serialization_.deserialize_message(&serialized_msg, ros_msg.get());



        read_publisher_->publish(*ros_msg);
	      int64_t ts = msg->time_stamp/1000;


        filewriter << "time: " << ts << ", ";
        filewriter << "can_Header_time=" << ros_msg->header.stamp.nanosec << ", ";
        filewriter << "can_Header_frame_id=" << ros_msg->header.frame_id<< ", ";
        filewriter << "can_id = " << ros_msg->id << ", ";
        filewriter << "can_dlc = " << ros_msg->dlc << ", "<< "\n";
        }
       }
     std::cout<<"done\n";
     
    }

    rclcpp::TimerBase::SharedPtr timer_; 
    rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr read_publisher_;

    rclcpp::Serialization<can_msgs::msg::Frame> serialization_;
    rosbag2_cpp::Reader reader_;
    std::ofstream filewriter;
};



int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <bag>" << std::endl;
    return 1;
  }

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Vel_Read>(argv[1]));
  std::cout<<"done\n";
  rclcpp::shutdown();

  return 0;
}


