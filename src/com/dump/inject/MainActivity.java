package com.dump.inject;

import java.util.ArrayList;
import java.util.List;

import com.dump.inject.R;

import de.robv.android.xposed.callbacks.XC_LoadPackage;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

public class MainActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		DumpLog.Init(this);
		
		setContentView(R.layout.activity_main);
		
		Button btnStartup = (Button)findViewById(R.id.btnStartup);
		btnStartup.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				DumpLog.LOGD("btnStartup onClick");
				
				PackageManager packageManager = getPackageManager();
				Intent intent = new Intent();
				intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				intent = packageManager.getLaunchIntentForPackage("com.tencent.tmgp.hhw");
				startActivity(intent);
			}
		});
		
		Spinner spinner = (Spinner) findViewById(R.id.spinner);
		
		//数据
		List<String> data_list = new ArrayList<String>();
		data_list.add("com.tencent.tmgp.hhw");
        
        //适配器
        ArrayAdapter<String> arr_adapter= new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, data_list);
        //设置样式
        arr_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        //加载适配器
        spinner.setAdapter(arr_adapter);
	}
}
