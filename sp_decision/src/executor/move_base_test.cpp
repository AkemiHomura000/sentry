#include <ros/ros.h>
#include <nav_msgs/GetPlan.h>
#include <geometry_msgs/PoseStamped.h>
#include <string>
#include <boost/foreach.hpp>
#define forEach BOOST_FOREACH
void fillPathRequest(nav_msgs::GetPlan::Request &request, double x, double y)
{
    request.start.header.frame_id = "map";
    request.start.pose.position.x = 0;      // 初始位置x坐标
    request.start.pose.position.y = 0;      // 初始位置y坐标
    request.start.pose.orientation.w = 1.0; // 方向
    request.goal.header.frame_id = "map";
    request.goal.pose.position.x = x; // 终点坐标
    request.goal.pose.position.y = y;
    request.goal.pose.orientation.w = 1.0;
    request.tolerance = 0.2; // 如果不能到达目标，最近可到的约束
}
// 路线规划结果回调
void callPlanningService(ros::ServiceClient &serviceClient, nav_msgs::GetPlan &srv)
{
    // Perform the actual path planner call
    // 执行实际路径规划器
    if (serviceClient.call(srv))
    {
        // srv.response.plan.poses 为保存结果的容器，遍历取出
        if (!srv.response.plan.poses.empty())
        {
            forEach(const geometry_msgs::PoseStamped &p, srv.response.plan.poses)
            {
                ROS_INFO("x = %f, y = %f", p.pose.position.x, p.pose.position.y);
            }
        }
        else
        {
            ROS_WARN("Got empty plan");
        }
    }
    else
    {
        ROS_ERROR("Failed to call service %s - is the robot moving?",
                  serviceClient.getService().c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "make_plan_node");
    ros::NodeHandle nh;
    // Init service query for make plan
    // 初始化路径规划服务，服务名称为"move_base_node/make_plan"
    std::string service_name = "move_base_node/make_plan";
    // 等待服务空闲，如果已经在运行这个服务，会等到运行结束。
    while (!ros::service::waitForService(service_name, ros::Duration(3.0)))
    {
        ROS_INFO("Waiting for service move_base/make_plan to become available");
    }
    /*初始化客户端，(nav_msgs/GetPlan)
    Allows an external user to ask for a plan to a given pose from move_base without causing move_base to execute that plan.
    允许用户从move_base 请求一个plan，并不会导致move_base 执行此plan
    */
    ros::ServiceClient serviceClient = nh.serviceClient<nav_msgs::GetPlan>(service_name, true);
    if (!serviceClient)
    {
        ROS_FATAL("Could not initialize get plan service from %s",
                  serviceClient.getService().c_str());
        return -1;
    }
    while (ros::ok)
    {
        std::string userinput;
        double userorder[2];
        std::cout << "x,y?" << std::endl;
        std::getline(std::cin, userinput);
        std::istringstream userstr(userinput);
        for (int j = 0; j < 2; j++)
        {
            userstr >> userorder[j];
        }
        nav_msgs::GetPlan srv;
        // 请求服务：规划路线
        fillPathRequest(srv.request,userorder[0],userorder[1]);
        if (!serviceClient)
        {
            ROS_FATAL("Persistent service connection to %s failed",
                      serviceClient.getService().c_str());
            return -1;
        }
        ROS_INFO("conntect to %s", serviceClient.getService().c_str());
        callPlanningService(serviceClient, srv);
    }
}
