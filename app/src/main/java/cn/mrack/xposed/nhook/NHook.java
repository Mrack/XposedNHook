package cn.mrack.xposed.nhook;

import android.content.Context;

public class NHook {
    public static native void initNativeHook(Context context);

    public static native String sign1(String data);

    public static native String test111(int value);
}
