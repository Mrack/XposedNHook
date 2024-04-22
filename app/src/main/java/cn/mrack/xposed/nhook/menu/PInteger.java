package cn.mrack.xposed.nhook.menu;


/**
 * Wrapper class for primitive int, to be used as a parameter for Menu
 */
public class PInteger {
    private int value;

    public PInteger(int value) {
        this.value = value;
    }

    public int get() {
        return value;
    }

    public void set(int value) {
        this.value = value;
    }

    public static PInteger of(int value) {
        return new PInteger(value);
    }
}
