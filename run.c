#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
        char * const envp[] = {
                "PATH=/sbin:/system/sbin:/system/bin:/system/xbin",
		"CLASSPATH=/system/framework/am.jar",
                NULL,};

        char *argv[] = {
		"/system/bin/env",
		"CLASSPATH=/system/framework/am.jar",
                "/system/bin/app_process",
		"/system/bin",
		"com.android.commands.am.Am start -n com.android.camera/com.android.camera.VideoCamera",
                NULL,};


//	char *argv[] = {
//		"/system/a.sh",
//		NULL,};

	printf("Before...\n");
//	int ret = execle(argv[0], argv[0], argv[1], argv[2], argv[3], argv[4], NULL, envp);
//	if (ret < 0)
//		perror("execve");
	printf("Done...\n");

	printf("run lo..\n");
	system("/system/bin/env CLASSPATH=/system/framework/am.jar /system/bin/app_process /system/bin com.android.commands.am.Am start -n com.android.camera/com.android.camera.VideoCamera");
	printf("run end..\n");
	return 0;
}

