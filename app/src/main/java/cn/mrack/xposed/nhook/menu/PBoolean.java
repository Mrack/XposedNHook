package cn.mrack.xposed.nhook.menu;


import android.content.SharedPreferences;

/**
 * Wrapper class for primitive boolean, to be used as a parameter for Menu
 */
public class PBoolean extends PValue<Boolean> {
    public PBoolean(SharedPreferences sp, String key, Boolean value) {
        super(sp, key, value);
    }

    public PBoolean(Boolean value) {
        super(value);
    }

    public static PBoolean of(SharedPreferences sp, String key, Boolean value) {
        return new PBoolean(sp, key, value);
    }

    public static PBoolean of(Boolean value) {
        return new PBoolean(value);
    }
}
