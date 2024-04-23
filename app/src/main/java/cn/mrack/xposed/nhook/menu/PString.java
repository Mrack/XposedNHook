package cn.mrack.xposed.nhook.menu;

import android.content.SharedPreferences;

/**
 * Wrapper class for primitive String, to be used as a parameter for Menu
 */
public class PString extends PValue<String> {
    public PString(SharedPreferences sp, String key, String value) {
        super(sp, key, value);
    }

    public PString(String value) {
        super(value);
    }

    public static PString of(SharedPreferences sp, String key, String value) {
        return new PString(sp, key, value);
    }

    public static PString of(String value) {
        return new PString(value);
    }
}
