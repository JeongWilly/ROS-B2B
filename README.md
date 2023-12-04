# <span style="color: green">ROS-B2B</span>
## pkg description
<b>if you want to test this code, you need to download ros2_humble in your linux.  </b>

## recording turtlebot's movement
<b>quest_pkg</b> : turtlebot의 topic_name을 subscribe하며 bagfile을 생성한다.  

<b>read_quest_pkg</b> : turtlebot이 발행하는 topic_name을 publish하여 turtlebot의 움직임을 읽고, 동시에 bagfile을 생성한다.  

## can_msg
can viewer구현을 마치는 대로 pub/sub 확인 예정, 현재는 터미널 내에서 pub/sub만 가능

<b>can_logger_pkg</b> : publish can type topic msg & read a bagfile 
