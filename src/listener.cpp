#include "ros/ros.h"
#include "bits/stdc++.h"//c++ header now compiling
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "nav_msgs/OccupancyGrid.h"
#include<geometry_msgs/Twist.h> //for linear velocity
#define pb push_back
using namespace std_msgs;
using namespace std;
using namespace nav_msgs;
vector<int> mytest;
void mapcallback(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
 std_msgs::Header header  = msg->header;
 nav_msgs::MapMetaData info = msg->info;
// int8_t data[];
 ROS_INFO(" ");//this is nessecarry before any cout in order to produce the see the output on the log;
cout<<"-------------------------------Printing map data -------------------------------"<<"\n";
for(auto it : msg->data)
cout<<(int)it<<" ";
cout<<"-------------------------------Printed all map data ----------------------------"<<"\n";
cout<<"---------------------------------Printing position------------------------------"<<"\n";
cout<<"i am here"<<info.origin.position.x<<" "<<info.origin.position.y;

}

void velCallBack(const geometry_msgs::Twist& msg){
    float linx, liny;
    ROS_INFO("");
    cout<<"--------------------------------------Printing velocity-------------------------"<<"\n";
    //for doing this subscribe to the sensor fusion data /odom/filetered or /imu/data 
    //use this topic /cmd_vel_mux/input/navi to move the robot
    //subcribe to /odom for linear vel
    linx=msg.linear.x;
    liny=msg.linear.y;
    cout<<"linear velocity in x:"<<linx<<endl;
    cout<<"linear velocity in y:"<<liny<<endl;

}

int main(int argc, char **argv)
{
 ros::init(argc,argv,"listener");
 ros::NodeHandle n;
 ros::Subscriber map_sub = n.subscribe("/move_base/local_costmap/costmap",1000,mapcallback);
 ros::Subscriber vel_sub = n.subscribe("/odom", 1000, velCallBack);
ros::spin();
return 0;
}
