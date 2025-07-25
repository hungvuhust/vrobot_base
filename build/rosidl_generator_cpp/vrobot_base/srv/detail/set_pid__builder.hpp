// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from vrobot_base:srv/SetPID.idl
// generated code does not contain a copyright notice

#ifndef VROBOT_BASE__SRV__DETAIL__SET_PID__BUILDER_HPP_
#define VROBOT_BASE__SRV__DETAIL__SET_PID__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "vrobot_base/srv/detail/set_pid__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace vrobot_base
{

namespace srv
{

namespace builder
{

class Init_SetPID_Request_motor_id
{
public:
  explicit Init_SetPID_Request_motor_id(::vrobot_base::srv::SetPID_Request & msg)
  : msg_(msg)
  {}
  ::vrobot_base::srv::SetPID_Request motor_id(::vrobot_base::srv::SetPID_Request::_motor_id_type arg)
  {
    msg_.motor_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::vrobot_base::srv::SetPID_Request msg_;
};

class Init_SetPID_Request_kd
{
public:
  explicit Init_SetPID_Request_kd(::vrobot_base::srv::SetPID_Request & msg)
  : msg_(msg)
  {}
  Init_SetPID_Request_motor_id kd(::vrobot_base::srv::SetPID_Request::_kd_type arg)
  {
    msg_.kd = std::move(arg);
    return Init_SetPID_Request_motor_id(msg_);
  }

private:
  ::vrobot_base::srv::SetPID_Request msg_;
};

class Init_SetPID_Request_ki
{
public:
  explicit Init_SetPID_Request_ki(::vrobot_base::srv::SetPID_Request & msg)
  : msg_(msg)
  {}
  Init_SetPID_Request_kd ki(::vrobot_base::srv::SetPID_Request::_ki_type arg)
  {
    msg_.ki = std::move(arg);
    return Init_SetPID_Request_kd(msg_);
  }

private:
  ::vrobot_base::srv::SetPID_Request msg_;
};

class Init_SetPID_Request_kp
{
public:
  Init_SetPID_Request_kp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetPID_Request_ki kp(::vrobot_base::srv::SetPID_Request::_kp_type arg)
  {
    msg_.kp = std::move(arg);
    return Init_SetPID_Request_ki(msg_);
  }

private:
  ::vrobot_base::srv::SetPID_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::vrobot_base::srv::SetPID_Request>()
{
  return vrobot_base::srv::builder::Init_SetPID_Request_kp();
}

}  // namespace vrobot_base


namespace vrobot_base
{

namespace srv
{

namespace builder
{

class Init_SetPID_Response_success
{
public:
  Init_SetPID_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::vrobot_base::srv::SetPID_Response success(::vrobot_base::srv::SetPID_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return std::move(msg_);
  }

private:
  ::vrobot_base::srv::SetPID_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::vrobot_base::srv::SetPID_Response>()
{
  return vrobot_base::srv::builder::Init_SetPID_Response_success();
}

}  // namespace vrobot_base

#endif  // VROBOT_BASE__SRV__DETAIL__SET_PID__BUILDER_HPP_
