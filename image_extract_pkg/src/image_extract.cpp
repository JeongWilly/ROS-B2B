#include <string> 

#include <memory> 
#include <chrono> 
#include <functional> 
#include <iostream> 
#include <fstream> 
#include <sensor_msgs/msg/image.hpp> // use for sensor_msgs 
#include <rclcpp/rclcpp.hpp> 
#include <rosbag2_cpp/reader.hpp> 
#include <rclcpp/serialization.hpp> 
#include <rosbag2_cpp/writer.hpp>
// #include <std_msgs/msg/header.hpp>


using namespace std::chrono_literals;  

class Vel_Read : public rclcpp::Node 
{ 
  public: 
   Vel_Read(const std::string & bag_filename)  
        : Node("vel_read")
   {   

      reader_.open(bag_filename);


      filewriter.open("compare_bag2");
      read_publisher1_ = this->create_publisher<sensor_msgs::msg::Image>("image_8", 10);
      read_publisher2_ = this->create_publisher<sensor_msgs::msg::Image>("image_7", 10);
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



      if (msg->topic_name != "image_8"&&msg->topic_name != "image_7"){
           continue;
        }            
      else if(msg->topic_name == "image_8"){
        rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
        sensor_msgs::msg::Image::SharedPtr ros_msg1 = std::make_shared<sensor_msgs::msg::Image>();
        serialization_.deserialize_message(&serialized_msg, ros_msg1.get());



        read_publisher1_->publish(*ros_msg1);
	      int64_t ts = msg->time_stamp/1000;


        filewriter << "time: " << ts << ", ";
        filewriter << "Header_time=" << ros_msg1->header.stamp.nanosec << ", ";
        filewriter << "Header_frame_id=" << ros_msg1->header.frame_id<< ", ";
        filewriter << "height=" << ros_msg1->height << ", ";
        filewriter << "width=" << ros_msg1->width << ", "<< "\n";
        }
      else if(msg->topic_name == "image_7"){
        rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
        sensor_msgs::msg::Image::SharedPtr ros_msg2 = std::make_shared<sensor_msgs::msg::Image>();
        serialization_.deserialize_message(&serialized_msg, ros_msg2.get());



        read_publisher2_->publish(*ros_msg2);
	      int64_t ts = msg->time_stamp/1000;



        filewriter << "time: " << ts << ", ";
        filewriter << "Header_time=" << ros_msg2->header.stamp.nanosec << ", ";
        filewriter << "Header_frame_id=" << ros_msg2->header.frame_id << ", ";
        filewriter << "height=" << ros_msg2->height << ", ";
        filewriter << "width=" << ros_msg2->width << ", "<< "\n";          

         }
        
        // break;
       }
     std::cout<<"done\n";
     
    }

    rclcpp::TimerBase::SharedPtr timer_; 
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr read_publisher1_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr read_publisher2_;

    rclcpp::Serialization<sensor_msgs::msg::Image> serialization_;
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


