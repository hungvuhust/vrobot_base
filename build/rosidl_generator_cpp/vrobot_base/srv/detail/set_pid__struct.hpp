// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from vrobot_base:srv/SetPID.idl
// generated code does not contain a copyright notice

#ifndef VROBOT_BASE__SRV__DETAIL__SET_PID__STRUCT_HPP_
#define VROBOT_BASE__SRV__DETAIL__SET_PID__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__vrobot_base__srv__SetPID_Request __attribute__((deprecated))
#else
# define DEPRECATED__vrobot_base__srv__SetPID_Request __declspec(deprecated)
#endif

namespace vrobot_base
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetPID_Request_
{
  using Type = SetPID_Request_<ContainerAllocator>;

  explicit SetPID_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->kp = 0.0f;
      this->ki = 0.0f;
      this->kd = 0.0f;
      this->motor_id = 0;
    }
  }

  explicit SetPID_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->kp = 0.0f;
      this->ki = 0.0f;
      this->kd = 0.0f;
      this->motor_id = 0;
    }
  }

  // field types and members
  using _kp_type =
    float;
  _kp_type kp;
  using _ki_type =
    float;
  _ki_type ki;
  using _kd_type =
    float;
  _kd_type kd;
  using _motor_id_type =
    uint8_t;
  _motor_id_type motor_id;

  // setters for named parameter idiom
  Type & set__kp(
    const float & _arg)
  {
    this->kp = _arg;
    return *this;
  }
  Type & set__ki(
    const float & _arg)
  {
    this->ki = _arg;
    return *this;
  }
  Type & set__kd(
    const float & _arg)
  {
    this->kd = _arg;
    return *this;
  }
  Type & set__motor_id(
    const uint8_t & _arg)
  {
    this->motor_id = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    vrobot_base::srv::SetPID_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const vrobot_base::srv::SetPID_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      vrobot_base::srv::SetPID_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      vrobot_base::srv::SetPID_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__vrobot_base__srv__SetPID_Request
    std::shared_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__vrobot_base__srv__SetPID_Request
    std::shared_ptr<vrobot_base::srv::SetPID_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetPID_Request_ & other) const
  {
    if (this->kp != other.kp) {
      return false;
    }
    if (this->ki != other.ki) {
      return false;
    }
    if (this->kd != other.kd) {
      return false;
    }
    if (this->motor_id != other.motor_id) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetPID_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetPID_Request_

// alias to use template instance with default allocator
using SetPID_Request =
  vrobot_base::srv::SetPID_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace vrobot_base


#ifndef _WIN32
# define DEPRECATED__vrobot_base__srv__SetPID_Response __attribute__((deprecated))
#else
# define DEPRECATED__vrobot_base__srv__SetPID_Response __declspec(deprecated)
#endif

namespace vrobot_base
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetPID_Response_
{
  using Type = SetPID_Response_<ContainerAllocator>;

  explicit SetPID_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit SetPID_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    vrobot_base::srv::SetPID_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const vrobot_base::srv::SetPID_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      vrobot_base::srv::SetPID_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      vrobot_base::srv::SetPID_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__vrobot_base__srv__SetPID_Response
    std::shared_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__vrobot_base__srv__SetPID_Response
    std::shared_ptr<vrobot_base::srv::SetPID_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetPID_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetPID_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetPID_Response_

// alias to use template instance with default allocator
using SetPID_Response =
  vrobot_base::srv::SetPID_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace vrobot_base

namespace vrobot_base
{

namespace srv
{

struct SetPID
{
  using Request = vrobot_base::srv::SetPID_Request;
  using Response = vrobot_base::srv::SetPID_Response;
};

}  // namespace srv

}  // namespace vrobot_base

#endif  // VROBOT_BASE__SRV__DETAIL__SET_PID__STRUCT_HPP_
