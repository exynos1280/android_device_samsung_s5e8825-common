/*
 * Copyright (C) The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LIBINIT_DEVICE_H
#define LIBINIT_DEVICE_H

#include <string>
#include <vector>

typedef struct device_info {
  std::string codename;
  std::string full_name;
  std::string nfc_chip;
  bool adaptive_rr = false;
  bool side_fp = false;
  bool udfps = false;
} device_info_t;

void search_device(const std::vector<device_info_t> devices);

void set_device_props(const device_info_t device);

#endif // LIBINIT_DEVICE_H
