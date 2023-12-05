#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>

tf2_ros::Buffer tfBuffer;
static tf2_ros::TransformBroadcaster br;
static tf2_ros::StaticTransformBroadcaster sbr;
geometry_msgs::TransformStamped transformStamped;

// Transformation from map to odom_init
transformStamped.header.stamp = ros::Time::now();
transformStamped.header.frame_id = "map";
transformStamped.child_frame_id = "odom_init";
transformStamped.transform.translation.x = odom_init_pos.pose.position.x;
transformStamped.transform.translation.y = odom_init_pos.pose.position.y;
transformStamped.transform.translation.z = odom_init_pos.pose.position.z;
transformStamped.transform.rotation = odom_init_pos.pose.orientation;
sbr.sendTransform(transformStamped);

// Transformation from odom_init to drone_vision
transformStamped.header.stamp = ros::Time::now();
transformStamped.header.frame_id = "odom_init";
transformStamped.child_frame_id = "current_position";
transformStamped.transform.translation.x = vo_position.pose.pose.position.x;
transformStamped.transform.translation.y = vo_position.pose.pose.position.y;
transformStamped.transform.translation.z = vo_position.pose.pose.position.z;
transformStamped.transform.rotation = vo_position.pose.pose.orientation;
br.sendTransform(transformStamped);

if (send_vision_estimate) {
  try {
    transformStamped = tfBuffer.lookupTransform("map", "current_position", ros::Time(0));
    vision_pos_ENU.pose.position.x = transformStamped.transform.translation.x;
    vision_pos_ENU.pose.position.y = transformStamped.transform.translation.y;
    vision_pos_ENU.pose.position.z = transformStamped.transform.translation.z;
    vision_pos_ENU.pose.orientation = transformStamped.transform.rotation;

    vision_pos_pub.publish(vision_pos_ENU);
    ros::spinOnce();

    }
    catch (tf2::TransformException &ex){
    ROS_ERROR("%s",ex.what());
  }
}