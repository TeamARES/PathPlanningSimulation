#include<bits/stdc++.h>
#include <tr1/unordered_map>
using namespace std::tr1;
using namespace std;
#define pb push_back
int motion[8][3];
vector<vector<int> > obmap (101);
double minx,miny,maxx,maxy,xwidth,ywidth;
class coordinates
{
public:
  double x,y;
  coordinates(double x, double y)
  {
    this->x = x;
    this->y = y;
  }
};
class node
{
public:
  double x,y,cost;
  int p_index;


  node()
  {
	x=0;y=0;cost=0;

  }
  node(double x, double y, double cost, int p_index)
  {
    this->x = x;
    this->y = y;
    this->cost = cost;
    this->p_index=p_index;
  }

};
bool verify_node( node node1,int minx, int miny, int maxx, int maxy);
int find_min(vector<int> obs, int n);
int find_max(vector<int>  obs, int n);
void calc_obstacle_map(vector<int> obsx, vector<int> obsy, int obsn, int reso, int rr);
coordinates a_star_planning(double sx, double sy, double gx, double gy, vector<int>  obsx, vector<int>  obsy, int obsn, int reso, int rr  );
void get_motion_model();
double calc_heuristic(node n1, node n2);
void path_planning(double x_in, double y_in, double gx, double  gy, vector<int> ox, vector<int> oy);
void revise_obstacle_coordinates(vector<int>  &ox, vector<int>  &oy);
void revise_obstacle_coordinates(vector<int>  &ox, vector<int>  &oy)
{
    printf("Enter Obstacle coordinates in the form (x,y)\nEnter -1 -1 to terminate");
    double a,b;
    cin >> a >> b;
    //ox.pb(0);
    //oy.pb(0);
    //ox.pb(1000);
    //oy.pb(1000);
    while (a!=-1 && b!=-1)
    {
        ox.pb(a);
        oy.pb(b);
        cin >> a >> b;

      }

    return ;
}

