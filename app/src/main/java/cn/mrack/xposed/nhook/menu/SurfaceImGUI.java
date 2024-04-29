package cn.mrack.xposed.nhook.menu;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.os.RemoteException;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;

public class SurfaceImGUI extends SurfaceView implements SurfaceHolder.Callback {

    private static final String TAG = "SurfaceImGUI";
    public static native void initImGUI(Surface surface);

    public static native void destroyImGUI();

    public static native void setOrientationImGUI(int orientation);

    public final WindowManager wms;

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        Log.d(TAG, "onDetachedFromWindow: ");
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        Log.d(TAG, "onAttachedToWindow: ");
    }

    public SurfaceImGUI(Context context) {
        super(context);
        setZOrderOnTop(true);
        SurfaceHolder holder = getHolder();
        holder.setFormat(PixelFormat.TRANSPARENT);
        getHolder().addCallback(this);
        wms = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        new Thread( () ->{
            initImGUI(surfaceHolder.getSurface());
        }).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        setOrientationImGUI(wms.getDefaultDisplay().getRotation());
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        destroyImGUI();
    }
}