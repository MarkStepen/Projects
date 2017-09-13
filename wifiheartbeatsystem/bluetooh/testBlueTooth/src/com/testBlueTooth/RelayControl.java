package com.testBlueTooth;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;


public class RelayControl extends Activity {	
	
	public static boolean isRecording = false;// 线程控制标记
	
	
	private Button button1,button2,button3,button4,button5,releaseCtrl,btBack,btSend;
	
	private OutputStream outStream = null;
	
	private EditText _txtRead,_txtSend ;
	
	private ConnectedThread manageThread;
	private Handler mHandler;
	
	private String  encodeType ="GBK";
	
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.relaycontrol);		
		
		//接收线程启动
		manageThread=new ConnectedThread();
		mHandler  = new MyHandler();
		manageThread.Start();
		
		findMyView();
		
		setMyViewListener(); 
        	
		setTitle("返回前需先关闭socket连接");
		//接收区不可见
		_txtRead.setCursorVisible(false);      //设置输入框中的光标不可见
		_txtRead.setFocusable(false);           //无焦点

	}

	private void findMyView() {
		
		button1=(Button) findViewById(R.id.button1);
		button2=(Button) findViewById(R.id.button2);
		button3=(Button) findViewById(R.id.button3);
		button4=(Button) findViewById(R.id.button4);
		button5=(Button) findViewById(R.id.button5);
		
		releaseCtrl=(Button)findViewById(R.id.release);
		btBack=(Button) findViewById(R.id.btBack);
		btSend=(Button) findViewById(R.id.btSend);		
      
		_txtRead = (EditText) findViewById(R.id.etShow);
		_txtRead .setMovementMethod(ScrollingMovementMethod.getInstance());

		_txtSend = (EditText) findViewById(R.id.etSend);
	//	_txtSend .setMovementMethod(ScrollingMovementMethod.getInstance());
	}

	private void setMyViewListener() {
		// 监听Button
		encodeType="GBK";//设置为默认选中为 PC通信
		
		button1.setOnClickListener(new ClickEvent());
		button2.setOnClickListener(new ClickEvent());
		button3.setOnClickListener(new ClickEvent());
		button4.setOnClickListener(new ClickEvent());
		
		releaseCtrl.setOnClickListener(new ClickEvent());
		btBack.setOnClickListener(new ClickEvent());
		btSend.setOnClickListener(new ClickEvent());
	}

	@Override
	public void onDestroy()  
    {  
		
		try {
			testBlueTooth.btSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
        super.onDestroy();  
    }
	
	private	class ClickEvent implements View.OnClickListener {
		@Override
		public void onClick(View v) {

			if (v == releaseCtrl)// 释放连接
			{
				try {
					testBlueTooth.btSocket.close();
					manageThread.Stop();
					setTitle("socket连接已关闭");
				} catch (IOException e) {
					setTitle("关闭连接失败");
				}			
			} 
			else if (v == btBack) {RelayControl.this.finish();} // 返回
			else if (v == button1){sendMessage("11");} 
			else if (v == button2){sendMessage("21");} 
			else if (v == button3){sendMessage("31");} 
			else if (v == button4){sendMessage("41");}
			else if (v == btSend){									
					String infoSend =_txtSend.getText().toString();	
					sendMessage(infoSend);
					setTitle("发送成功");						
			}		
		}
	}
	
	public void sendMessage(String message) {
		
		//控制模块
		try {
			outStream = testBlueTooth.btSocket.getOutputStream();
			
		} catch (IOException e) {		
			//Log.e(TAG, "ON RESUME: Output stream creation failed.", e);
			Toast.makeText(getApplicationContext(), " Output stream creation failed.", Toast.LENGTH_SHORT);
			
		}
				
		byte[] msgBuffer = null;		
		try {
			msgBuffer = message.getBytes(encodeType);//编码
		} catch (UnsupportedEncodingException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			Log.e("write", "Exception during write encoding GBK ", e1);
		}		

			
		try {
			   outStream.write(msgBuffer);				
				//Toast.makeText(getApplicationContext(), "发送数据中..", Toast.LENGTH_SHORT);
				setTitle("成功发送指令:"+message);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				//Log.e(TAG, "ON RESUME: Exception during write.", e);
				Toast.makeText(getApplicationContext(), "发送数据失败", Toast.LENGTH_SHORT);				
			}			
		}

	
	 class ConnectedThread extends Thread {
		
		private InputStream inStream = null;// 蓝牙数据输入流
		private long wait;
		private Thread thread;
		
		public ConnectedThread() {
			isRecording = false;
			this.wait=50;
			thread =new Thread(new ReadRunnable());
		}

		public void Stop() {
			isRecording = false;			
			}
		
		public void Start() {
			isRecording = true;
			State aa = thread.getState();
			if(aa==State.NEW){
			   thread.start();
			}else thread.resume();
		}
		
		private class ReadRunnable implements Runnable {
		public void run() {
			
			while (isRecording) {
				
				try {					
					inStream = testBlueTooth.btSocket.getInputStream();						
				} catch (IOException e) {			
					//Log.e(TAG, "ON RESUME: Output stream creation failed.", e);
					Toast.makeText(getApplicationContext(), " input stream creation failed.", Toast.LENGTH_SHORT);
					
				}						
                     
				int length=20;
				byte[] temp = new byte[length];

				//keep listening to InputStream while connected
				if (inStream!= null) {
				try{
					int len = inStream.read(temp,0,length-1);	
					Log.e("available", String.valueOf(len));
					if (len > 0) {
						byte[] btBuf = new byte[len];
						System.arraycopy(temp, 0, btBuf, 0, btBuf.length);	
								            
						//读编码
			            String readStr1 = new String(btBuf,encodeType);
			            mHandler.obtainMessage(01,len,-1,readStr1).sendToTarget();
			            
					}			             
		             Thread.sleep(wait);// 延时一定时间缓冲数据
					}catch (Exception e) {
						// TODO Auto-generated catch block
						mHandler.sendEmptyMessage(00);
					}				
				
				}
				}
		}
		}	
	}
	
	 private class MyHandler extends Handler{ 
	    	@Override		    
	        public void dispatchMessage(Message msg) { 
	    		switch(msg.what){
	    		case 00:
	    			isRecording=false;
	    			_txtRead.setText("");
	    			_txtRead.setHint("socket连接已关闭");
	    			//_txtRead.setText("inStream establishment Failed!");
	    			break;
	    			
	    		case 01:
	    			String info=(String) msg.obj;
	    			_txtRead.append(info);
	    			break;    			

	            default:	            
	                break;
	    		}
	    		}
	    	}
	
	}