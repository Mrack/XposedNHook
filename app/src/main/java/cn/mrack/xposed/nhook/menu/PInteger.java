package cn.mrack.xposed.nhook.menu;


import android.content.SharedPreferences;

/**
 * Wrapper class for primitive int, to be used as a parameter for Menu
 */
public class PInteger extends PValue<Integer> {
    public PInteger(SharedPreferences sp, String key, Integer value) {
        super(sp, key, value);
    }

    public PInteger(Integer value) {
        super(value);
    }

    public static PInteger of(SharedPreferences sp, String key, Integer value) {
        return new PInteger(sp, key, value);
    }

    public static PInteger of(Integer value) {
        return new PInteger(value);
    }
}
