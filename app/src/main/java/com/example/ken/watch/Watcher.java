package com.example.ken.watch;

/**
 * Created by Ken on 2018/10/20.
 */
public class Watcher {

    static {
        System.loadLibrary("native-lib");
    }

    public native void createSocket(String uid);

    public native void connectMonitor ();

}
