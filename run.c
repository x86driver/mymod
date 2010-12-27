#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s [r/d] [0/1]\n", argv[0]);
		fprintf(stderr, "Example:\n");
		fprintf(stderr, "    %s r 0 rear camera out\n", argv[0]);
		fprintf(stderr, "    %s r 1 rear camera in\n", argv[0]);
		fprintf(stderr, "    %s d 0 driving recorder out\n", argv[0]);
		fprintf(stderr, "    %s d 1 driving recorder in\n", argv[0]);
		exit(1);
	}
	int device = (argv[1][0] == 'r' ? 0 : 1);
	int state = atoi(argv[2]);
	char amcmd[] = "/system/bin/env CLASSPATH=/system/framework/am.jar /system/bin/app_process /system/bin com.android.commands.am.Am broadcast";
	char *action[2] = {"com.garmin.android.apps.rearcamera.action.REAR_CAMERA_EVENT",
			"com.garmin.android.apps.drivingrecorder.action.DRIVING_RECORDER_EVENT"};
	char *extra[2] = {"com.garmin.android.apps.rearcamera.extra.REAR_CAMERA_STATE",
			"com.garmin.android.apps.drivingrecorder.extra.DRIVING_RECORDER_STATE"};
	char cmd[512];
	snprintf(cmd, sizeof(cmd), "%s -a %s --ei %s %d", amcmd, action[device], extra[device], state);
	system(cmd);
	return 0;
}

