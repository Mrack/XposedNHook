package cn.mrack.xposed.nhook.menu;

import static android.view.ViewGroup.LayoutParams.MATCH_PARENT;
import static android.view.ViewGroup.LayoutParams.WRAP_CONTENT;
import static android.widget.RelativeLayout.ALIGN_PARENT_LEFT;
import static android.widget.RelativeLayout.ALIGN_PARENT_RIGHT;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.res.ColorStateList;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.os.Build;
import android.text.Html;
import android.text.InputType;
import android.text.TextUtils;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

/**
 * The menu class
 **/
public class Menu extends FrameLayout {
    private boolean isCollapsed;
    private final Point position;
    private final Config config;
    private final Context context;
    private final RelativeLayout mCollapsed;
    private final LinearLayout mExpanded, featureView;
    private final ImageView launcherIcon;
    private LinearLayout curCollapse;
    private WindowManager windowManager;
    private final WindowManager.LayoutParams wmParams;

    /**
     * Create a menu
     *
     * @param context The context of the menu
     **/
    public Menu(Context context) {
        this(context, new Config());
    }

    /**
     * Create a menu
     *
     * @param context The context of the menu
     * @param config  The config of the menu
     **/
    public Menu(Context context, Config config) {
        super(context);

        this.isCollapsed = true;
        this.context = context;
        this.config = config;
        //get the screen size
        DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
        int screenWidth = displayMetrics.widthPixels;
        int screenHeight = displayMetrics.heightPixels;
        this.position = new Point(0, screenHeight / 2);
        this.mCollapsed = new RelativeLayout(this.context);
        this.mExpanded = new LinearLayout(this.context);
        this.launcherIcon = new ImageView(this.context);
        this.featureView = new LinearLayout(this.context);
        this.wmParams = new WindowManager.LayoutParams(WRAP_CONTENT, WRAP_CONTENT, position.x, position.y, WindowManager.LayoutParams.TYPE_APPLICATION, WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_LAYOUT_IN_OVERSCAN | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN | WindowManager.LayoutParams.FLAG_SPLIT_TOUCH, PixelFormat.TRANSPARENT);

        this.Init();
    }

    /**
     * Create an action button
     *
     * @param text   The text of the button
     * @param action The action to run when the button is clicked
     **/
    public void ButtonAction(String text, Runnable action) {
        Button btn = Button(text);
        btn.setOnClickListener(view -> action.run());

        addFeature(btn);
    }

    /**
     * Create a on/off button
     *
     * @param text  The text of the button
     * @param value The reference value of the button
     **/
    public void ButtonOnOff(String text, PBoolean value) {
        Button btn = Button(text);
        btn.setText(Html.fromHtml(text + ": " + (value.get() ? "ON" : "OFF")));
        btn.setBackgroundColor(value.get() ? config.BTN_ON_BG_COLOR : config.BTN_OFF_BG_COLOR);
        btn.setOnClickListener(view -> {
            value.set(!value.get());
            btn.setText(Html.fromHtml(text + ": " + (value.get() ? "ON" : "OFF")));
            btn.setBackgroundColor(value.get() ? config.BTN_ON_BG_COLOR : config.BTN_OFF_BG_COLOR);
        });

        addFeature(btn);
    }

    /**
     * Create a button
     *
     * @param text  The text of the button
     * @param value The reference value of the button
     **/
    public void Button(String text, PBoolean value) {
        Button btn = Button(text);
        btn.setOnClickListener(view -> value.set(!value.get()));

        addFeature(btn);
    }

    private Button Button(String text) {
        final LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);

        final Button btn = new Button(context);
        btn.setLayoutParams(layoutParams);
        btn.setText(Html.fromHtml(text));
        btn.setAllCaps(false);
        btn.setBackgroundColor(config.MENU_BUTTON_BG_COLOR);
        btn.setTextColor(config.TEXT_COLOR_SECONDARY);
        btn.setHintTextColor(config.TEXT_COLOR_SECONDARY);
        btn.setHighlightColor(config.TEXT_COLOR_SECONDARY);

