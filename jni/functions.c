#include <jni.h>
#include <android/log.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "suinput.h"

#define LOGTAG "jni"

int uinput_fd = -1;

jboolean Java_com_daohoangson_android_uinput_NativeMethods_open(JNIEnv* env) {

	struct input_id id = { BUS_VIRTUAL, 1, 1, 1 };

	// chmod the /dev/uinput node so we cant open it
	// by default, the permission is 0660
	system("su -c \"chmod 0666 /dev/uinput\"");

	uinput_fd = suinput_open("uinputdemo", &id);

	__android_log_print(ANDROID_LOG_VERBOSE, LOGTAG, "suinput_open() = %d", uinput_fd);

	if (uinput_fd == -1) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

jboolean Java_com_daohoangson_android_uinput_NativeMethods_close(JNIEnv* env) {

	int result = suinput_close(uinput_fd);

	// restore the node permission for security reason
	system("su -c \"chmod 0660 /dev/uinput\"");

	__android_log_print(ANDROID_LOG_VERBOSE, LOGTAG, "suinput_close(%d) = %d", uinput_fd, result);
	uinput_fd = -1;

	if (result == -1) {
		return JNI_FALSE;
	} else {
		return JNI_TRUE;
	}
}

jboolean Java_com_daohoangson_android_uinput_NativeMethods_keyPress(JNIEnv* env,
		jint uinput_fd, jint key, jboolean shift, jboolean alt) {

	if (shift) {
		suinput_press(uinput_fd, 42);
	}
	if (alt) {
		suinput_press(uinput_fd, 56);
	}

	suinput_click(uinput_fd, key);

	if (alt) {
		suinput_release(uinput_fd, 56);
	}
	if (shift) {
		suinput_release(uinput_fd, 42);
	}

	return JNI_TRUE;
}

jboolean Java_com_daohoangson_android_uinput_NativeMethods_pointerTap(
		JNIEnv* env, jint uinput_fd, jint x, jint y) {

	suinput_move_pointer(uinput_fd, x, y);

	suinput_click(uinput_fd, BTN_LEFT);

	return JNI_TRUE;
}
