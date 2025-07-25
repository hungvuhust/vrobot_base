#include "rclcpp/rclcpp.hpp"
#include "vrobot_base/diff_controller.hpp"


int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<vrobot_base::DiffController>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}