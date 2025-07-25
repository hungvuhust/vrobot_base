// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from vrobot_base:srv/SetPID.idl
// generated code does not contain a copyright notice

#ifndef VROBOT_BASE__SRV__DETAIL__SET_PID__STRUCT_H_
#define VROBOT_BASE__SRV__DETAIL__SET_PID__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/SetPID in the package vrobot_base.
typedef struct vrobot_base__srv__SetPID_Request
{
  float kp;
  float ki;
  float kd;
  uint8_t motor_id;
} vrobot_base__srv__SetPID_Request;

// Struct for a sequence of vrobot_base__srv__SetPID_Request.
typedef struct vrobot_base__srv__SetPID_Request__Sequence
{
  vrobot_base__srv__SetPID_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} vrobot_base__srv__SetPID_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/SetPID in the package vrobot_base.
typedef struct vrobot_base__srv__SetPID_Response
{
  bool success;
} vrobot_base__srv__SetPID_Response;

// Struct for a sequence of vrobot_base__srv__SetPID_Response.
typedef struct vrobot_base__srv__SetPID_Response__Sequence
{
  vrobot_base__srv__SetPID_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} vrobot_base__srv__SetPID_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VROBOT_BASE__SRV__DETAIL__SET_PID__STRUCT_H_
