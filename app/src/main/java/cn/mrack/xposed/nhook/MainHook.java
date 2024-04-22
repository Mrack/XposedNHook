package cn.mrack.xposed.nhook;


import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;

import java.util.Arrays;

import cn.mrack.xposed.nhook.menu.Menu;
import cn.mrack.xposed.nhook.menu.PBoolean;
import cn.mrack.xposed.nhook.menu.PInteger;
import cn.mrack.xposed.nhook.menu.PString;
import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.IXposedHookZygoteInit;
import de.robv.android.xposed.XC_MethodHook;
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
            Log.d(TAG, "handleLoadPackage: " + lpparam.packageName + " loaded");
            HookUtils.attachApplication(HookUtils::nativeHookInit);
            HookUtils.attachActivity(activity -> {
                createMenu(activity);
                NHook.sign1("test sign trace");
            });
        }
    }

    private static void createMenu(Context activity) {
        Menu menu = new Menu(activity);
        menu.attach();
        menu.Category("Category 1");
        menu.InputNum("InputNum 1", PInteger.of(0));
        menu.InputText("InputText 1", PString.of("Hello World"));
        menu.Button("Button 1", PBoolean.of(false));
        menu.CheckBox("CheckBox 1", PBoolean.of(false));
        menu.Switch("Switch 1", PBoolean.of(false));
        menu.ButtonOnOff("Button 2", PBoolean.of(false));
        menu.SeekBar("SeekBar 1", PInteger.of(0), 0, 100, 5);
        menu.CheckBox("CheckBox 2", PBoolean.of(false));
        menu.ButtonLink("External Link &nearr;", "https://baidu.com");
        menu.SeekBar("SeekBar 2", PInteger.of(0), -50, 100, 5);
        menu.SeekBar("SeekBar 3", PInteger.of(0), 0, 5);
        menu.RadioButton("Radio Button 1", new String[]{"Option 1", "Option 2", "Option 3"}, PInteger.of(0));

        menu.startCollapse("Collapse 1");

        menu.ButtonAction("Button 3", () -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(activity);
            builder.setTitle("Alert");
            builder.setMessage("This is an alert");
            builder.setPositiveButton("OK", null);
            builder.show();
        });

        menu.ButtonLink("External Link &nearr;", "https://baidu.com");
        menu.SeekBar("SeekBar 2", PInteger.of(0), -50, 100, 5);
        menu.SeekBar("SeekBar 3", PInteger.of(0), 0, 5);
        menu.Switch("Switch 1", PBoolean.of(false));

        menu.endCollapse();

        menu.TextView("This is a TextView not fully html supported. <b>bold</b> <i>italic</i> <u>underline</u> <s>strikethrough</s> <font color='red'>color</font> <font size='20'>size</font> <font face='monospace'>face</font> <a href='https://www.google.com'>link</a>");

        menu.SeekBar("SeekBar 2", PInteger.of(0), -50, 100, 5);

        menu.WebTextView("<html><head><style>body{color: white;}</style></head><body>" + "This is WebView, with REAL HTML support!" + "<div style=\"background-color: darkblue; text-align: center;\">Support CSS</div>" + "<marquee style=\"color: green; font-weight:bold;\" direction=\"left\" scrollamount=\"5\" behavior=\"scroll\">This is <u>scrollable</u> text</marquee>" + "</body></html>");

        menu.SeekBar("SeekBar 3", PInteger.of(0), 0, 5);
        menu.RadioButton("Radio Button 1", new String[]{"Option 1", "Option 2", "Option 3"}, PInteger.of(0));
    }
}