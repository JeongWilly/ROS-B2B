# ROS-B2B : several camera msg type, pub/sub & logging   

## pkg description 
###  if you want to test this code, you need to download ros2_humble in your linux.  

<br></br>
## recording turtlebot's movement
<b>quest_pkg</b> : turtlebot의 topic_name을 subscribe하며 bagfile을 생성한다.  

<b>read_quest_pkg</b> : turtlebot이 발행하는 topic_name을 publish하여 turtlebot의 움직임을 읽고, 동시에 bagfile을 생성한다.  

<br></br>  
## can_msg
can viewer구현을 마치는 대로 pub/sub 확인 예정, 현재는 터미널 내에서 pub/sub만 가능

<b>can_logger_pkg</b> : publish topic msg & read a bagfile   
<b>can_sub_pkg</b> : subscribe topic about can_msg  

<br></br>

  
  ## 매일 작업 내용 정리하기

