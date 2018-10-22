//
// Created by lenovo on 2018/10/20.
//

#ifndef WATCH_NATIVE_HEAD_H
#define WATCH_NATIVE_HEAD_H

#endif //WATCH_NATIVE_HEAD_H

#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <linux/signal.h>
#include <android/log.h>
#define LOG_TAG "TAG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__)


void child_do_work();

int child_create_channel();

void child_listen_msg();




