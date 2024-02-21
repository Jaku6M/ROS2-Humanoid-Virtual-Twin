#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include <cmath>

class LegTrajectoryNode : public rclcpp::Node
{
public:
    LegTrajectoryNode() : Node("leg_trajectory_node")
    {
        publisher_ = create_publisher<sensor_msgs::msg::JointState>("/joint_states", 10);
        timer_ = create_wall_timer(std::chrono::milliseconds(100), std::bind(&LegTrajectoryNode::publish_joint_states, this));

        // Initialize member variables
        time_ = 0.0;
    }

private:
    void publish_joint_states()
    {
        auto joint_state_msg = std::make_unique<sensor_msgs::msg::JointState>();
        joint_state_msg->header.stamp = now();
        joint_state_msg->name = {"LTz", "LTx", "LTy", "LSy", "LFy", "LFx"};

        // Generate sinusoidal trajectory
        double amplitude = 0.1;
        double frequency = 1.0;
        joint_state_msg->position = {
            amplitude * std::sin(frequency * time_),
            amplitude * std::cos(frequency * time_),
            amplitude * std::sin(frequency * time_),
            amplitude * std::cos(frequency * time_),
            amplitude * std::sin(frequency * time_),
            amplitude * std::cos(frequency * time_)
        };

        joint_state_msg->velocity = {};  // You can set velocities if needed
        joint_state_msg->effort = {};    // You can set efforts if needed

        // Update time for the next iteration
        time_ += 0.1;

        publisher_->publish(std::move(joint_state_msg));
        RCLCPP_INFO(get_logger(), "Publishing joint states");
    }

    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    // Member variable for time in seconds
    double time_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LegTrajectoryNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}