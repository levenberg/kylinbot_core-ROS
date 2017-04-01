//
// Created by kelfor on 3/27/17.
//
#include <ros/ros.h>
#include<tf/transform_broadcaster.h>
#include "std_msgs/Int8.h"
#include "std_msgs/Int32.h"
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
  int i=0;     //wait for other node initilization
  while(i++<1000)
    ;
}

void state_callback(const std_msgs::Bool &state)
{
  excute_state=state.data;
}

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "kylinbot_main");
    ros::NodeHandle n;

    geometry_msgs::Twist cmd_vel;
    std_msgs::Int8 detectionMode;
    std_msgs::Int32 cmd_velgrasp; 
    int workState = 0;
    detectionMode.data=0;
    
    ros::Rate r(100);
      

    ros::Publisher pub_detectionMode = n.advertise<std_msgs::Int8>("kylinbot/detectioMode",100);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel",100);    //Navigation
    ros::Publisher pub_velsource =n.advertise<std_msgs::Int32>("kylinbot/cmd_graspvel",100);
    ros::Subscriber sub_state = n.subscribe("kylinbot/state",100, state_callback);
  //  current_time = ros::Time::now();
//    last_time = ros::Time::now();
    SystemInit();   
    
    while(ros::ok())
    {
      pub_velsource.publish(cmd_velgrasp);
      pub_detectionMode.publish(detectionMode);


        
        switch (workState)
        {
            case 0: // 90°，init the grasp
	      cmd_velgrasp.data=0;      
	      detectionMode.data=0;
	      cmd_vel.linear.x=0;
	      cmd_vel.linear.y=0;   //grasp position
	      cmd_vel.linear.z=0;   //
	      cmd_vel.angular.x=314;   //open paws
	      cmd_vel.angular.y=1.57*1000;
	      cmd_vel.angular.z=0;
	      pub.publish(cmd_vel);    //90°
	      if(excute_state)
	      {
		workState=1;
		excute_state=false;
	      }
	      break;

            case 1:    //start detect squares and move
	      cmd_velgrasp.data = 0;
	      detectionMode.data = 1;
	      if(excute_state)  //TODO: distance from box
	      {
		detectionMode.data = 2;
		if(excute_state) //TODO: in the line
		{
		  detectionMode.data = 0;
		  cmd_vel.linear.x=0;
	          cmd_vel.linear.y=0;   //grasp position
	          cmd_vel.linear.z=100000;   //
	          cmd_vel.angular.x=314;   //open paws
	          cmd_vel.angular.y=1.57*1000;
	          cmd_vel.angular.z=100;   //speed 
	          pub.publish(cmd_vel); 
		  if(excute_state)  //TODO:near 
		{
		  cmd_vel.angular.x=2199;   //open paws
		  pub.publish(cmd_vel); 
		}
		  if(excute_state)
		  {
		    cmd_vel.linear.y=500;
		    pub.publish(cmd_vel); 
		  }
		  if(excute_state)
		    workState=2;
		}
	      }

                break;
	    case 2:  //TODO: go to the original position
	      detectionMode.data=0;
	      cmd_vel.linear.x=0;
	      cmd_vel.linear.z=0;   //
	      cmd_vel.angular.y=0;
	      pub.publish(cmd_vel);  
	      if(excute_state)
		workState=3;
                break;
            case 3:
	      detectionMode.data=0;
	      cmd_vel.linear.x=0;
	      cmd_vel.linear.z=3485;   //
	      cmd_vel.angular.y=0;
	      pub.publish(cmd_vel); 
	      if(excute_state)
	      {
		cmd_vel.linear.y=0;
		if(excute_state)
		{
		  cmd_vel.angular.x=314;
		  if(excute_state)
		    workState=4;
		}
		
	      }
                break;
	    case 4:
	      detectionMode.data=0;
	      cmd_vel.linear.x=0;
	      cmd_vel.linear.z=0;   //
	      cmd_vel.angular.y=0;
	      pub.publish(cmd_vel); 
	      if(excute_state)
		workState=0;
	      break;
            case 100:
                
                break;
            default:
                break;
        }
        ros::spinOnce();
        r.sleep();
    }

    return 0;
}