        return btn;
    }

    /**
     * Create a category separator
     *
     * @param text The text of the category
     */
    public void Category(String text) {
        final TextView textView = new TextView(context);
        textView.setBackgroundColor(config.MENU_CATEGORY_BG_COLOR);
        textView.setText(Html.fromHtml(text));
        textView.setGravity(Gravity.CENTER);
        textView.setTextColor(config.TEXT_COLOR_SECONDARY);
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(0, 5, 0, 5);

        addFeature(textView);
    }

    /**
     * Create a switch
     *
     * @param text  The text of the switch
     * @param value The reference value of the switch
     **/
    @SuppressLint("ResourceType")
    public void Switch(String text, PBoolean value) {
        final ColorStateList colorStateList = new ColorStateList(new int[][]{new int[]{-android.R.attr.state_enabled}, new int[]{android.R.attr.state_checked}, new int[]{}}, new int[]{Color.BLUE, Color.GREEN, Color.RED});

        @SuppressLint("UseSwitchCompatOrMaterialCode") final Switch switchView;
        switchView = new Switch(context);
        switchView.setText(Html.fromHtml(text));
        switchView.setTextColor(config.TEXT_COLOR_SECONDARY);
        switchView.setPadding(10, 5, 0, 5);
        switchView.setChecked(value.get());

        try {
            switchView.getThumbDrawable().setTintList(colorStateList);
            switchView.getTrackDrawable().setTintList(colorStateList);
        } catch (Exception e) {
            switchView.setTrackResource(0x7f0800a0);
            switchView.setThumbResource(0x7f0800a1);
            Log.d("Switch", e.toString());
        }

        switchView.setOnCheckedChangeListener((buttonView, isChecked) -> value.set(isChecked));

        addFeature(switchView);
    }

    /**
     * Create a seek bar
     *
     * @param featName The name of the feature
     * @param value    The reference value of the feature
     * @param min      The minimum value of the seek bar
     * @param max      The maximum value of the seek bar
     **/
    public void SeekBar(String featName, PInteger value, int min, int max) {
        SeekBar(featName, value, min, max, 1);
    }

    /**
     * Create a seek bar
     *
     * @param featName The name of the feature
     * @param value    The reference value of the feature
     * @param min      The minimum value of the seek bar
     * @param max      The maximum value of the seek bar
     * @param step     The step of the seek bar
     * @throws IllegalArgumentException If the value is not between min and max
     * @throws IllegalArgumentException If the step is less than 1
     * @throws IllegalArgumentException If the step is not a divisor of (max - min)
     * @throws IllegalArgumentException If the min is greater than max
     **/
    public void SeekBar(String featName, PInteger value, int min, int max, int step) {
        if (value.get() < min || value.get() > max)
            throw new IllegalArgumentException("Value must be between min and max");

        if (step < 1) throw new IllegalArgumentException("Step must be greater than 0");

        if ((max - min) % step != 0)
            throw new IllegalArgumentException("Step must be a divisor of (max - min)");

        if (min > max) throw new IllegalArgumentException("Min must be less than max");

        final LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setPadding(0, 5, 0, 5);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        linearLayout.setGravity(Gravity.CENTER);

        final TextView textView = new TextView(context);
        textView.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + value.get() + "</font>"));
        textView.setTextColor(config.TEXT_COLOR_SECONDARY);
        textView.setHighlightColor(config.TEXT_COLOR_SECONDARY);
        textView.setHintTextColor(config.TEXT_COLOR_SECONDARY);

        final SeekBar seekBar = new SeekBar(context);
        seekBar.setMax((max - min) / step);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) seekBar.setMin(0);

        seekBar.setPadding(35, 10, 35, 10);
        seekBar.setProgress(value.get());
        seekBar.getThumb().setColorFilter(config.SEEKBAR_COLOR, PorterDuff.Mode.SRC_ATOP);
        seekBar.getProgressDrawable().setColorFilter(config.SEEKBAR_PROGRESS_COLOR, PorterDuff.Mode.SRC_ATOP);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int val, boolean b) {
                value.set(val * step + min);
                final int color = value.get() <= 0 ? config.SEEKBAR_NUMBER_NEG_COLOR : config.SEEKBAR_NUMBER_POS_COLOR;
                textView.setText(Html.fromHtml(featName + ": <font color='" + color + "'>" + value.get() + "</font>"));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                // Do nothing
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                // Do nothing
            }
        });

        linearLayout.addView(textView);
        linearLayout.addView(seekBar);

        addFeature(linearLayout);
    }

    private void addFeature(View view) {
        if (curCollapse == null) featureView.addView(view);
        else curCollapse.addView(view);
    }

    /**
     * Create a collapse menu
     * <p>
     * Every feature added after this method will be added to the collapse menu
     * until {@link #endCollapse()} is called
     * <br>
     * <b>NOTE:</b> You can't start a collapse menu inside another collapse menu
     * </p>
     *
     * @param title The title of the collapse menu
     * @throws IllegalStateException If a collapse menu is already started
     **/
    public void startCollapse(String title) {
        if (curCollapse != null)
            throw new IllegalStateException("You must end the previous collapse before starting a new one");
        curCollapse = Collapse(title);
    }

    /**
     * End a collapse menu
     *
     * @throws IllegalStateException If a collapse menu is not started
     **/
    public void endCollapse() {
        if (curCollapse == null)
            throw new IllegalStateException("You must start a collapse before ending it");
        featureView.addView(curCollapse);
        curCollapse = null;
    }

    private LinearLayout Collapse(String title) {
        final LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(0, 5, 0, 0);

        final LinearLayout collapse = new LinearLayout(context);
        collapse.setLayoutParams(layoutParams);
        collapse.setVerticalGravity(Gravity.CENTER);
        collapse.setOrientation(LinearLayout.VERTICAL);

        final LinearLayout collapseSub = new LinearLayout(context);
        collapseSub.setVerticalGravity(Gravity.CENTER);
        collapseSub.setPadding(0, 5, 0, 5);
        collapseSub.setOrientation(LinearLayout.VERTICAL);
        collapseSub.setBackgroundColor(config.COLLAPSE_BG_COLOR);
        collapseSub.setVisibility(View.GONE);

        final TextView textView = new TextView(context);
        textView.setBackgroundColor(config.MENU_CATEGORY_BG_COLOR);
        textView.setText(Html.fromHtml("▽ " + title + " ▽"));
        textView.setGravity(Gravity.CENTER);
        textView.setTextColor(config.TEXT_COLOR_SECONDARY);
        textView.setTypeface(null, Typeface.BOLD);
        textView.setPadding(0, 20, 0, 20);
        textView.setOnClickListener(view -> {
            if (collapseSub.getVisibility() == View.GONE) {
                collapseSub.setVisibility(View.VISIBLE);
                textView.setText(Html.fromHtml("△ " + title + " △"));
            } else {
                collapseSub.setVisibility(View.GONE);
                textView.setText(Html.fromHtml("▽ " + title + " ▽"));
            }
        });

        collapse.addView(textView);
        collapse.addView(collapseSub);
        featureView.addView(collapse);

        return collapseSub;
    }


    /**
     * Create an input of numbers
     *
     * @param featName The name of the feature
     * @param value    The reference value of the feature
     */
    public void InputNum(String featName, PInteger value) {
        final LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);

        final LinearLayout linearLayout = new LinearLayout(context);

        final Button button = new Button(context);
        button.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + value.get() + "</font>"));
        button.setAllCaps(false);
        button.setLayoutParams(layoutParams);
        button.setBackgroundColor(config.MENU_BUTTON_BG_COLOR);
        button.setTextColor(config.TEXT_COLOR_SECONDARY);

        button.setOnLongClickListener(view -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setTitle("Enter a number");

            final EditText input = new EditText(context);
            input.setInputType(InputType.TYPE_CLASS_NUMBER);
            input.setText(String.valueOf(value.get()));
            builder.setView(input);

            builder.setPositiveButton("OK", (dialog, which) -> {
                try {
                    value.set(Integer.parseInt(input.getText().toString()));
                    button.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + value.get() + "</font>"));
                } catch (NumberFormatException e) {
                    Toast.makeText(context, "Invalid number", Toast.LENGTH_SHORT).show();
                }
            });

            builder.setNegativeButton("Cancel", (dialog, which) -> dialog.cancel());

            builder.show();

            return true;
        });

        linearLayout.addView(button);

        addFeature(linearLayout);
    }

    /**
     * Create a text input
     *
     * @param featName The name of the feature
     * @param value    The reference value of the feature
     **/
    public void InputText(String featName, PString value) {
        final LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, MATCH_PARENT);
        layoutParams.setMargins(7, 5, 7, 5);

        final LinearLayout linearLayout = new LinearLayout(context);

        final Button button = new Button(context);
        button.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + value.get() + "</font>"));
        button.setAllCaps(false);
        button.setLayoutParams(layoutParams);
        button.setBackgroundColor(config.MENU_BUTTON_BG_COLOR);
        button.setTextColor(config.TEXT_COLOR_SECONDARY);

        button.setOnLongClickListener(view -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setTitle("Enter a text");

            final EditText input = new EditText(context);
            input.setInputType(InputType.TYPE_CLASS_TEXT);
            input.setText(value.get());
            builder.setView(input);

            builder.setPositiveButton("OK", (dialog, which) -> {
                try {
                    value.set(input.getText().toString());
                    button.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + value.get() + "</font>"));
                } catch (NumberFormatException e) {
                    Toast.makeText(context, "Invalid number", Toast.LENGTH_SHORT).show();
                }
            });

            builder.setNegativeButton("Cancel", (dialog, which) -> dialog.cancel());

            builder.show();

            return true;
        });

        linearLayout.addView(button);

        addFeature(linearLayout);
    }

    /**
     * Create a text view
     * <p>
     * <b>NOTE:</b> This is not fully html supported
     * </p>
     *
     * @param text The text of the text view
     */
    public void TextView(String text) {
        final TextView textView = new TextView(context);
        textView.setText(Html.fromHtml(text));
        textView.setTextColor(config.TEXT_COLOR_SECONDARY);
        textView.setPadding(10, 5, 10, 5);

        addFeature(textView);
    }

    /**
     * Create a web text view
     * <p>
     * <b>NOTE:</b> This is fully html supported
     *
     * <b>NOTE:</b> This must be called in the main thread, use Java.scheduleOnMainThread
     * </p>
     *
     * @param html The html string of the web text view
     */
    public void WebTextView(String html) {
        WebView webView = new WebView(context);
        webView.loadData(html, "text/html", "UTF-8");
        webView.setBackgroundColor(Color.TRANSPARENT);
        webView.setPadding(0, 5, 0, 5);
        webView.getSettings().setDatabaseEnabled(false);

        addFeature(webView);
    }

    /**
     * Create a radio button
     *
     * @param featName The name of the feature
     * @param options  The options of the radio button
     * @param value    The reference value of the feature
     **/
    public void RadioButton(String featName, String[] options, PInteger value) {
        if (value.get() < 0 || value.get() >= options.length)
            throw new IllegalArgumentException("Value must be between 0 and options.length");

        if (options.length < 2) throw new IllegalArgumentException("Options must be at least 2");

        final TextView textView = new TextView(context);
        textView.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + options[value.get()] + "</font>"));
        textView.setTextColor(config.TEXT_COLOR_SECONDARY);

        final RadioGroup radioGroup = new RadioGroup(context);
        radioGroup.setPadding(10, 5, 10, 5);
        radioGroup.setOrientation(LinearLayout.VERTICAL);
        radioGroup.addView(textView);

        for (int i = 0; i < options.length; i++) {
            final RadioButton radioButton = new RadioButton(context);
            radioButton.setText(options[i]);
            radioButton.setTextColor(Color.LTGRAY);
            radioButton.setButtonTintList(ColorStateList.valueOf(config.RADIO_BUTTON_COLOR));

            final int finalI = i;
            radioButton.setOnClickListener(view -> {
                value.set(finalI);
                textView.setText(Html.fromHtml(featName + ": <font color='" + config.NUMBER_TEXT_COLOR + "'>" + options[value.get()] + "</font>"));
            });

            radioGroup.addView(radioButton);
        }

        radioGroup.check(radioGroup.getChildAt(value.get()).getId());

        addFeature(radioGroup);
    }

    /**
     * Create a button that opens a link
     *
     * @param featName The name of the feature
     * @param url      The url to open
     **/
    public void ButtonLink(String featName, String url) {
        final Button btn = Button(featName);
        btn.setOnClickListener(view -> {
            Intent intent = new Intent(Intent.ACTION_VIEW, android.net.Uri.parse(url));
            context.startActivity(intent);
        });

        addFeature(btn);
    }

    /**
     * Create a checkbox
     *
     * @param featName The name of the feature
     * @param value    The reference value of the feature
     **/
    public void CheckBox(String featName, PBoolean value) {
        final CheckBox checkbox = new CheckBox(context);
        checkbox.setText(Html.fromHtml(featName));
        checkbox.setTextColor(config.TEXT_COLOR_SECONDARY);
        checkbox.setButtonTintList(ColorStateList.valueOf(config.CHECKBOX_COLOR));
        checkbox.setOnCheckedChangeListener((buttonView, isChecked) -> value.set(isChecked));

        addFeature(checkbox);
    }

    @SuppressLint("ClickableViewAccessibility")
    private void Init() {
        setOnTouchListener(onToucheListener());
        setLayoutParams(new ViewGroup.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));

        featureView.setOrientation(LinearLayout.VERTICAL);

        wmParams.gravity = Gravity.TOP | Gravity.START;
        wmParams.x = position.x;
        wmParams.y = position.y;

        mCollapsed.setVisibility(View.VISIBLE);
        mCollapsed.setAlpha(config.MENU_COLLAPSED_ALPHA);

        mExpanded.setVisibility(View.GONE);
        mExpanded.setBackgroundColor(config.MENU_BG_COLOR);
        mExpanded.setOrientation(LinearLayout.VERTICAL);
        mExpanded.setLayoutParams(new LinearLayout.LayoutParams(dp(config.MENU_WIDTH), WRAP_CONTENT));

        launcherIcon.setScaleType(ImageView.ScaleType.FIT_XY);
        launcherIcon.setLayoutParams(new RelativeLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));
        int launcherIconSize = (int) (TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, config.MENU_LAUNCHER_ICON_SIZE, context.getResources().getDisplayMetrics()));
        ViewGroup.LayoutParams launcherIconLayoutParams = launcherIcon.getLayoutParams();
        launcherIconLayoutParams.width = launcherIconSize;
        launcherIconLayoutParams.height = launcherIconSize;
        byte[] launcherIconDecodedImg = Base64.decode(config.MENU_LAUNCHER_ICON, Base64.DEFAULT);
        launcherIcon.setImageBitmap(BitmapFactory.decodeByteArray(launcherIconDecodedImg, 0, launcherIconDecodedImg.length));
        ((MarginLayoutParams) launcherIconLayoutParams).topMargin = convertDipToPixels(10);
        launcherIcon.setOnTouchListener(onToucheListener());
        launcherIcon.setOnClickListener(view -> expandMenu());

        RelativeLayout mHeader = new RelativeLayout(this.context);
        mHeader.setPadding(10, 5, 10, 5);
        mHeader.setVerticalGravity(16);

        TextView title = new TextView(context);
        title.setTextColor(config.TEXT_COLOR_PRIMARY);
        title.setTypeface(null, Typeface.BOLD);
        title.setText(config.MENU_TITLE);
        title.setTextSize(18.0f);
        title.setGravity(Gravity.CENTER);
        RelativeLayout.LayoutParams titleLayoutParams = new RelativeLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT);
        titleLayoutParams.addRule(RelativeLayout.CENTER_HORIZONTAL);
        title.setLayoutParams(titleLayoutParams);

        TextView subtitle = new TextView(context);
        subtitle.setText(Html.fromHtml(config.MENU_SUBTITLE));
        subtitle.setEllipsize(TextUtils.TruncateAt.MARQUEE);
        subtitle.setMarqueeRepeatLimit(-1);
        subtitle.setSingleLine(true);
        subtitle.setSelected(true);
        subtitle.setTextColor(config.TEXT_COLOR_PRIMARY);
        subtitle.setTextSize(10.0f);
        subtitle.setGravity(Gravity.CENTER);
        subtitle.setPadding(0, 0, 0, 5);

        ScrollView scrollView = new ScrollView(context);
        ViewGroup.LayoutParams scrollViewLayoutParams = new LinearLayout.LayoutParams(MATCH_PARENT, dp(config.MENU_HEIGHT));
        scrollView.setLayoutParams(scrollViewLayoutParams);
        scrollView.setBackgroundColor(config.MENU_FEATURE_BG_COLOR);

        RelativeLayout mFooter = new RelativeLayout(context);
        mFooter.setPadding(10, 3, 10, 3);
        mFooter.setVerticalGravity(Gravity.CENTER);

        RelativeLayout.LayoutParams hideBtnLayoutParams = new RelativeLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT);
        hideBtnLayoutParams.addRule(ALIGN_PARENT_LEFT);

        Button hideBtn = new Button(context);
        hideBtn.setText(config.MENU_HIDE_BUTTON_TEXT);
        hideBtn.setLayoutParams(hideBtnLayoutParams);
        hideBtn.setBackgroundColor(Color.TRANSPARENT);
        hideBtn.setTextColor(config.TEXT_COLOR_PRIMARY);
        hideBtn.setOnClickListener(view -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setTitle("Are you sure you want to hide the menu?");
            builder.setMessage("You can always show it again by clicking on the icon. Remember the hidden icon position.");
            builder.setPositiveButton("Yes", (dialog, which) -> {
                hideMenu();
                Toast.makeText(context, "Menu hidden", Toast.LENGTH_SHORT).show();
            });

            builder.setNegativeButton("No", (dialog, which) -> {
                // Do nothing
            });

            AlertDialog dialog = builder.create();
            dialog.show();
        });

        hideBtn.setOnLongClickListener(view -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(context);
            builder.setTitle("Are you sure you want to kill the menu?");
            builder.setMessage("Killing the menu will remove it from the screen. You can always restart the app to get it back.");
            builder.setPositiveButton("Yes", (dialog, which) -> {
                killMenu();
                Toast.makeText(context, "Menu killed", Toast.LENGTH_SHORT).show();
            });

            builder.setNegativeButton("No", (dialog, which) -> {
                // Do nothing
            });

            AlertDialog dialog = builder.create();
            dialog.show();

            return true;
        });

        RelativeLayout.LayoutParams closeBtnLayoutParams = new RelativeLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT);
        closeBtnLayoutParams.addRule(ALIGN_PARENT_RIGHT);
        Button closeBtn = new Button(context);
        closeBtn.setText(config.MENU_CLOSE_BUTTON_TEXT);
        closeBtn.setLayoutParams(closeBtnLayoutParams);
        closeBtn.setBackgroundColor(Color.TRANSPARENT);
        closeBtn.setTextColor(config.TEXT_COLOR_PRIMARY);
        closeBtn.setOnClickListener(view -> collapseMenu());

        mHeader.addView(title);
        scrollView.addView(featureView);

        mFooter.addView(hideBtn);
        mFooter.addView(closeBtn);

        mExpanded.addView(mHeader);
        mExpanded.addView(subtitle);
        mExpanded.addView(scrollView);
        mExpanded.addView(mFooter);

        mCollapsed.addView(launcherIcon);

        addView(mCollapsed);
        addView(mExpanded);
    }

    /**
     * Attach the menu to the screen
     **/
    public void attach() {
        windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        windowManager.addView(this, wmParams);
    }

    /**
     * Detach the menu from the screen
     **/
    public void detach() {
        windowManager.removeView(this);
    }

    private void hideMenu() {
        isCollapsed = true;
        mCollapsed.setVisibility(View.VISIBLE);
        mCollapsed.setAlpha(0);
        mExpanded.setVisibility(View.GONE);
    }

    private void collapseMenu() {
        isCollapsed = true;
        mCollapsed.setVisibility(View.VISIBLE);
        mCollapsed.setAlpha(config.MENU_COLLAPSED_ALPHA);
        mExpanded.setVisibility(View.GONE);
    }

    private void expandMenu() {
        isCollapsed = false;
        mCollapsed.setVisibility(View.GONE);
        mExpanded.setVisibility(View.VISIBLE);
    }

    private void killMenu() {
        removeAllViews();
        setVisibility(View.GONE);
    }

    @SuppressLint("ClickableViewAccessibility")
    private OnTouchListener onToucheListener() {
        final Menu menu = this;
        Point initialTouch = new Point(0, 0);
        Point initialPosition = new Point(0, 0);

        return (view, event) -> {
            switch (event.getAction()) {
                case MotionEvent.ACTION_DOWN:
                    initialTouch.x = (int) event.getRawX();
                    initialTouch.y = (int) event.getRawY();
                    initialPosition.x = position.x;
                    initialPosition.y = position.y;
                    break;
                case MotionEvent.ACTION_MOVE:
                    setAlpha(0.5f);

                    int x = initialPosition.x + (int) (event.getRawX() - initialTouch.x);
                    int y = initialPosition.y + (int) (event.getRawY() - initialTouch.y);
                    position.x = x;
                    position.y = y;
                    WindowManager.LayoutParams params = (WindowManager.LayoutParams) menu.getLayoutParams();
                    params.x = x;
                    params.y = y;
                    windowManager.updateViewLayout(menu, params);
                    break;
                case MotionEvent.ACTION_UP:
                    setAlpha(1.0f);

                    int rawX = (int) (event.getRawX() - initialTouch.x);
                    int rawY = (int) (event.getRawY() - initialTouch.y);

                    if (rawX < 5 && rawY < 5 && isCollapsed) expandMenu();

                    break;
                default:
                    return false;
            }

            return true;
        };
    }

    private int dp(int i) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, (float) i, context.getResources().getDisplayMetrics());
    }

    private int convertDipToPixels(int i) {
        return (int) ((i * context.getResources().getDisplayMetrics().density) + 0.5f);
    }
}