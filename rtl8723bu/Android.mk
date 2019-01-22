#
# Copyright (C) 2018 The Android-x86 Open Source Project
#
# Licensed under the GNU General Public License Version 2 or later.
# You may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.gnu.org/licenses/gpl.html
#

LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)
# LOCAL_MODULE := $(notdir $(LOCAL_PATH))
LOCAL_MODULE := rtl8723bu
LOCAL_VENDOR_MODULE := true
EXTRA_KERNEL_MODULE_PATH_$(LOCAL_MODULE) := $(LOCAL_PATH)
