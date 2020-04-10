#  Path Planning Simulation in turtlebot3
**The main aim of this repo to create a node for our customised planner to test the software on turtlebot3.**

## Process:
1. First we have to launch turtlebot3 on gazebo :
```
roslaunch turtlebot3_gazebo turtlebot3_world.launch

```
2. Then launch turtlebot3 navigation :
```
roslaunch turtlebot3_navigation turtlebot3_navigation.launch

```
3. Then launch gmapping package in turtlebot:
```
roslanch turtlebot3_slam turtlebot3_gmapping.launch
```
4. Open another terminal:
````
cd catkin_ws/src
git clone https://github.com/TeamARES/PathPlanningSimulation
cd ..
catkin_make
````
5. To finally run the listener node that records all the useful data from turtlebot
```
rosrun listener listener 2
```
## Important Conclusions:
1. It is noted in the turtlebot3 simulation the robot is always centered at the local costmap grid.This is also verified by taking real gps co-ordinate , origin of the map and the resolution of the grid.

2. In turtlebot3 width and hieght of the local costmap is 60*60

3. In turtlebot3 res of the local costmap is 0.05

4. to calculate real position of any object on the grid we have to use the formula:
*real x = origin.position.x + (x_co-ordinate of grid) x resolution*

5. Local costmap will give probabilities of wheather a point is occupied of not in (0-100).
