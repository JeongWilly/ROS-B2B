# ROS-B2B
## pkg description
<b>if you want to test this code, you need to download ros2_humble in your linux.  </b>

quest_pkg : turtlebot의 topic_name을 subscribe하며 bagfile을 생성한다.  

read_quest_pkg : turtlebot이 발행하는 topic_name을 publish하여 turtlebot의 움직임을 읽고, 동시에 bagfile을 생성한다.  

can_logger_pkg : publish can type topic msg & read a bagfile 추후 pkg의 역할 식별이 어려울시, 변경 가능
