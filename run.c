#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("run lo..\n");
//	system("/system/bin/env CLASSPATH=/system/framework/am.jar /system/bin/app_process /system/bin com.android.commands.am.Am start -a android.intent.action.MAIN -c android.intent.category.LAUNCHER -f 0x10200000 -n org.credil.helloworldservice/.HelloWorld");
	system("/system/bin/env CLASSPATH=/system/framework/am.jar /system/bin/app_process /system/bin com.android.commands.am.Am broadcast -a com.garmin.android.apps.rearcamera.action.REAR_CAMERA_EVENT --ei com.garmin.android.apps.rearcamera.extra.REAR_CAMERA_STATE 0");
	printf("run end..\n");
	return 0;
}

