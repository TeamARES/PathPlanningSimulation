#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "nav_msgs/OccupancyGrid.h"
using namespace std_msgs;
using namespace nav_msgs;
void mapcallback(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
 std_msgs::Header header  = msg->header;
 nav_msgs::MapMetaData info = msg->info;
 ROS_INFO("got map %d %d",info.width,info.height);

}

int main(int argc, char **argv)
{
 ros::init(argc,argv,"listener");
 ros::NodeHandle n;
 ros::Subscriber map_sub = n.subscribe("/move_base/local_costmap/costmap",1000,mapcallback);
ros::spin();
return 0;
}
