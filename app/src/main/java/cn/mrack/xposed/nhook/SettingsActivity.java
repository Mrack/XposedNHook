package cn.mrack.xposed.nhook;

import android.app.Activity;
import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.util.Log;

public class SettingsActivity extends Activity {

    public static class SettingsFragment extends PreferenceFragment {
        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.settings);
            Preference moduleStatus = findPreference("module_status");
            boolean active = isModuleActive();
            Log.d("MainHook", "onCreate: " + active);
            moduleStatus.setEnabled(active);
            moduleStatus.setTitle(active ? "模块状态: [已激活]" : "模块状态: [未激活]");
        }
    }

    private static final String TAG = "SettingsActivity";
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getFragmentManager().beginTransaction().replace(android.R.id.content, new SettingsFragment()).commit();
    }

    public static boolean isModuleActive() {
        return false;
    }
}
