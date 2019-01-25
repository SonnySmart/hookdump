package com.dump.log;

import android.content.Context;

public class DumpLog {
	
	static Context mContext;
	
	final static String TAG = "hookdump";
	
	public static void Init(Context context) {
		mContext = context;
	}
	
	public static void LOGD(String msg) {
		android.util.Log.d(TAG, msg);
	}
	
	public static void LOGI(String msg) {
		android.util.Log.i(TAG, msg);
	}
	
	public static void LOGE(String msg) {
		android.util.Log.e(TAG, msg);
	}
	
	public static void Toast(String text) {
		android.widget.Toast.makeText(mContext, text, android.widget.Toast.LENGTH_SHORT).show();
	}
}
