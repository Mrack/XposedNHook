package cn.mrack.xposed.nhook.menu;

import android.content.SharedPreferences;

public class PValue<T> {
    public interface onValueChangedListener<T> {
        void onValueChanged(T v, T v1);
    }

    private T value;
    private SharedPreferences sp;
    private String key;

    public PValue(SharedPreferences sp, String key, T value) {
        this.sp = sp;
        if (value instanceof Boolean) {
            this.value = (T) Boolean.valueOf(sp.getBoolean(key, (Boolean) value));
        } else if (value instanceof Integer) {
            this.value = (T) Integer.valueOf(sp.getInt(key, (Integer) value));
        } else if (value instanceof String) {
            this.value = (T) sp.getString(key, (String) value);
        } else {
            this.value = value;
        }
        this.key = key;
    }

    public PValue(T value) {
        this.value = value;
    }

    public T get() {
        return value;
    }

    public void set(T value) {
        T oldValue = this.value;
        this.value = value;
        if (listener != null) {
            listener.onValueChanged(oldValue, value);
        }
        if (sp != null){
            if (value instanceof Boolean) {
                sp.edit().putBoolean(key, (Boolean) value).apply();
            } else if (value instanceof Integer) {
                sp.edit().putInt(key, (Integer) value).apply();
            } else if (value instanceof String) {
                sp.edit().putString(key, (String) value).apply();
            }
        }
    }

    private onValueChangedListener<T> listener;

    public void setOnValueChangedListener(onValueChangedListener<T> listener) {
        this.listener = listener;
    }

}
