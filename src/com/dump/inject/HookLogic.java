package com.dump.inject;

import java.io.File;
import java.util.ArrayList;
import java.util.Dictionary;
import java.util.List;

import android.R.integer;

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

    static {    	
        // TODO: Add the package name of application your want to hook!
        hostAppPackages.add("com.game.qeylyx");
        hostAppPackages.add("com.mengxiangyuele.n1");
        hostAppPackages.add("com.tc.tbnn");  
        hostAppPackages.add("com.estoty.game2048"); 
        hostAppPackages.add("com.uchess.zzyl"); 
    }
    
	static void LOGD(String msg) {
		XposedBridge.log(msg);
	}

	@Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam loadPackageParam) throws Throwable {
    	LOGD(loadPackageParam.packageName + " start .1");
		
    	//注入so
		System.load("/data/data/com.dump.inject/lib/libdump.so");
		LOGD("so injected .");
    }
}
