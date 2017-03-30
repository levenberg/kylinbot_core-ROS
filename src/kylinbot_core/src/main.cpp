//
// Created by kelfor on 3/27/17.
//
#include <ros/ros.h>
#include<tf/transform_broadcaster.h>
#include "std_msgs/Int8.h"
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"
#include<geometry_msgs/Twist.h>
//#include <main.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include"asp.h"
#include "serial.h"
#define MAF_BUF_LEN 20

bool excute_state=false;     //indicate the excute state of close loop

void SystemInit()
{
                  //Odometry init
                  //set the original pos of ramp
}

void state_callback(const std_msgs::Bool &state)
{
  excute_state=state.data;
}
bool go_near_cube()
{
    //For Demo Use
    //Go straight, 2m ahead;
    return true;//TODO:
}
bool grab_cube()
{
    return true;//TODO:
}
bool fold_cubes()
{
    return true;//TODO:
}
int main(int argc, char ** argv)
{
    ros::init(argc, argv, "kylinbot_main");
    ros::NodeHandle n;

    

    ros::Rate r(100);
    SystemInit();     
    std_msgs::Int8 detectionMode;
    ros::Publisher pub_detectionMode = n.advertise<std_msgs::Int8>("kylinbot/detectioMode",100);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("kylinbot/cmd_vel",100);
    ros::Subscriber sub_state = n.subscribe("kylinbot/state",100, state_callback);
  //  current_time = ros::Time::now();
//    last_time = ros::Time::now();
    geometry_msgs::Twist cmd_vel;
    int cmd_vel_source = 1;
    int workState = 0;
    detectionMode.data=0;
    
    while(ros::ok())
    {
      
      pub_detectionMode.publish(detectionMode);


        
        switch (workState)
        {
            case 0: //
	      detectionMode.data=0;
	      cmd_vel.linear.x=0;
	      cmd_vel.linear.y=0;
	      cmd_vel.linear.z=1.57*1000;
	      cmd_vel.angular.x=0;
	      cmd_vel.angular.y=0;
	      cmd_vel.angular.z=0;
	      pub.publish(cmd_vel);    //90°
	      if(excute_state)
	      {
		workState=1;
		excute_state=false;
	      }
                //;//Ignore the cmd_vel from image processing.
                if(go_near_cube()) //If this is a loop, make sure you call ros::spinOnce()!
                    workState = 1;
                break;
            case 1:
	      detectionMode.data=1;
                if(grab_cube())
                    workState = 2;
                break;
            case 2:
                break;
            case 3:
                break;
            case 100:
                fold_cubes();
                break;
            default:
                break;
        }
        ros::spinOnce();
        r.sleep();
    }

    return 0;
}