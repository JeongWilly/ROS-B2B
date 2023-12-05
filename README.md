
![ros logo](https://github.com/JeongWilly/ROS-B2B/assets/109045231/7de15729-4c50-42af-8adb-38c4095c04e4)
# ROS-B2B  
several camera msg type, pub/sub & logging  
recently I used to real logging data, but there's a copyright issue. So i think i`ll probably test it with dummy data next time.


## pkg description 
###  if you want to test this code, you need to download ros2_humble in your linux.  

<br></br>
## recording turtlebot's movement
<b>quest_pkg</b> : subscribe turtlebot's topicname and generate bagfile  

<b>read_quest_pkg</b> : publish turtlebot's ros2 topicname and subscribe movement at sametime.   

<br></br>  
## can_msg
can viewer구현을 마치는 대로 pub/sub 확인 예정, 현재는 터미널 내에서 pub/sub만 가능

<b>can_logger_pkg</b> : publish topic msg & read a bagfile   
<b>can_sub_pkg</b> : subscribe topic about can_msg    
<b>can_extract_pkg</b> : re-enable the bagfile only with the code topic_name that corresponds to a specific name

<br></br>
