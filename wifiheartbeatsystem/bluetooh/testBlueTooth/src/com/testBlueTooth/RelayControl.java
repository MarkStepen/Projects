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
	
	public static boolean isRecording = false;// �߳̿��Ʊ��
	
	
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
		
		//�����߳�����
		manageThread=new ConnectedThread();
		mHandler  = new MyHandler();
		manageThread.Start();
		
		findMyView();
		
		setMyViewListener(); 
        	
		setTitle("����ǰ���ȹر�socket����");
		//���������ɼ�
		_txtRead.setCursorVisible(false);      //����������еĹ�겻�ɼ�
		_txtRead.setFocusable(false);           //�޽���

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
		// ����Button
		encodeType="GBK";//����ΪĬ��ѡ��Ϊ PCͨ��
		
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

			if (v == releaseCtrl)// �ͷ�����
			{
				try {
					testBlueTooth.btSocket.close();
					manageThread.Stop();
					setTitle("socket�����ѹر�");
				} catch (IOException e) {
					setTitle("�ر�����ʧ��");
				}			
			} 
			else if (v == btBack) {RelayControl.this.finish();} // ����
			else if (v == button1){sendMessage("11");} 
			else if (v == button2){sendMessage("21");} 
			else if (v == button3){sendMessage("31");} 
			else if (v == button4){sendMessage("41");}
			else if (v == btSend){									
					String infoSend =_txtSend.getText().toString();	
					sendMessage(infoSend);
					setTitle("���ͳɹ�");						
			}		
		}
	}
	
	public void sendMessage(String message) {
		
		//����ģ��
		try {
			outStream = testBlueTooth.btSocket.getOutputStream();
			
		} catch (IOException e) {		
			//Log.e(TAG, "ON RESUME: Output stream creation failed.", e);
			Toast.makeText(getApplicationContext(), " Output stream creation failed.", Toast.LENGTH_SHORT);
			
		}
				
		byte[] msgBuffer = null;		
		try {
			msgBuffer = message.getBytes(encodeType);//����
		} catch (UnsupportedEncodingException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
			Log.e("write", "Exception during write encoding GBK ", e1);
		}		

			
		try {
			   outStream.write(msgBuffer);				
				//Toast.makeText(getApplicationContext(), "����������..", Toast.LENGTH_SHORT);
				setTitle("�ɹ�����ָ��:"+message);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				//Log.e(TAG, "ON RESUME: Exception during write.", e);
				Toast.makeText(getApplicationContext(), "��������ʧ��", Toast.LENGTH_SHORT);				
			}			
		}

	
	 class ConnectedThread extends Thread {
		
		private InputStream inStream = null;// ��������������
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
								            
						//������
			            String readStr1 = new String(btBuf,encodeType);
			            mHandler.obtainMessage(01,len,-1,readStr1).sendToTarget();
			            
					}			             
		             Thread.sleep(wait);// ��ʱһ��ʱ�仺������
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
	    			_txtRead.setHint("socket�����ѹر�");
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