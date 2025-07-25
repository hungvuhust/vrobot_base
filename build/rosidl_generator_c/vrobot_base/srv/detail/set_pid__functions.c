// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from vrobot_base:srv/SetPID.idl
// generated code does not contain a copyright notice
#include "vrobot_base/srv/detail/set_pid__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
vrobot_base__srv__SetPID_Request__init(vrobot_base__srv__SetPID_Request * msg)
{
  if (!msg) {
    return false;
  }
  // kp
  // ki
  // kd
  // motor_id
  return true;
}

void
vrobot_base__srv__SetPID_Request__fini(vrobot_base__srv__SetPID_Request * msg)
{
  if (!msg) {
    return;
  }
  // kp
  // ki
  // kd
  // motor_id
}

bool
vrobot_base__srv__SetPID_Request__are_equal(const vrobot_base__srv__SetPID_Request * lhs, const vrobot_base__srv__SetPID_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // kp
  if (lhs->kp != rhs->kp) {
    return false;
  }
  // ki
  if (lhs->ki != rhs->ki) {
    return false;
  }
  // kd
  if (lhs->kd != rhs->kd) {
    return false;
  }
  // motor_id
  if (lhs->motor_id != rhs->motor_id) {
    return false;
  }
  return true;
}

bool
vrobot_base__srv__SetPID_Request__copy(
  const vrobot_base__srv__SetPID_Request * input,
  vrobot_base__srv__SetPID_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // kp
  output->kp = input->kp;
  // ki
  output->ki = input->ki;
  // kd
  output->kd = input->kd;
  // motor_id
  output->motor_id = input->motor_id;
  return true;
}

vrobot_base__srv__SetPID_Request *
vrobot_base__srv__SetPID_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vrobot_base__srv__SetPID_Request * msg = (vrobot_base__srv__SetPID_Request *)allocator.allocate(sizeof(vrobot_base__srv__SetPID_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(vrobot_base__srv__SetPID_Request));
  bool success = vrobot_base__srv__SetPID_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
vrobot_base__srv__SetPID_Request__destroy(vrobot_base__srv__SetPID_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    vrobot_base__srv__SetPID_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
vrobot_base__srv__SetPID_Request__Sequence__init(vrobot_base__srv__SetPID_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vrobot_base__srv__SetPID_Request * data = NULL;

  if (size) {
    data = (vrobot_base__srv__SetPID_Request *)allocator.zero_allocate(size, sizeof(vrobot_base__srv__SetPID_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = vrobot_base__srv__SetPID_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        vrobot_base__srv__SetPID_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
vrobot_base__srv__SetPID_Request__Sequence__fini(vrobot_base__srv__SetPID_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      vrobot_base__srv__SetPID_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

vrobot_base__srv__SetPID_Request__Sequence *
vrobot_base__srv__SetPID_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vrobot_base__srv__SetPID_Request__Sequence * array = (vrobot_base__srv__SetPID_Request__Sequence *)allocator.allocate(sizeof(vrobot_base__srv__SetPID_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = vrobot_base__srv__SetPID_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
vrobot_base__srv__SetPID_Request__Sequence__destroy(vrobot_base__srv__SetPID_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    vrobot_base__srv__SetPID_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
vrobot_base__srv__SetPID_Request__Sequence__are_equal(const vrobot_base__srv__SetPID_Request__Sequence * lhs, const vrobot_base__srv__SetPID_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!vrobot_base__srv__SetPID_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
vrobot_base__srv__SetPID_Request__Sequence__copy(
  const vrobot_base__srv__SetPID_Request__Sequence * input,
  vrobot_base__srv__SetPID_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(vrobot_base__srv__SetPID_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    vrobot_base__srv__SetPID_Request * data =
      (vrobot_base__srv__SetPID_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!vrobot_base__srv__SetPID_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          vrobot_base__srv__SetPID_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!vrobot_base__srv__SetPID_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
vrobot_base__srv__SetPID_Response__init(vrobot_base__srv__SetPID_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  return true;
}

void
vrobot_base__srv__SetPID_Response__fini(vrobot_base__srv__SetPID_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
}

bool
vrobot_base__srv__SetPID_Response__are_equal(const vrobot_base__srv__SetPID_Response * lhs, const vrobot_base__srv__SetPID_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  return true;
}

bool
vrobot_base__srv__SetPID_Response__copy(
  const vrobot_base__srv__SetPID_Response * input,
  vrobot_base__srv__SetPID_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  return true;
}

vrobot_base__srv__SetPID_Response *
vrobot_base__srv__SetPID_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vrobot_base__srv__SetPID_Response * msg = (vrobot_base__srv__SetPID_Response *)allocator.allocate(sizeof(vrobot_base__srv__SetPID_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(vrobot_base__srv__SetPID_Response));
  bool success = vrobot_base__srv__SetPID_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
vrobot_base__srv__SetPID_Response__destroy(vrobot_base__srv__SetPID_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    vrobot_base__srv__SetPID_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
vrobot_base__srv__SetPID_Response__Sequence__init(vrobot_base__srv__SetPID_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vrobot_base__srv__SetPID_Response * data = NULL;

  if (size) {
    data = (vrobot_base__srv__SetPID_Response *)allocator.zero_allocate(size, sizeof(vrobot_base__srv__SetPID_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = vrobot_base__srv__SetPID_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        vrobot_base__srv__SetPID_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
vrobot_base__srv__SetPID_Response__Sequence__fini(vrobot_base__srv__SetPID_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      vrobot_base__srv__SetPID_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

vrobot_base__srv__SetPID_Response__Sequence *
vrobot_base__srv__SetPID_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  vrobot_base__srv__SetPID_Response__Sequence * array = (vrobot_base__srv__SetPID_Response__Sequence *)allocator.allocate(sizeof(vrobot_base__srv__SetPID_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = vrobot_base__srv__SetPID_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
vrobot_base__srv__SetPID_Response__Sequence__destroy(vrobot_base__srv__SetPID_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    vrobot_base__srv__SetPID_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
vrobot_base__srv__SetPID_Response__Sequence__are_equal(const vrobot_base__srv__SetPID_Response__Sequence * lhs, const vrobot_base__srv__SetPID_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!vrobot_base__srv__SetPID_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
vrobot_base__srv__SetPID_Response__Sequence__copy(
  const vrobot_base__srv__SetPID_Response__Sequence * input,
  vrobot_base__srv__SetPID_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(vrobot_base__srv__SetPID_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    vrobot_base__srv__SetPID_Response * data =
      (vrobot_base__srv__SetPID_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!vrobot_base__srv__SetPID_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          vrobot_base__srv__SetPID_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!vrobot_base__srv__SetPID_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
