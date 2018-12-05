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

TARGET_EXTRA_KERNEL_MODULES += $(patsubst $(LOCAL_PATH)/%/Android.mk,%,$(wildcard $(LOCAL_PATH)/*/Android.mk))

include $(call all-subdir-makefiles)
