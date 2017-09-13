package com.ThinBTClient.www;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;

public class Activity01 extends Activity {

	private TextView myTextView = null;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_BLUR_BEHIND, WindowManager.LayoutParams.FLAG_BLUR_BEHIND);
        setContentView(R.layout.activity01);
        

		Intent intent = getIntent();
		//从Intent当中根据key取得value
		String BluetoothMAC = intent.getStringExtra("BluetoothMAC");
		//根据控件的ID得到响应的控件对象
		myTextView = (TextView)findViewById(R.id.myTextView);
		//为控件设置Text值
		myTextView.setText("你选择的蓝牙MAC地址为:"+BluetoothMAC);
		Log.d("Season", "你选择的蓝牙MAC地址为:"+BluetoothMAC);
		
		
		
        Button Button_back = (Button)findViewById(R.id.back);
        
        Button_back.setOnTouchListener(new Button.OnTouchListener(){     	
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				int action = event.getAction();
				switch(action)
				{
				case MotionEvent.ACTION_DOWN:
				break;
				
				case MotionEvent.ACTION_UP:
					Intent enabler = new Intent(Activity01.this, ThinBTClient.class);
					startActivity(enabler);
					break;
				}					
				return false;
			}
        	
     
        });
        
        
        
        
    }
    
}
