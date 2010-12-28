#!/system/bin/sh
export CLASSPATH=/system/framework/am.jar
/system/bin/env CLASSPATH=/system/framework/am.jar /system/bin/app_process /system/bin com.android.commands.am.Am broadcast -a com.garmin.android.apps.rearcamera.action.REAR_CAMERA_EVENT --ei com.garmin.android.apps.rearcamera.extra.REAR_CAMERA_STATE 0

