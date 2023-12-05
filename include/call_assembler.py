#!/usr/bin/env python3

import roslib; roslib.load_manifest('laser_assembler')
import rospy; from laser_assembler.srv import *
from sensor_msgs.msg import PointCloud

rospy.init_node("assemble_scan_client")
rate = rospy.Rate(0.5)

while not rospy.is_shutdown():

    rospy.wait_for_service("assemble_scans")

    try:
        assemble_scans = rospy.ServiceProxy('assemble_scans', AssembleScans)
        resp = assemble_scans(rospy.Time(0,0), rospy.get_rostime())
        print("Got cloud with %u points" % len(resp.cloud.points))

        pub=rospy.Publisher('laser_cloud', PointCloud, queue_size=10)
        pub.publish(resp.cloud)

    except rospy.ServiceException as e:
        print("Service call failed: %s"%e)

    rate.sleep()