// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from vrobot_base:srv/SetPID.idl
// generated code does not contain a copyright notice

#ifndef VROBOT_BASE__SRV__DETAIL__SET_PID__TRAITS_HPP_
#define VROBOT_BASE__SRV__DETAIL__SET_PID__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "vrobot_base/srv/detail/set_pid__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace vrobot_base
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetPID_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: kp
  {
    out << "kp: ";
    rosidl_generator_traits::value_to_yaml(msg.kp, out);
    out << ", ";
  }

  // member: ki
  {
    out << "ki: ";
    rosidl_generator_traits::value_to_yaml(msg.ki, out);
    out << ", ";
  }

  // member: kd
  {
    out << "kd: ";
    rosidl_generator_traits::value_to_yaml(msg.kd, out);
    out << ", ";
  }

  // member: motor_id
  {
    out << "motor_id: ";
    rosidl_generator_traits::value_to_yaml(msg.motor_id, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetPID_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: kp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "kp: ";
    rosidl_generator_traits::value_to_yaml(msg.kp, out);
    out << "\n";
  }

  // member: ki
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ki: ";
    rosidl_generator_traits::value_to_yaml(msg.ki, out);
    out << "\n";
  }

  // member: kd
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "kd: ";
    rosidl_generator_traits::value_to_yaml(msg.kd, out);
    out << "\n";
  }

  // member: motor_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "motor_id: ";
    rosidl_generator_traits::value_to_yaml(msg.motor_id, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetPID_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace vrobot_base

namespace rosidl_generator_traits
{

[[deprecated("use vrobot_base::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const vrobot_base::srv::SetPID_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  vrobot_base::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use vrobot_base::srv::to_yaml() instead")]]
inline std::string to_yaml(const vrobot_base::srv::SetPID_Request & msg)
{
  return vrobot_base::srv::to_yaml(msg);
}

template<>
inline const char * data_type<vrobot_base::srv::SetPID_Request>()
{
  return "vrobot_base::srv::SetPID_Request";
}

template<>
inline const char * name<vrobot_base::srv::SetPID_Request>()
{
  return "vrobot_base/srv/SetPID_Request";
}

template<>
struct has_fixed_size<vrobot_base::srv::SetPID_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<vrobot_base::srv::SetPID_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<vrobot_base::srv::SetPID_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace vrobot_base
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetPID_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetPID_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetPID_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace vrobot_base

namespace rosidl_generator_traits
{

[[deprecated("use vrobot_base::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const vrobot_base::srv::SetPID_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  vrobot_base::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use vrobot_base::srv::to_yaml() instead")]]
inline std::string to_yaml(const vrobot_base::srv::SetPID_Response & msg)
{
  return vrobot_base::srv::to_yaml(msg);
}

template<>
inline const char * data_type<vrobot_base::srv::SetPID_Response>()
{
  return "vrobot_base::srv::SetPID_Response";
}

template<>
inline const char * name<vrobot_base::srv::SetPID_Response>()
{
  return "vrobot_base/srv/SetPID_Response";
}

template<>
struct has_fixed_size<vrobot_base::srv::SetPID_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<vrobot_base::srv::SetPID_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<vrobot_base::srv::SetPID_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<vrobot_base::srv::SetPID>()
{
  return "vrobot_base::srv::SetPID";
}

template<>
inline const char * name<vrobot_base::srv::SetPID>()
{
  return "vrobot_base/srv/SetPID";
}

template<>
struct has_fixed_size<vrobot_base::srv::SetPID>
  : std::integral_constant<
    bool,
    has_fixed_size<vrobot_base::srv::SetPID_Request>::value &&
    has_fixed_size<vrobot_base::srv::SetPID_Response>::value
  >
{
};

template<>
struct has_bounded_size<vrobot_base::srv::SetPID>
  : std::integral_constant<
    bool,
    has_bounded_size<vrobot_base::srv::SetPID_Request>::value &&
    has_bounded_size<vrobot_base::srv::SetPID_Response>::value
  >
{
};

template<>
struct is_service<vrobot_base::srv::SetPID>
  : std::true_type
{
};

template<>
struct is_service_request<vrobot_base::srv::SetPID_Request>
  : std::true_type
{
};

template<>
struct is_service_response<vrobot_base::srv::SetPID_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // VROBOT_BASE__SRV__DETAIL__SET_PID__TRAITS_HPP_
