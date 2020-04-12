#include "ros/ros.h"
#include "bits/stdc++.h"//c++ header now compiling
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/Odometry.h"
#include<geometry_msgs/Twist.h> //for linear velocity
#define pb push_back
using namespace std_msgs;
using namespace std;
using namespace nav_msgs;
#define ll long long int
float a =0;
float b_x = 0;
float b_y = 0;
float res = 0.05;
float c_x=0;
float c_y = 0;
/*void mapcallback(const nav_msgs::OccupancyGrid::ConstPtr& msg)
{
vector<int> mytest;
vector<int> data_sizes;

 std_msgs::Header header  = msg->header;
 nav_msgs::MapMetaData info = msg->info;
// int8_t data[];
 ROS_INFO(" ");//this is nessecarry before any cout in order to produce the see the output on the log;
//cout<<"-------------------------------Printing map data -------------------------------"<<"\n";
for(auto it : msg->data)//is this printing the data 60 then 60 then 60 ---> so on
mytest.pb((int)it);
//cout<<"-------------------------------Printed all map data ----------------------------"<<"\n";
cout<<"array size"<<mytest.size()<<"\n";//writing it to check my data
data_sizes.pb(mytest.size());
//cout<<"my map data should be of"<<info.width<<info.height<<"\n";//this is printing 60 and 60
cout<<"---------------------------------Printing position------------------------------"<<"\n";
cout<<"i am here"<<info.origin.position.x<<" "<<info.origin.position.y;
c_x = info.origin.position.x;
c_y = info.origin.position.y;
cout<<"-------------------------------All map data sizes-------------------------------"<<"\n";
for(auto it : data_sizes)
cout<<(int)it<<"\n";
return;
}

void velCallBack(const nav_msgs::Odometry& msg){

    ROS_INFO(" ");
    cout<<"--------------------------------------Printing velocity-------------------------"<<"\n";
    //for doing this subscribe to the sensor fusion data /odom/filetered or /imu/data
    //use this topic /cmd_vel_mux/input/navi to move the robot
    //subcribe to /odom for linear vel
    cout<<(float)msg.twist.twist.linear.x<<"x->linear"<<"\n";
    cout<<(float)msg.twist.twist.linear.y<<"y->linear"<<"\n";
    cout<<(float)msg.twist.twist.linear.z<<"z->linear"<<"\n";
    return;
//cout<<it<<" ";

}

void gpsCallBack(const nav_msgs::Odometry& msg){

    ROS_INFO(" ");
    cout<<"--------------------------------------Printing position-------------------------"<<"\n";
    //for doing this subscribe to the sensor fusion data /odom/filetered or /imu/data
    //use this topic /cmd_vel_mux/input/navi to move the robot
    //subcribe to /odom for linear vel
    cout<<(float)msg.pose.pose.position.x<<"x->position"<<"\n";
    b_x= (float)msg.pose.pose.position.x;
    b_y = (float)msg.pose.pose.position.y;
    cout<<(float)msg.pose.pose.position.y<<"y->position"<<"\n";
    cout<<(float)msg.pose.pose.position.z<<"z->position"<<"\n";
    cout<<"grid position of rover"<<"x"<<":"<<(b_x-c_x)/res<<"y"<<(b_y-c_y)/res<<"\n";
    return ;

}
*/
void call_once()
{
  boost::shared_ptr<const nav_msgs::Odometry> msg = ros::topic::waitForMessage<nav_msgs::Odometry>("/odom");
  cout<<(float)msg->twist.twist.linear.x<<"x->linear"<<"\n";
  cout<<(float)msg->twist.twist.linear.y<<"y->linear"<<"\n";
  cout<<(float)msg->twist.twist.linear.z<<"z->linear"<<"\n";
 return;
}

int main(int argc, char **argv)
{
 ros::init(argc,argv,"listener");
 //ros::NodeHandle n;
 //ros::Subscriber map_sub = n.subscribe("/move_base/local_costmap/costmap",1000,mapcallback);
 //ros::Subscriber vel_sub = n.subscribe("/odom", 1000, velCallBack);//comment out to do actual node programming
/* boost::shared_ptr<const nav_msgs::Odometry> msg = ros::topic::waitForMessage<nav_msgs::Odometry>("/odom");
 cout<<(float)msg->twist.twist.linear.x<<"x->linear"<<"\n";
 cout<<(float)msg->twist.twist.linear.y<<"y->linear"<<"\n";
 cout<<(float)msg->twist.twist.linear.z<<"z->linear"<<"\n";
 //ros::Subscriber gps_sub = n.subscribe("/odom",1000,gpsCallBack);


 cout<<"---------------------------------------continue with the rest of code--------------------------------------------------------------"<<"\n";
 cout<<"Done with the rest of the code"<<"\n";
 cout<<"Need to call again"<<"\n";
 boost::shared_ptr<const nav_msgs::Odometry> msg2 = ros::topic::waitForMessage<nav_msgs::Odometry>("/odom");
 cout<<(float)msg2->twist.twist.linear.x<<"x->linear"<<"\n";
 cout<<(float)msg2->twist.twist.linear.y<<"y->linear"<<"\n";
 cout<<(float)msg2->twist.twist.linear.z<<"z->linear"<<"\n";
*/
call_once();
cout<<"Continue with the rest of the code"<<"\n";
cout<<"Done with the rest of the code"<<"\n";
cout<<"Now need to call that again"<<"\n";
call_once();

return 0;
}
