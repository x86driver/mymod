#!/system/bin/sh
export CLASSPATH=/system/framework/am.jar
/system/bin/env CLASSPATH=/system/framework/am.jar /system/bin/app_process /system/bin com.android.commands.am.Am broadcast -a com.garmin.android.apps.drivingrecorder.action.DRIVING_RECORDER_EVENT --ei com.garmin.android.apps.drivingrecorder.extra.DRIVING_RECORDER_STATE 0
