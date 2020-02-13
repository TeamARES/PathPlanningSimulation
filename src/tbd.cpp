#include<stdc++/bits.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Header.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/Odometry.h"
#include<geometry_msgs/Twist.h> //for linear velocity
#define pb push_back
using namespace std_msgs;
using namespace std;
using namespace nav_msgs;

using namespace std;

int threshold= 0; //SET THRESHOLD HERE

double minx,miny,maxx,maxy,xwidth,ywidth;
double max_velx=3;//max vel in x direction
double min_velx=-3;//min vel in x direction
double max_vely=3;//max vel in y direction
double min_vely=-3;//min vel in y direction
bool depth_map_status=false;//variable for checking if the depth map is updated or not    

class coordinates
{
public:
  double x,y,vx,vy;
  coordinates(double x, double y,double vx,double vy)
  {
    this->x = x;
    this->y = y;
    this->vx=vx;//current velocity at the co-ordinate in x direction
    this->vy=vy;//current velcity at the co-ordinate in y direction
  }
};
class node
{
public:
  double x,y,cost;//gcost;
  int p_index;//parent-index
  double vx,vy;//current velocity in x and y direction at the node

  node()
  {
	x=0;y=0;cost=0;
 vx=0;vy=0;
  }
  node(double x, double y,double vx,double vy,double cost, int p_index)
  {
    this->x = x;
    this->y = y;
    this->cost = cost;
    this->p_index=p_index;
    this->vx=vx;
    this->vy=vy;
  }

};


void revise_obstacle_coordinates(vector<int>  &ox, vector<int>  &oy, nav_msgs::OccupancyGrid::ConstPtr &occgrid_constptr )  {
    for(auto it : occgrid_constptr->data)
    {
        if((int)it>threshold){
            int x, y;
            //get x and y value of that point;
            ox.pb(x);
            oy.pb(y);
        }
    }
    return;
} 

void path_planning(double x_in, double y_in,double vx,double vy,double gx, double  gy,double vgx,double vgy,vector<int> ox, vector<int> oy){
    /*
    gx: goal x position [m]
    gx: goal x position [m]
    ox: x position list of Obstacles [m]
    oy: y position list of Obstacles [m]
    reso: grid resolution [m]
    rr: robot radius[m]
    rx: rover x-co-ordinates
    ry : rover y-co-ordinate
    vx:starting x velocity
    vy:starting y velocity
    vgx:goal x velocity
    vgy:goal y vleocity
    */
    int reso = 1;
    int rr = 1;
    cout<<"Starting the process"<<"\n";
    double rx=x_in;
    double ry=y_in;
    ROS_INFO("");
    cout<<"Now at"<<rx<<" "<<ry<<"\n";

    cout<<"My velocity in x direction is:"<<vx<<" "<<"and in y direction is:"<<vy<<"\n";
    

    while (1)
    {
        coordinates ggkk(67,7,1,1);//values inside this variable are random , taken just to store the output from a_star_planning function

        ggkk = a_star_planning(rx,ry,vx,vy,gx, gy,vgx,vgy,ox, oy, ox.size(),reso,rr);
        rx = ggkk.x;
        ry = ggkk.y;
       ROS_INFO(""); 
       cout<<"go to ::::::"<<" ";
       cout<<rx<<" "<<ry<<"\n";

       cout<<"You will reach with velcity in x:"<<ggkk.vx<<" "<<"and in y:"<<ggkk.vy<<endl;
       
        if (rx==gx && ry==gy && ggkk.vx == vgx && ggkk.vy == vgy)
         {

             break;
         }

       /*
        Suppose we also get feedback corresponding to the location of the rover as input
        then we can do incorporate that as wellself.
        if bool variable feed_loc is true then the revised location of rover will be used
        for path path_planning
        */

            /*cout<<"Depth Map Completed?"<<"\n";
            cin>>depth_map_status;
            while(!depth_map_status)
             cin>>depth_map_status;
            
            i am just going to update the deptch map once here
            */
            nav_msgs::OccupancyGrid::ConstPtr occgrid_constptr = ros::topic::waitForMessage("/move_base/local_costmap/costmap");
    
            revise_obstacle_coordinates(ox, oy, occgrid_constptr);

            
            
            /*printf("Enter 1 for location feedback\n");
            int feed_loc;
            cin >> feed_loc;
            if (feed_loc == 1)
            {
                printf("Enter new coordinates\n");
                cin >> rx >> ry;
            }*/

      }
    ROS_INFO("");
    printf("Yeah! We reached the target");
    

}


