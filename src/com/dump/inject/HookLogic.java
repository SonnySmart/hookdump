package com.dump.inject;

import java.util.ArrayList;
import java.util.List;

import com.dump.log.DumpLog;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

/**
 * @author DX
 * 注意：该类不要自己写构造方法，否者可能会hook不成功
 * 开发Xposed模块完成以后，建议修改xposed_init文件，并将起指向这个类,以提升性能
 * 所以这个类需要implements IXposedHookLoadPackage,以防修改xposed_init文件后忘记
 * Created by DX on 2017/10/4.
 */
public class HookLogic implements IXposedHookLoadPackage {
	
    /**
     * 宿主程序的包名(允许多个),过滤无意义的包名,防止无意义的apk文件加载
     */
    public static List<String> hostAppPackages = new ArrayList<String>();
    
    public String apkAbsoluteString;
    
    static String mCurPackage;

    static {    	
        // TODO: Add the package name of application your want to hook!
        hostAppPackages.add("com.game.qeylyx");
        hostAppPackages.add("com.mengxiangyuele.n1");
        hostAppPackages.add("com.tc.tbnn");  
        hostAppPackages.add("com.estoty.game2048"); 
        hostAppPackages.add("com.uchess.zzyl"); 
        hostAppPackages.add("com.tencent.tmgp.hhw"); 
        hostAppPackages.add("com.tencent.game.SSGame");
        hostAppPackages.add("com.next.netcraft");
        hostAppPackages.add("com.tencent.tmgp.dnf");
        hostAppPackages.add("com.tencent.tmgp.sgame");
        
    }
    
	static void LOGD(String msg) {
		XposedBridge.log(msg);
		//DumpLog.LOGD(msg);
	}
	
	public void setApkAbsoluteString(String adbString) {
		apkAbsoluteString = adbString;
		LOGD("setApkAbsoluteString:" + apkAbsoluteString);
	}

	// inject entry
	@Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam loadPackageParam) throws Throwable {
    	LOGD(loadPackageParam.packageName + " start .");
    	
    	mCurPackage = loadPackageParam.packageName;
		
    	//注入so
		System.load("/data/data/com.dump.inject/lib/libinject.so");
		LOGD("so injected .");
		
		LOGD("str:" + native_hello());
    }
	
	// native func
	public static native String native_hello();
	
	// native get packagename
	public static String getPackageString() {
		if (mCurPackage == null || mCurPackage.length() == 0) {
			LOGD("mCurPackage is null .");
			return null;
		}
		return mCurPackage;
	}
	
	// native get sdcard path
	public static String getSdcardDirString() {
		final String rootString = "/sdcard/Android/data/";
		String packString = getPackageString();
		if (packString == null){
			LOGD("getPackageString is null .");
			return null;
		}
		return rootString + packString;
	}
	
	public static String getAssetString() {		
		return null;
	}
}
