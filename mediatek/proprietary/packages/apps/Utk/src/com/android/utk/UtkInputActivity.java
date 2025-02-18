/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.utk;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.ContentObserver;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.telephony.SubscriptionManager;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextWatcher;
import android.text.method.PasswordTransformationMethod;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.TextView.BufferType;

import com.android.internal.telephony.cdma.utk.FontSize;
import com.android.internal.telephony.cdma.utk.Input;
import com.android.internal.telephony.cdma.utk.UtkLog;
import com.android.internal.telephony.PhoneConstants;
import com.android.internal.telephony.PhoneFactory;

//Utk modification for TDD data only
import com.mediatek.internal.telephony.cdma.CdmaFeatureOptionUtils;
import com.mediatek.internal.telephony.ltedc.svlte.SvlteRatController;
import com.mediatek.internal.telephony.ltedc.svlte.SvlteRatController.RoamingMode;
import com.mediatek.telephony.TelephonyManagerEx;
import com.mediatek.internal.telephony.ltedc.LteDcPhoneProxy;
/**
 * Display a request for a text input a long with a text edit form.
 */
public class UtkInputActivity extends Activity implements View.OnClickListener,
        TextWatcher {

    // Members
    private int mState;
    private Context mContext;
    private EditText mTextIn = null;
    private TextView mPromptView = null;
    private View mYesNoLayout = null;
    private View mNormalLayout = null;
    private Input mUtkInput = null;

    // Constants
    private static final int STATE_TEXT = 1;
    private static final int STATE_YES_NO = 2;

    static final String YES_STR_RESPONSE = "YES";
    static final String NO_STR_RESPONSE = "NO";

    // Font size factor values.
    static final float NORMAL_FONT_FACTOR = 1;
    static final float LARGE_FONT_FACTOR = 2;
    static final float SMALL_FONT_FACTOR = (1 / 2);

    // message id for time out
    private static final int MSG_ID_TIMEOUT = 1;

    private int mPatternLteDataOnly = -1;
    private boolean mIrSwitchflag = false;
    
    private int mPhoneId = -1;

    Handler mTimeoutHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch(msg.what) {
            case MSG_ID_TIMEOUT:
                //mAcceptUsersInput = false;
                sendResponse(UtkAppService.RES_ID_TIMEOUT);
                finish();
                break;
            }
        }
    };

    // Click listener to handle buttons press..
    public void onClick(View v) {
        String input = null;

        switch (v.getId()) {
        case R.id.button_ok:
            // Check that text entered is valid .
            if (!verfiyTypedText()) {
                return;
            }
            input = mTextIn.getText().toString();
            break;
        // Yes/No layout buttons.
        case R.id.button_yes:
            input = YES_STR_RESPONSE;
            break;
        case R.id.button_no:
            input = NO_STR_RESPONSE;
            break;
        }
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  input = " + input);
        sendResponse(UtkAppService.RES_ID_INPUT, input, false);
        finish();
    }

    //Utk modification for TDD data only
    private ContentObserver m4GDataModeObserver = new ContentObserver(new Handler()) {
        @Override
        public void onChange(boolean selfChange) {
            UtkLog.d(this, "4G data only mode changed!");
            int subId = SubscriptionManager.getSubIdUsingPhoneId(mPhoneId);
            UtkLog.d(this, "subId = " + subId);
            mPatternLteDataOnly = Settings.Global.getInt(mContext.getContentResolver(),
                    TelephonyManagerEx.getDefault().getCdmaRatModeKey(subId),
                    TelephonyManagerEx.SVLTE_RAT_MODE_4G);
            UtkLog.d(this, "mPatternLteDataOnly = " + mPatternLteDataOnly);
            if (TelephonyManagerEx.SVLTE_RAT_MODE_4G_DATA_ONLY == mPatternLteDataOnly) {
                UtkLog.d(this, "UtkInputActivity.finish()");
                UtkInputActivity.this.cancelTimeOut();
                UtkInputActivity.this.finish();
            }
        }
    };

    //Utk Modification for IR
     //Add for Utk IR case
    private final IntentFilter mIRStateChangeFilter = new IntentFilter();

    private final BroadcastReceiver mIRStateChangeReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            //Temp solution for build pass
            if ((SvlteRatController.INTENT_ACTION_START_SWITCH_ROAMING_MODE.equals(
                                                                            intent.getAction())) ||
                (SvlteRatController.INTENT_ACTION_FINISH_SWITCH_ROAMING_MODE.equals(
                                                                            intent.getAction()))) {
                //Query RAT Mode
                //SvlteRatController mLteRatController = SvlteRatController.getInstance();
                int phoneId = intent.getIntExtra(PhoneConstants.PHONE_KEY, -1);
                UtkLog.d(this, "received IR state changed broadcast at phone " + phoneId);
                if (phoneId == mPhoneId) {
                    LteDcPhoneProxy lteDcPhoneProxy =
                            (LteDcPhoneProxy) PhoneFactory.getPhone(mPhoneId);
                    SvlteRatController mLteRatController = null;
                    if (lteDcPhoneProxy != null) {
                        mLteRatController = lteDcPhoneProxy.getSvlteRatController();
                    }
                    RoamingMode mRoamingMode = null;
                    if (mLteRatController != null) {
                        mRoamingMode = mLteRatController.getRoamingMode();
                        UtkLog.d(this, "mIRStateChangeReceiver mRoamingMode = " + mRoamingMode);
                        if (RoamingMode.ROAMING_MODE_NORMAL_ROAMING == mRoamingMode) {
                            mIrSwitchflag = true;
                            UtkInputActivity.this.cancelTimeOut();
                            UtkInputActivity.this.finish();
                        }
                    }
                }
            }
        }
    };

    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        // Set the layout for this activity.
        requestWindowFeature(Window.FEATURE_LEFT_ICON);
        setContentView(R.layout.utk_input);

        // Initialize members
        mTextIn = (EditText) this.findViewById(R.id.in_text);
        mPromptView = (TextView) this.findViewById(R.id.prompt);

        // Set buttons listeners.
        Button okButton = (Button) findViewById(R.id.button_ok);
        Button yesButton = (Button) findViewById(R.id.button_yes);
        Button noButton = (Button) findViewById(R.id.button_no);

        okButton.setOnClickListener(this);
        yesButton.setOnClickListener(this);
        noButton.setOnClickListener(this);

        mYesNoLayout = findViewById(R.id.yes_no_layout);
        mNormalLayout = findViewById(R.id.normal_layout);

        // Get the calling intent type: text/key, and setup the
        // display parameters.
        Intent intent = getIntent();
        if (intent != null) {
            mUtkInput = intent.getParcelableExtra("INPUT");
            if (mUtkInput == null) {
                finish();
            } else {
                mState = mUtkInput.yesNo ? STATE_YES_NO : STATE_TEXT;
                configInputDisplay();
            }
            mPhoneId = intent.getIntExtra(UtkAppService.PHONE_ID, -1);
            UtkLog.d(this, "mPhoneId :" + mPhoneId);
        } else {
            finish();
        }
        mContext = getBaseContext();

        //Utk modification for TDD data only
        if (CdmaFeatureOptionUtils.isCdmaLteDcSupport()) {
            //UtkLog.d(this, "register 4G data only observer");
            mPatternLteDataOnly = -1;
            register4GDataModeObserver();

            //Utk modification for IR
            mIrSwitchflag = false;
            mIRStateChangeFilter.addAction(
                                SvlteRatController.INTENT_ACTION_START_SWITCH_ROAMING_MODE);
            mIRStateChangeFilter.addAction(
                                SvlteRatController.INTENT_ACTION_FINISH_SWITCH_ROAMING_MODE);
            registerReceiver(mIRStateChangeReceiver, mIRStateChangeFilter);
        }
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

        mTextIn.addTextChangedListener(this);
    }

    @Override
    public void onResume() {
        super.onResume();
        startTimeOut();
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  onResume");
    }

    @Override
    public void onPause() {
        super.onPause();
        cancelTimeOut();
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  onPause");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        cancelTimeOut();

        //Utk modification for TDD data only
        if (CdmaFeatureOptionUtils.isCdmaLteDcSupport()) {
            UtkLog.d(this, "UtkInputActivity onDestory");
            unregister4GDataModeObserver();
            //Utk modification for IR
            unregisterReceiver(mIRStateChangeReceiver);

            if ((TelephonyManagerEx.SVLTE_RAT_MODE_4G_DATA_ONLY == mPatternLteDataOnly) ||
                (mIrSwitchflag)) {
                sendResponse(UtkAppService.RES_ID_END_SESSION);
            }
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
        case KeyEvent.KEYCODE_BACK:
            sendResponse(UtkAppService.RES_ID_BACKWARD, null, false);
            finish();
            break;
        }
        return super.onKeyDown(keyCode, event);
    }

    private void sendResponse(int resId) {
        sendResponse(resId, null, false);
    }

    private void sendResponse(int resId, String input, boolean help) {
        Bundle args = new Bundle();
        args.putInt(UtkAppService.OPCODE, UtkAppService.OP_RESPONSE);
        args.putInt(UtkAppService.PHONE_ID, mPhoneId);
        args.putInt(UtkAppService.RES_ID, resId);
        if (input != null) {
            args.putString(UtkAppService.INPUT, input);
        }
        args.putBoolean(UtkAppService.HELP, help);
        mContext.startService(new Intent(mContext, UtkAppService.class)
                .putExtras(args));
    }

    @Override
    public boolean onCreateOptionsMenu(android.view.Menu menu) {
        super.onCreateOptionsMenu(menu);
        menu.add(android.view.Menu.NONE, UtkApp.MENU_ID_END_SESSION, 1,
                R.string.menu_end_session);
        menu.add(0, UtkApp.MENU_ID_HELP, 2, R.string.help);

        return true;
    }

    @Override
    public boolean onPrepareOptionsMenu(android.view.Menu menu) {
        super.onPrepareOptionsMenu(menu);
        menu.findItem(UtkApp.MENU_ID_END_SESSION).setVisible(true);
        menu.findItem(UtkApp.MENU_ID_HELP).setVisible(mUtkInput.helpAvailable);

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  item.getItemId() = " + item.getItemId());
        switch (item.getItemId()) {
        case UtkApp.MENU_ID_END_SESSION:
            sendResponse(UtkAppService.RES_ID_END_SESSION);
            finish();
            return true;
        case UtkApp.MENU_ID_HELP:
            sendResponse(UtkAppService.RES_ID_INPUT, "", true);
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    public void beforeTextChanged(CharSequence s, int start, int count,
            int after) {
    }

    public void onTextChanged(CharSequence s, int start, int before, int count) {
        // Reset timeout.
        startTimeOut();
    }

    public void afterTextChanged(Editable s) {
    }

    private boolean verfiyTypedText() {
        UtkLog.d("verfiyTypedText","TextLength = "+mTextIn.getText().length());
        // If not enough input was typed in stay on the edit screen.
        if (mTextIn.getText().length() >= mUtkInput.minLen && mTextIn.getText().length() <= mUtkInput.maxLen)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    private void cancelTimeOut() {
        mTimeoutHandler.removeMessages(MSG_ID_TIMEOUT);
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  cancelTimeOut");
    }

    private void startTimeOut() {
        cancelTimeOut();
        mTimeoutHandler.sendMessageDelayed(mTimeoutHandler
                .obtainMessage(MSG_ID_TIMEOUT), UtkApp.UI_TIMEOUT);
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  startTimeOut");
    }

    private void configInputDisplay() {
        TextView numOfCharsView = (TextView) findViewById(R.id.num_of_chars);
        TextView inTypeView = (TextView) findViewById(R.id.input_type);

        int inTypeId = R.string.alphabet;

        // set the prompt.
        mPromptView.setText(mUtkInput.text);

        // Set input type (alphabet/digit) info close to the InText form.
        if (mUtkInput.digitOnly) {
            mTextIn.setKeyListener(UtkDigitsKeyListener.getInstance());
            inTypeId = R.string.digits;
        }
        inTypeView.setText(inTypeId);

        if (mUtkInput.icon != null) {
            setFeatureDrawable(Window.FEATURE_LEFT_ICON, new BitmapDrawable(
                    mUtkInput.icon));
        }
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  mState = " + mState);
        // Handle specific global and text attributes.
        switch (mState) {
        case STATE_TEXT:
            int maxLen = mUtkInput.maxLen;
            int minLen = mUtkInput.minLen;
            mTextIn.setFilters(new InputFilter[] {new InputFilter.LengthFilter(
                    maxLen)});

            // Set number of chars info.
            String lengthLimit = String.valueOf(minLen);
            if (maxLen != minLen) {
                lengthLimit = minLen + " - " + maxLen;
            }
            numOfCharsView.setText(lengthLimit);

            if (!mUtkInput.echo) {
                mTextIn.setTransformationMethod(PasswordTransformationMethod
                        .getInstance());
            }
            // Set default text if present.
            if (mUtkInput.defaultText != null) {
                mTextIn.setText(mUtkInput.defaultText);
            } else {
                // make sure the text is cleared
                mTextIn.setText("", BufferType.EDITABLE);
            }

            break;
        case STATE_YES_NO:
            // Set display mode - normal / yes-no layout
            mYesNoLayout.setVisibility(View.VISIBLE);
            mNormalLayout.setVisibility(View.GONE);
            break;
        }
    }

    private float getFontSizeFactor(FontSize size) {
        final float[] fontSizes =
            {NORMAL_FONT_FACTOR, LARGE_FONT_FACTOR, SMALL_FONT_FACTOR};

        return fontSizes[size.ordinal()];
    }

    //Utk modification for TDD data only
    private void register4GDataModeObserver() {
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  register 4G data only observer");
        int subId = SubscriptionManager.getSubIdUsingPhoneId(mPhoneId);
        UtkLog.d(this, "subId = " + subId);
        Uri uri = Settings.Global.getUriFor(
            TelephonyManagerEx.getDefault().getCdmaRatModeKey(subId));
        mContext.getContentResolver().registerContentObserver(
            uri, true, m4GDataModeObserver);
    }

    private void unregister4GDataModeObserver() {
        UtkLog.d(this, "mPhoneId:" + mPhoneId + "  unregister 4G data only observer");
        mContext.getContentResolver().unregisterContentObserver(
                m4GDataModeObserver);
    }
}
