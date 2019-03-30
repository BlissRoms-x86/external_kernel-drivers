#
# Copyright (C) 2009-2018 Bliss OS x86 Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#

LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := tpt10_bat
LOCAL_VENDOR_MODULE := true
EXTRA_KERNEL_MODULE_PATH_$(LOCAL_MODULE) := $(LOCAL_PATH)
