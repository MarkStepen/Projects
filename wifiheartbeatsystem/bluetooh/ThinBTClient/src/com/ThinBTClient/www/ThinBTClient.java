package com.ThinBTClient.www;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;

public class ThinBTClient extends Activity implements Runnable {

	private static final boolean D = true;
	private BluetoothAdapter mBluetoothAdapter = null;
	private BluetoothSocket btSocket = null;
	private OutputStream outStream = null;
	private InputStream InStream = null;
	public boolean BluetoothFlag = true;
	public int runState = 0;// 0:ֹͣ 1:ǰ�� 2:����
	public int tansState = 0;
	SensorManager mySensorManager; // SensorManager��������

	Button mButtonF;
	Button mButtonB;
	Button mButtonL;
	Button mButtonR;
	Button mButtonS;
	Button mButtongotoactivity01;

	TextView myText;
	SeekBar seekBarPWM;

	private static final UUID MY_UUID = UUID
			.fromString("00001101-0000-1000-8000-00805F9B34FB");

	private static String address = null;									 // <==Ҫ���ӵ������豸MAC��ַ
	
	/* ��ʾѡ��һ��Ҫ���ӵķ����� */
	/* ��ת�������������豸�б���������ѡ�� */
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		myText = (TextView) findViewById(R.id.myText);
		myText.setText("������������,���Ժ�...");
		seekBarPWM = (SeekBar) findViewById(R.id.seekBarPWM);
		seekBarPWM.setMax(100);
		seekBarPWM.setProgress(30);

		mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
		if (mBluetoothAdapter == null) {
			Toast.makeText(this, "�����豸�����ã����������", Toast.LENGTH_LONG).show();
			finish();
			return;
		}

		if (!mBluetoothAdapter.isEnabled()) {
			DisplayToast("����δ�򿪣������Զ���������");
			mBluetoothAdapter.enable();
		}

