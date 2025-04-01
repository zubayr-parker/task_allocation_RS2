// so in order to have coords the robot can work with, we need a map of our arena.
// might have to look into nav2???
    // ask arya







#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <vector>
#include <cmath>
#include <set>

struct Point {
    double x, y;
    double distanceTo(const Point& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
};

struct TurtleBot {
    int id;
    Point position;
    Point goal;
};

class TurtleBotGoalAssignment : public rclcpp::Node {
public:
    TurtleBotGoalAssignment() : Node("turtlebot_goal_assignment") {
        // Initialize ROS 2 Publisher
        position_publisher_ = this->create_publisher<geometry_msgs::msg::Point>("turtlebot_positions", 10);
        assignment_publisher_ = this->create_publisher<std_msgs::msg::String>("turtlebot_goal_assignments", 10);
        
        // Initialize some TurtleBots and goals
        bots_ = {{0, {0, 0}}, {1, {1, 1}}};
        goals_ = {{3, 3}, {4, 4}, {5, 5}, {6, 6}};
        
        // Timer to call update function every 2 seconds
        timer_ = this->create_wall_timer(
            std::chrono::seconds(2), std::bind(&TurtleBotGoalAssignment::updatePositions, this));
    }

private:
    void assignGoals() {
        std::set<int> assignedGoals;  // To track goals already taken
        
        // Loop through pairs of goals
        for (size_t i = 0; i < goals_.size(); i += 2) {
            std::stringstream assignment_message;
            assignment_message << "Assigning goals (" << goals_[i].x << ", " << goals_[i].y << ") and (";
            if (i + 1 < goals_.size()) {
                assignment_message << goals_[i + 1].x << ", " << goals_[i + 1].y << ")\n";
            }

            // Assign goals to TurtleBots
            for (size_t j = 0; j < bots_.size(); ++j) {
                if (assignedGoals.count(i) == 0 && assignedGoals.count(i + 1) == 0) {
                    double dist_to_goal_1 = bots_[j].position.distanceTo(goals_[i]);
                    double dist_to_goal_2 = bots_[j].position.distanceTo(goals_[i + 1]);

                    // Choose the nearest goal
                    if (dist_to_goal_1 < dist_to_goal_2) {
                        bots_[j].goal = goals_[i];
                        assignedGoals.insert(i);
                        assignment_message << "TurtleBot " << bots_[j].id << " moved to goal at ("
                                           << goals_[i].x << ", " << goals_[i].y << ")\n";
                    } else {
                        bots_[j].goal = goals_[i + 1];
                        assignedGoals.insert(i + 1);
                        assignment_message << "TurtleBot " << bots_[j].id << " moved to goal at ("
                                           << goals_[i + 1].x << ", " << goals_[i + 1].y << ")\n";
                    }
                }
            }

            // Publish the assignment
            auto assignment_msg = std_msgs::msg::String();
            assignment_msg.data = assignment_message.str();
            assignment_publisher_->publish(assignment_msg);
        }
    }

    void updatePositions() {
        // Update positions to simulate moving towards the assigned goal
        for (auto& bot : bots_) {
            bot.position = bot.goal;  // In this simulation, they instantly reach their goal
        }

        // Publish updated positions
        for (const auto& bot : bots_) {
            auto position_msg = geometry_msgs::msg::Point();
            position_msg.x = bot.position.x;
            position_msg.y = bot.position.y;
            position_publisher_->publish(position_msg);
        }

        // After publishing positions, assign new goals
        assignGoals();
    }

    std::vector<TurtleBot> bots_;
    std::vector<Point> goals_;
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr position_publisher_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr assignment_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TurtleBotGoalAssignment>());
    rclcpp::shutdown();
    return 0;
}
