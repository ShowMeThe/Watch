#include <jni.h>

#include <unistd.h>
#include <string.h>
#include<android/log.h>
#include "native-head.h"


const char *PATH  = "/data/data/com.example.ken.watch/protect.socket";
const char *userId;
int m_child;


void child_do_work() {
    //开启socket  服务端
    if (child_create_channel()) {
        child_listen_msg();
    }


}


/**
 * 内容 服务端
 */
void child_listen_msg(){
    //清空内容
    fd_set fdSet;
    struct  timeval time_out = {3,0};
    while (1){
        //重置
        FD_ZERO(&fdSet);
        FD_SET(m_child,&fdSet);

        //选择,默认+1
        int mSelect =  select(m_child+1,&fdSet,NULL,NULL,&time_out);

        if(mSelect > 0){
            //缓冲区域
            char buffer[256] = {0};

            //确定是指定当前的客户端
            if(FD_ISSET(m_child,&fdSet)){
                //阻塞函数
                int result =  read(m_child,buffer, sizeof(buffer));
                LOGE("userId %s" ,userId);
                LOGE("启动服务");
                //开启服务
                execl("am", "am", "startservice", "--user", userId,"-n",buffer, "com.example.ken.watch/com.example.ken.watch.ProcessService", (char*)NULL);
                break;
            }
        }

    }
}



/**
 * 服务端
 * @return
 */
int child_create_channel(){

    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);//创建一个socket句柄
    unlink(PATH);
    struct sockaddr_un addr;//指向了某个内存地址
    memset(&addr, 0, sizeof(sockaddr_un));//清空内存
    addr.sun_family = AF_LOCAL;
    int connfd = 0;
    strcpy(addr.sun_path, PATH);
    int i = bind(listenfd, (const sockaddr *) &addr, sizeof(sockaddr_un));
    if (i < 0) {
        LOGE("绑定错误");
        return 0;
    }

    listen(listenfd, 5);//最大监听5个
    //保证宿主进程连接成功
    while (1) {
        //阻塞式函数，返回客户端地址
        if ((connfd = accept(listenfd, NULL, NULL)) < 0) {//失败
            if (errno == EINTR) {
                continue;
            } else {
                LOGE("读取错误");
                return 0;
            }
        }
        m_child = connfd;
        LOGE("连接成功了");
        break;
    }
    return 1;
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_ken_watch_Watcher_createSocket(JNIEnv *env, jobject instance, jstring uid_) {
    userId = env->GetStringUTFChars(uid_, 0);
    pid_t pid = fork();
    //fork后下面会调用两次
    if (pid < 0) {

    } else if (pid == 0) {
        //子进程 我们的守护进程
        child_do_work();
    } else if (pid > 0) {
        //父进程
    }

    env->ReleaseStringUTFChars(uid_, userId);
}


extern "C"
JNIEXPORT void JNICALL
/**
 * 客户端连接
 * @param env
 * @param instance
 */
Java_com_example_ken_watch_Watcher_connectMonitor(JNIEnv *env, jobject instance) {

    //客户端 apk进程
    int socked;
    struct  sockaddr_un addr;
    while (1){
        LOGE("开始连接了");
        socked = socket(AF_LOCAL,SOCK_STREAM,0);
        if (socked<0){
            LOGE("连接失败");
            return;
        }

        memset(&addr,0, sizeof(sockaddr_un));
        addr.sun_family = AF_LOCAL;
        strcpy(addr.sun_path,PATH);

        if (connect(socked,(const sockaddr *)&addr, sizeof(sockaddr_un))<0){
            LOGE("连接失败");
            close(socked);
            sleep(1);
            continue;
        }
        LOGE("连接成功");
        break;
    }


}