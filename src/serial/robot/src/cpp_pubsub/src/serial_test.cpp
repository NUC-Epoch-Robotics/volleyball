#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <sstream>

class MotorCommandPublisher : public rclcpp::Node
{
public:
    MotorCommandPublisher() : Node("motor_command_publisher")
    {
        // 创建发布者，主题为 '/cmd_vel_smoothed'
        publisher_tail_ = this->create_publisher<std_msgs::msg::String>("/cmd_vel_smoothed", 10);
        // 创建定时器，每秒调用一次 publish_command 方法
        timer_tail = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&MotorCommandPublisher::publish_command_tail, this));

    }

private:
    void publish_command_tail()
    {
      auto message_tail = std_msgs::msg::String();
      message_tail.data = "tail";
      RCLCPP_INFO(this->get_logger(), " Publishing: '%s'", message_tail.data.c_str());
      publisher_tail_->publish(message_tail);


    }


    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_tail_; // 发布者指针
    rclcpp::TimerBase::SharedPtr timer_tail; // 定时器指针

};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv); // 初始化 ROS 2
    rclcpp::spin(std::make_shared<MotorCommandPublisher>()); // 创建并运行节点
    rclcpp::shutdown(); // 关闭 ROS 2
    return 0; // 返回 0
}
