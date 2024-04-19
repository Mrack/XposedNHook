package cn.mrack.xposed.nhook;

import android.annotation.SuppressLint;
import android.app.Application;
import android.content.Context;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.security.MessageDigest;

import cn.mrack.so.arm64.SoData;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;

public class HookUtils {

    interface InitCallback {
        void onHook(Context context);
    }

    public static void attachApplication(InitCallback callback) {
        // hook application attach
        XposedHelpers.findAndHookMethod(Application.class, "attach", Context.class, new XC_MethodHook() {
            @Override
            protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                // get the context
                Context context = (Context) param.args[0];
                callback.onHook(context);
            }
        });
    }

    public static void nativeHookInit(Context context) {
        if (loadSo(context)) {
            Log.d(TAG, "nativeHookInit: load so success");
        } else {
            Log.e(TAG, "nativeHookInit: load so failed");
        }
        NHook.initNativeHook();
    }


    private static final String TAG = "HookUtils";


    private static boolean verifyMd5(File file, String md5) {
        if (!file.exists()) {
            return false;
        }
        try {
            FileInputStream fileInputStream = new FileInputStream(file);
            byte[] buffer = new byte[1024];
            int bytesRead;
            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            while ((bytesRead = fileInputStream.read(buffer)) != -1) {
                byteArrayOutputStream.write(buffer, 0, bytesRead);
            }
            fileInputStream.close();
            byte[] bytes = byteArrayOutputStream.toByteArray();
            byteArrayOutputStream.close();
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] fileMd5 = md.digest(bytes);
            StringBuilder sb = new StringBuilder();
            for (byte b : fileMd5) {
                sb.append(Integer.toHexString((b & 0xFF) | 0x100).substring(1, 3));
            }
            if (!sb.toString().equals(md5)) {
                return false;
            }
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    private static boolean loadSo(Context context) {
        try {
            File file = context.getFileStreamPath("libnhook.so");
            String md5 = SoData.md5;
            byte[] data = SoData.data;
            if (!verifyMd5(file, md5)) {
                FileOutputStream fileOutputStream = new FileOutputStream(file);
                fileOutputStream.write(data);
                fileOutputStream.close();
            }
            System.load(file.getAbsolutePath());
            return true;
        } catch (Exception e) {
            Log.e(TAG, "loadSo: ", e);
            return false;
        }
    }
}
