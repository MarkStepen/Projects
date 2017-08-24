package com.android.server;

import android.os.MarkService;

public class MarkService extends MarkService.Stub{
    private static final String TAG = "MarkService";

    /* constructor method */
    public MarkService(){
        native_markOpen();
    }

    /* get datas form DHT11 */
    public byte[] markGetDatas() throws android.os.RemoteException  
    {  
        return native_markGetDatas();  
    }

  
    /* implement native methods */
    public native static byte[] native_markGetDatas(); // get datas
    public native static int 	native_markOpen();    // open dht11 device
    public native static void 	native_markClose();  // close dht11 device
      
}

 
