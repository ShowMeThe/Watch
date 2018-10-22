package com.example.ken.watch;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Process;
import android.os.SystemClock;
import android.support.annotation.Nullable;
import android.util.Log;
import android.widget.Toast;

import java.sql.Time;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Ken on 2018/10/20.
 */
public class ProcessService  extends Service{

    private int i = 0;

    @SuppressLint("HandlerLeak")
    private Handler mHandler = new Handler(){
        public void handleMessage(Message msg){
            switch (msg.what){
                case 1:
                    Toast.makeText(ProcessService.this,"能关了我再说", Toast.LENGTH_SHORT).show();
                    break;
                default:
            }
        }
    };


    @Override
    public void onCreate() {
        super.onCreate();

        Watcher watcher = new Watcher();
        watcher.createSocket(String.valueOf(Process.myUid()));
        watcher.connectMonitor();
        Timer timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.e("TestServer","服务启动 " + i);
                i++;
            }
        },0,3*1000);

        new Thread(){
            public void run(){
                while (true){
                    mHandler.sendEmptyMessage(1);
                    SystemClock.sleep(5000);
                }
            }
        }.start();

    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