		// �����϶����ı������
		OnSeekBarChangeListener osbcl = new OnSeekBarChangeListener() {
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				int data = seekBarPWM.getProgress();
				myText.setText("��ǰ���ȣ�" + data);
				String message;
				byte[] msgBuffer = new byte[5];
				try {
					outStream = btSocket.getOutputStream();

				} catch (IOException e) {
					e.printStackTrace();
				}
				msgBuffer[0] = 0;
				msgBuffer[1] = (byte) (data >> 24 & 0xff);
				msgBuffer[2] = (byte) (data >> 16 & 0xff);
				;
				msgBuffer[3] = (byte) (data >> 8 & 0xff);
				;
				msgBuffer[4] = (byte) (data >> 0 & 0xff);
				;

				try {
					outStream.write(msgBuffer);

				} catch (IOException e) {
					e.printStackTrace();
				}

			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				// Toast.makeText(getApplicationContext(),
				// "onStartTrackingTouch",
				// Toast.LENGTH_SHORT).show();
			}

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				// Toast.makeText(getApplicationContext(),
				// "onStopTrackingTouch",
				// Toast.LENGTH_SHORT).show();
			}

		};
		// Ϊ�϶����󶨼�����
		seekBarPWM.setOnSeekBarChangeListener(osbcl);

		// ǰ��
		mButtonF = (Button) findViewById(R.id.btnF);
		mButtonF.setOnTouchListener(new Button.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					runState = 1;
					sendCmd("a");
					Log.d("Season", "ǰ��");
					break;

				case MotionEvent.ACTION_UP:
					runState = 0;
					sendCmd("g");
					Log.d("Season", "ֹͣ��ת");
					break;
				}
				return false;
			}
		});

		// ����
		mButtonB = (Button) findViewById(R.id.btnB);
		mButtonB.setOnTouchListener(new Button.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					runState = 2;
					sendCmd("b");
					Log.d("Season", "����");
					break;

				case MotionEvent.ACTION_UP:
					runState = 0;
					sendCmd("g");
					Log.d("Season", "ֹͣyuzhaun");
					break;
				}
				return false;
			}
		});
		// ��ת
		mButtonL = (Button) findViewById(R.id.btnL);
		mButtonL.setOnTouchListener(new Button.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					sendCmd("c");
					break;

				case MotionEvent.ACTION_UP:
					sendCmd("C");
					break;
				}
				return false;
			}
		});
		// ��ת
		mButtonR = (Button) findViewById(R.id.btnR);
		mButtonR.setOnTouchListener(new Button.OnTouchListener() {

			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					sendCmd("d");
					break;

				case MotionEvent.ACTION_UP:
					sendCmd("D");
					break;
				}
				return false;
			}

		});

		mButtongotoactivity01 = (Button) findViewById(R.id.gotoactivity01);
		mButtongotoactivity01.setOnTouchListener(new Button.OnTouchListener() {

			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					break;

				case MotionEvent.ACTION_UP:
					Intent enabler = new Intent(ThinBTClient.this,
							DiscoveryActivity.class);
					startActivity(enabler);
					break;
				}
				return false;
			}

		});

		// ֹͣ
		mButtonS = (Button) findViewById(R.id.btnS);
		mButtonS.setOnTouchListener(new Button.OnTouchListener() {

			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch (action) {
				case MotionEvent.ACTION_DOWN:
					Log.d("Season", "ֹͣ");
					sendCmd("g");
					break;

				case MotionEvent.ACTION_UP:
					sendCmd("G");
					Log.d("Season", "ֹͣ");
					break;
				}

				return false;

			}

		});

		mySensorManager = (SensorManager) getSystemService(SENSOR_SERVICE); // ���SensorManager����
	}

	class MyHandler extends Handler {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			Bundle data = msg.getData();
			Log.d("Season", "in the thread:age=" + data.getInt("age")
					+ " name=" + data.getString("name"));

			try {
				InStream = btSocket.getInputStream();

			} catch (IOException e) {
				e.printStackTrace();
			}

			byte[] tmp = new byte[5];

			try {
				InStream.read(tmp, 0, 5);
				myText.setText(new String(tmp, 0, 5) + " ");
				System.out.println(new String(tmp, 0, 5) + " ");

			} catch (IOException e) {
				e.printStackTrace();
			}

			// Thread.sleep(1000);
		}

		MyHandler(Looper looper) {
			super(looper);
		}

		MyHandler() {

		}

	}

	// ����ʵ����SensorEventListener�ӿڵĴ�����������
	private SensorEventListener mySensorEventListener = new SensorEventListener() {
		@Override
		public void onAccuracyChanged(Sensor sensor, int accuracy) {// ��дonAccuracyChanged�������ڴ�Ϊ��ʵ��
		}

		@Override
		public void onSensorChanged(SensorEvent event) {
			if (event.sensor.getType() == Sensor.TYPE_ORIENTATION) {// �ж��Ƿ�Ϊ���ٶȴ������仯����������
				float[] values = event.values; // ��ò��������
				myText.setText("Yaw:" + values[0] + " Pitch:" + values[1]
						+ " Roll:" + values[2]);
				if (BluetoothFlag == true) {
					if (values[1] > 20)// ����
					{
						if (tansState == 0) {
							if (runState == 1)// ǰ��
							{
								sendCmd("h");// ��ת��
								Log.d("Season", "��ת��");
							} else if (runState == 2)// ����
							{
								sendCmd("k");// ������
								Log.d("Season", "������");
							}
							tansState = 1;// ֻ����һ��ָ��
						}

					}

					else if (values[1] < -20)// ����
					{
						if (tansState == 0) {
							if (runState == 1)// ǰ��
							{
								sendCmd("i");// ��ת��
								Log.d("Season", "��ת��");
							} else if (runState == 2)// ����
							{
								sendCmd("j");// ������
								Log.d("Season", "������");
							}
							tansState = 1;// ֻ����һ��ָ��
						}

					} else {
						if (tansState == 1) {
							tansState = 2;
							sendCmd("f");// ���ת��
							if (runState == 1)// ǰ��
							{
								sendCmd("a");
								Log.d("Season", "���ת��ǰ��");
							} else if (runState == 2)// ����
							{
								sendCmd("b");
								Log.d("Season", "���ת�����");
							}
						}

						else if (tansState == 2) {
							tansState = 0;
						}

					}

				}

			}
		}
	};

	@Override
	public void onStart() {
		super.onStart();
	}

	@Override
	public void onResume() {
		mySensorManager.registerListener(// ���÷���ΪSensorManagerע�������
				mySensorEventListener, // ʵ����SensorEventListener�ӿڵļ���������
				mySensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), // Sensor����
				SensorManager.SENSOR_DELAY_UI // ϵͳ����SensorEvent�¼���Ƶ��
				);
		super.onResume();

		DisplayToast("���ڳ������������豸�����Ժ󡤡�����");
		if(address == null){
			address = "00:12:06:01:51:26";
		}
		else {
		}
		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
		try {
			btSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
		} catch (IOException e) {
			DisplayToast("�׽��ִ���ʧ�ܣ�");
		}
		DisplayToast("�ɹ����������豸��");
		mBluetoothAdapter.cancelDiscovery();
		try {
			btSocket.connect();
			DisplayToast("���ӳɹ����������Կ�ʼ�ٿ���~~~");
			myText.setText("�����豸��׼������!");
			sendCmd("Connect OK!");
			BluetoothFlag = true;
			MyThread bluetoothThread = new MyThread();
			bluetoothThread.start();
		} catch (IOException e) {
			try {
				btSocket.close();
			} catch (IOException e2) {
				DisplayToast("����û�н������޷��ر��׽��֣�");
			}
		}
	}

	@Override
	public void onPause() {
		mySensorManager.unregisterListener(mySensorEventListener); // ȡ��ע�������
		super.onPause();
		if (outStream != null) {
			try {
				outStream.flush();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		try {
			btSocket.close();
			BluetoothFlag = false;// �ر��������߳�
		} catch (IOException e2) {

			DisplayToast("�׽��ֹر�ʧ�ܣ�");
		}
	}

	@Override
	public void onStop() {
		super.onStop();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();

	}

	public void DisplayToast(String str) {
		Toast toast = Toast.makeText(this, str, Toast.LENGTH_LONG);
		toast.setGravity(Gravity.TOP, 0, 220);
		toast.show();

	}

	public void sendCmd(String message) {
		try {
			outStream = btSocket.getOutputStream();

		} catch (IOException e) {
			e.printStackTrace();
		}
		byte[] msgBuffer;
		msgBuffer = message.getBytes();
		try {
			outStream.write(msgBuffer);

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public class MyThread extends Thread {

		MyThread() {
			BluetoothFlag = true;
			try {
				InStream = btSocket.getInputStream();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		@Override
		public void run() {
			byte[] tmp = null;
			while (BluetoothFlag) {
				try {
					demo();
				} catch (Exception e) {
					System.out.println("Exception...");
					e.printStackTrace();
				}
			}
		}

		public String demo() throws IOException, InterruptedException {
			byte[] buffer = new byte[1];
			int length = 0;
			byte bb = 0;
			String line = null;
			System.out.println("ThinBTClient.MyThread.demo()");
			ArrayList<Byte> list = new ArrayList<Byte>();
			while (InStream.read(buffer) == -1) {
				Thread.sleep(100);
				System.out.print("-->" + InStream.read(buffer));
			}
			return null;
		}

	}

	@Override
	public void run() {
		// TODO Auto-generated method stub

	}

}