coordinates a_star_planning(double sx, double sy,double vx,double vy,double gx, double gy,double vgx,double vgy,vector<int>  obsx, vector<int>  obsy, int obsn, int reso, int rr  )
{
  /*
  gx: goal x position [m]
  gx: goal x position [m]
  ox: x position list of Obstacles [m]
  oy: y position list of Obstacles [m]
  reso: grid resolution [m]
  rr: robot radius[m]
  rx: rover x-co-ordinates
  ry : rover y-co-ordinate
  vx:starting x velocity
  vy:starting y velocity
  vgx:goal x velocity
  vgy:goal y vleocity
  */
  //sx is the starting x co-ordinate
  //sy is the starting y co-ordinate
  //gx is the goal x co-ordinate
  //gy is the goal y co-ordinate


    node nstart = node(sx/reso, sy/reso,vx,vy,0.0,-1);
    node ngoal = node(gx/reso, gy/reso,vgx,vgy,0.0, -1);

    for(int i=0;i<obsn;i++)
    {
      obsx[i] = obsx[i]/reso;
      obsy[i] = obsy[i]/reso;
    }

    calc_obstacle_map(obsx, obsy, obsn, reso, rr);
    typedef tr1::unordered_map <int, node> pppp;
    pppp openset;
    pppp closedset;
    openset[calc_index(nstart, xwidth, minx, miny)] = nstart;
    int testing = 0;
    double finalx=0;
    double finaly=0;
    double finalvx=0;
    double finalvy=0;
    while(testing<2)//testing variable to ensure that this loop only runs twice
    {
      double min_temp = INT_MAX;//to find the value of the min f value
      int cid=0;//funtion to store index of the node
      for(pppp:: const_iterator i = openset.begin(); i!=openset.end(); ++i)
      {
        double rr = (i->second).cost + calc_heuristic(ngoal, i->second);
        if(rr<min_temp)
        {
          min_temp=rr;
          cid = i->first;
        }

      }
      node current = openset[cid];
     cout << current.x << "," <<  current.y << endl;
     finalx=current.x;
     finaly=current.y;
     finalvx = current.vx;
     finalvy = current.vy;
     cout<<finalvx<<endl;
     cout<<finalvy<<endl;

      if ( current.x == ngoal.x && current.y == ngoal.y && current.vx == ngoal.vx && current.vy == ngoal.vy)
      {
             // cout << "Goal found\n";
              ngoal.p_index = current.p_index;
              ngoal.cost = current.cost;
              break;
      }

      openset.erase(cid);
      closedset[cid] = current;

      for(int vi=min_velx;vi<=max_velx;vi++)
{      for(int vj=min_vely;vj<=max_vely;vj++)
      {
        double cost = pow((pow((vi-current.vx),2)+pow((vj-current.vy),2)),0.5);
        node dummy = node(current.x + (vi*1),current.y + (vj*1),vi,vj,cost ,cid);
	//error here xwidth intead of xw
        int n_id = calc_index(dummy, xwidth, minx, miny);

        if(closedset.find(n_id)!=closedset.end())
              continue;
        if(verify_node(dummy, minx, miny, maxx,maxy,current)==0)//have to change this.
          continue;
        if(openset.find(n_id)==openset.end())
        {
          openset[n_id] = dummy;
        }
        else
        {
          if(openset[n_id].cost>=dummy.cost)
          {
            openset[n_id] = dummy;
          }
        }

      }

}
testing++;
}
return coordinates(finalx, finaly,finalvx,finalvy);
//error here did not retunr coordinates


}




int main(){
    ros::init(argc,argv,"listener");
    ros::NodeHandle n; 

    vector<int> ox;
    vector<int> oy;    
    ROS_INFO("");
    printf("Lets start working\n");
    printf("Entering starting position coordinates");

    double x_in, y_in,gx,gy;
    /*for use of waitForMessage: http://docs.ros.org/kinetic/api/roscpp/html/namespaceros_1_1topic.html*/
    nav_msgs::OccupancyGrid::ConstPtr occgrid_constptr = ros::topic::waitForMessage("/move_base/local_costmap/costmap");
    nav_msgs::MapMetaData info = occgrid_constptr->info; //this line migh be problematic

    x_in=(double) info.origin.position.x;
    y_in=(double)info.origin.position.y;

    ROS_INFO("");
    cout<<"enter the goal coordinate"<<endl;
    cin>>gx>>gy;

    double vx=0;
    double vy=0;
    double vgx=0;
    double vgy=0;
    
    depth_map_status=true;
    //input the depth_map
    nav_msgs::OccupancyGrid::ConstPtr occgrid_constptr = ros::topic::waitForMessage("/move_base/local_costmap/costmap");
    
    revise_obstacle_coordinates(ox, oy, occgrid_constptr);
    path_planning(x_in, y_in,vx,vy,gx,gy,vgx,vgy,ox,oy);

    return 0;
}