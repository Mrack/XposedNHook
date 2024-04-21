package cn.mrack.xposed.nhook;


import android.content.Context;
import android.util.Log;

import java.util.Arrays;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.IXposedHookZygoteInit;
import de.robv.android.xposed.XC_MethodReplacement;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class MainHook implements IXposedHookLoadPackage {
    private static final String TAG = "MainHook";
    private static final String[] packageList = new String[]{
            "com.google.android.youtube"
    };

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) {
        if (lpparam.packageName.equals(BuildConfig.APPLICATION_ID)) {
            XposedHelpers.findAndHookMethod(BuildConfig.APPLICATION_ID + ".SettingsActivity",
                    lpparam.classLoader, "isModuleActive", XC_MethodReplacement.returnConstant(Boolean.TRUE));
            Log.d(TAG, "Hooked " + BuildConfig.APPLICATION_ID);
        } else if (Arrays.binarySearch(packageList, lpparam.packageName) >= 0) {
            HookUtils.attachApplication(new HookUtils.InitCallback() {
                @Override
                public void onHook(Context context) {
                    HookUtils.nativeHookInit(context);
                    // trace sign1
                    NHook.sign1("etyewioat");
                }
            });
        }
    }


}