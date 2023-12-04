# ROS-B2B
## pkg description
<b>if you want to test this code, you need to download ros2_humble in your linux.  </b>

###recording turtlebot's movement
quest_pkg : turtlebot의 topic_name을 subscribe하며 bagfile을 생성한다.  

read_quest_pkg : turtlebot이 발행하는 topic_name을 publish하여 turtlebot의 움직임을 읽고, 동시에 bagfile을 생성한다.  

###can_msg
추후 pkg의 역할 식별이 어려울시, 변경 가능(현재 viewer구현을 아직 미완성해서, viewer구현이 끝나는 대로 pub/sub 확인 예정, 현재는 터미널 내에서 pub/sub만 가능)  

can_logger_pkg : publish can type topic msg & read a bagfile 
