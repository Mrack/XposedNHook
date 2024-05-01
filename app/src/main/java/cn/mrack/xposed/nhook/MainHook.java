package cn.mrack.xposed.nhook;

import android.app.AlertDialog;
import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;
import java.util.ArrayList;
import java.util.List;
import cn.mrack.xposed.nhook.menu.Menu;
import cn.mrack.xposed.nhook.menu.PBoolean;
import cn.mrack.xposed.nhook.menu.PInteger;
import cn.mrack.xposed.nhook.menu.PString;
import cn.mrack.xposed.nhook.menu.SurfaceImGUI;
import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodReplacement;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class MainHook implements IXposedHookLoadPackage {
    private static final String TAG = "MainHook";

    public static final boolean USE_NATIVE_MENU = true;

    private static final List<String> packageList = new ArrayList<String>() {{
        add("com.google.android.youtube");
        add("com.block.juggle");
        add("com.tencent.tmgp.sgame");
        add("com.colossi.survival.samurai");
    }};

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) {
        if (lpparam.packageName.equals(BuildConfig.APPLICATION_ID)) {
            XposedHelpers.findAndHookMethod(BuildConfig.APPLICATION_ID + ".SettingsActivity",
                    lpparam.classLoader, "isModuleActive", XC_MethodReplacement.returnConstant(Boolean.TRUE));
        } else if (packageList.contains(lpparam.packageName)) {
            Log.d(TAG, "handleLoadPackage: " + lpparam.packageName + " loaded");
            HookUtils.attachApplication(HookUtils::nativeHookInit);
            HookUtils.attachActivity(activity -> {
                if (USE_NATIVE_MENU){
                    createNativeMenu(activity);
                } else {
                    createMenu(activity);
                }
                NHook.sign1("test sign trace");
            });
        }
    }

    private static void createNativeMenu(Context activity) {
        Menu menu = new Menu(activity);
        menu.attach();
        menu.Surface(new SurfaceImGUI(activity));
    }

    private static void createMenu(Context activity) {
        SharedPreferences sp = activity.getSharedPreferences("menu", Context.MODE_PRIVATE);
        Menu menu = new Menu(activity);
        menu.attach();
        menu.Category("Value Change");
        menu.Category("Value Change");
        PBoolean value1 = PBoolean.of(sp, "test1", false);
        value1.setOnValueChangedListener((v, v1) -> {
            Log.d(TAG, "Switch: " + NHook.test111(v1 ? 1 : 0));
        });
        menu.Switch("Switch 1", value1);
        PInteger value = PInteger.of(sp, "test2", 0);
        value.setOnValueChangedListener((v, v1) -> {
            Log.d(TAG, "SeekBar: " + NHook.test111(v1));
        });
        menu.SeekBar("SeekBar 1", value, 0, 100, 5);

        menu.Category("Category 1");
        menu.InputNum("InputNum 1", PInteger.of(0));
        menu.InputText("InputText 1", PString.of("Hello World"));
        menu.Button("Button 1", PBoolean.of(sp, "test3", false));
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