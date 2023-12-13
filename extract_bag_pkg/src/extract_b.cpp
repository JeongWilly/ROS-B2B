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

class Vel_Read : public rclcpp::Node 
{ 
  public: 
   Vel_Read(const std::string & bag_filename)  
        : Node("vel_read")
   {   

      reader_.open(bag_filename);

      //filewriter<< "time, linear.x, linear.y, linear.z, angular.x, angular.y, angular.z \n"; 

      filewriter.open("rogbag2");
      read_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
      timer_ = this->create_wall_timer(100ms, std::bind(&Vel_Read::timer_callback, this));

      // reader_.open(bag_filename);   


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



        if (msg->topic_name != "turtle1/cmd_vel"){
           continue;
        }            
        rclcpp::SerializedMessage serialized_msg(*msg->serialized_data);
        geometry_msgs::msg::Twist::SharedPtr ros_msg = std::make_shared<geometry_msgs::msg::Twist>();
        serialization_.deserialize_message(&serialized_msg, ros_msg.get());

        //char enoughbuf[130];

        read_publisher_->publish(*ros_msg);
	      int64_t ts = msg->time_stamp/1000;

        filewriter << "time: " << ts << ", ";
        filewriter << "linear_x=" << ros_msg->linear.x << ", ";
        filewriter << "linear_y=" << ros_msg->linear.y << ", ";
        filewriter << "linear_z=" << ros_msg->linear.z << ", ";
        filewriter << "angular_x=" << ros_msg->angular.x << ", ";
        filewriter << "angular_y=" << ros_msg->angular.y << ", ";
        filewriter << "angular_z=" << ros_msg->angular.z << "\n";

        // sprintf(enoughbuf,"time: %ld" ,ts);   S
        // sprintf(enoughbuf,"linear_x=%.2f",ros_msg->linear.x);
        // sprintf(enoughbuf,"linear_y=%.2f",ros_msg->linear.y);
        // sprintf(enoughbuf,"linear_z=%.2f",ros_msg->linear.z);
        // sprintf(enoughbuf,"angular_x=%.2f",ros_msg->angular.x);
        // sprintf(enoughbuf,"angular_y=%.2f",ros_msg->angular.y);
        // sprintf(enoughbuf,"angular_z=%.2f",ros_msg->angular.z);      
	
      	//filewriter<< enoughbuf << "\n";
        
        break;
       }
     
     
    }

    rclcpp::TimerBase::SharedPtr timer_; 
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr read_publisher_;

    rclcpp::Serialization<geometry_msgs::msg::Twist> serialization_;
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