int calc_index(node n, double xwidth, double xmin, double ymin)
{
  return (n.y - ymin)*xwidth + (n.x-xmin);
}
void path_planning(double x_in, double y_in, double gx, double  gy, vector<int> ox, vector<int> oy){
    /*
    gx: goal x position [m]
    gx: goal x position [m]
    ox: x position list of Obstacles [m]
    oy: y position list of Obstacles [m]
    reso: grid resolution [m]
    rr: robot radius[m]
    */
    int reso = 1;
    int rr = 1;
    cout<<"Starting the process"<<"\n";
    int ares_main=0;
    double rx=x_in;
    double ry=y_in;
    cout<<"Now at"<<rx<<" "<<ry<<"\n";
    while (1)
    {
        ares_main=ares_main+1;
        coordinates ggkk(67,7);

        ggkk = a_star_planning(rx, ry, gx, gy, ox, oy, ox.size(), reso, rr);
        rx = ggkk.x;
        ry = ggkk.y;
       cout<<"go to ::::::"<<" ";
       cout<<rx<<" "<<ry<<"\n";        
        if (rx==gx and ry==gy)
         {
           
             break;
          }

       /*
        Suppose we also get feedback corresponding to the location of the rover as input
        then we can do incorporate that as wellself.
        if bool variable feed_loc is true then the revised location of rover will be used
        for path path_planning
        */


            revise_obstacle_coordinates(ox,oy);
            printf("Enter 1 for location feedback\n");
            int feed_loc;
            cin >> feed_loc;
            if (feed_loc == 1)
            {
                printf("Enter new coordinates\n");
                cin >> rx >> ry;
            }

      }


    printf("Yeah! We reached the target");

}
coordinates a_star_planning(double sx, double sy, double gx, double gy, vector<int>  obsx, vector<int>  obsy, int obsn, int reso, int rr  )
{
  /*
    gx: goal x position [m]
    gx: goal x position [m]
    obsx: x position list of Obstacles [m]
    obsy: y position list of Obstacles [m]
    obsn: number of obstacles
    reso: grid resolution [m]
    rr: robot radius[m]
    */
    node nstart = node(sx/reso, sy/reso, 0.0, -1);
    node ngoal = node(gx/reso, gy/reso, 0.0, -1);

    for(int i=0;i<obsn;i++)
    {
      obsx[i] = obsx[i]/reso;
      obsy[i] = obsy[i]/reso;
    }

    calc_obstacle_map(obsx, obsy, obsn, reso, rr);
    get_motion_model();
	//error here
    typedef tr1::unordered_map <int, node> pppp;
    pppp openset;
    pppp closedset;
    ///unordered_map <int, node> closedset;
    openset[calc_index(nstart, xwidth, minx, miny)] = nstart;
   int testing = 0;
  double finalx=0;
  double finaly=0;  
  while(testing<2)
    {
      double min_temp = INT_MAX;
      int cid=0;
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
     // cout << current.x << "," <<  current.y << endl;
      finalx=current.x;
    finaly=current.y;

      if ( current.x == ngoal.x && current.y == ngoal.y)
      {
             // cout << "Goal found\n";
              ngoal.p_index = current.p_index;
              ngoal.cost = current.cost;
              break;
      }

      openset.erase(cid);
      closedset[cid] = current;

      for(int j=0;j<8;j++)
      {
        node dummy = node(current.x + motion[j][0],
                          current.y + motion[j][1],
                          current.cost + motion[j][2], cid);
	//error here xwidth intead of xw
        int n_id = calc_index(dummy, xwidth, minx, miny);

        if(closedset.find(n_id)!=closedset.end())
              continue;
        if(verify_node(dummy, minx, miny, maxx,maxy)==0)
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
testing++;    
   }

return coordinates(finalx, finaly);
//error here did not retunr coordinates

    
}

bool verify_node( node node1,int minx, int miny, int maxx, int maxy)
{
  if(node1.x<minx)
    return 0;
  else if(node1.y<miny)
    return 0;
  else if(node1.x>=maxx || node1.y>=maxy)
    return 0;
	//error here, should be node1.x-minx and same with y
  if(obmap[int(node1.x-minx)][int(node1.y-miny)])
    return 0;


  return 1;
}



int find_min(vector<int> obs, int n)
{
  int min = obs[0];
  for(int i=1;i<n;i++)
  {
    if(min>obs[i])
    min=obs[i];
  }
  return min;
}
int find_max(vector<int> obs, int n)
{
  int max = obs[0];
  for(int i=1;i<n;i++)
  {
    if(max<obs[i])
    max=obs[i];
  }
//have to return max instead of min
  return max;
}
void calc_obstacle_map(vector<int> obsx, vector<int> obsy, int obsn, int reso, int rr)
{
  minx = 1;
  miny = 1;

  maxx = 101;
  maxy = 101;

  xwidth = maxx-minx;
  ywidth = maxy-miny;

  //size of map is 100 by 100
  
  for(int i=0;i<101;i++)
  {
  
  for(int j=0;j<=100;j++)
  obmap[i].pb(0);
  }

  for(int j=0;j<obsn;j++)
  {
      obmap[int(obsx[j]-minx)][int(obsy[j]-miny)]=1;
  }

  
}

void get_motion_model()
{
    // dx, dy, cost


    motion[0][0]=1;
    motion[0][1]=0;
    motion[0][2]=1;

    motion[1][0]=0;
    motion[1][1]=1;
    motion[1][2]=1;

    motion[2][0]=-1;
    motion[2][1]=0;
    motion[2][2]=1;

    motion[3][0]=0;
    motion[3][1]=-1;
    motion[3][2]=1;

    motion[4][0]=-1;
    motion[4][1]=-1;
    motion[4][2]=sqrt(2);

    motion[5][0]=-1;
    motion[5][1]=1;
    motion[5][2]=sqrt(2);

    motion[6][0]=1;
    motion[6][1]=-1;
    motion[6][2]=sqrt(2);

    motion[7][0]=1;
    motion[7][1]=1;
    motion[7][2]=sqrt(2);


}


double calc_heuristic(node n1, node n2)
{
    double w = 1.0;  //# weight of heuristic
    double d = w * sqrt((n1.x - n2.x)*(n1.x - n2.x) + (n1.y - n2.y)*(n1.y - n2.y));
    return d;
}

int main()
{
 //[10] = {1, 2, 2, 2, 2, 4, 4, 4, 4, 4}, obsyarr[10] = {6, 2, 3, 4, 6, 2, 4, 5, 6, 7};
  
    vector<int> ox;
    vector<int> oy;
    /*obsxarr.pb(1);obsyarr.pb(6);
    obsxarr.pb(2);obsyarr.pb(2);
    obsxarr.pb(2);obsyarr.pb(3);
    obsxarr.pb(2);obsyarr.pb(4);
    obsxarr.pb(2);obsyarr.pb(6);
    obsxarr.pb(4);obsyarr.pb(2);
    obsxarr.pb(4);obsyarr.pb(4);
    obsxarr.pb(4);obsyarr.pb(4);
    obsxarr.pb(4);obsyarr.pb(6);
    obsxarr.pb(4);obsyarr.pb(7);
*/    
   printf("Lets start working\n");
printf("Enter starting position coordinates");


double x_in, y_in,gx,gy;
cin >> x_in >> y_in;

printf("Enter goal coordinates");
cin >> gx >> gy;
    
    
   revise_obstacle_coordinates(ox,oy);

    path_planning(x_in, y_in, gx, gy,ox ,oy );
   
}

