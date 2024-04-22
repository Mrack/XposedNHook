package cn.mrack.xposed.nhook.menu;

/**
 * Wrapper class for primitive String, to be used as a parameter for Menu
 */
public class PString {
    private String value;

    public PString(String value) {
        this.value = value;
    }

    public String get() {
        return value;
    }

    public void set(String value) {
        this.value = value;
    }

    public static PString of(String value) {
        return new PString(value);
    }
}
