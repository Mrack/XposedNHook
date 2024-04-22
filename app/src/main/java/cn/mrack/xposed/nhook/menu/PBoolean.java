package cn.mrack.xposed.nhook.menu;


/**
 * Wrapper class for primitive boolean, to be used as a parameter for Menu
 */
public class PBoolean {
    private boolean value;

    public PBoolean(boolean value) {
        this.value = value;
    }

    public boolean get() {
        return value;
    }

    public void set(boolean value) {
        this.value = value;
    }

    public static PBoolean of(boolean value) {
        return new PBoolean(value);
    }
}
