package com.mediatek.rcs.pam.ui.conversation;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.ViewConfiguration;
import android.view.ViewGroup;
import android.widget.Scroller;

public class LevelControlLayout extends ViewGroup {

    private static final String TAG = "ScrollControlLayout";

    private boolean mCanMove = true;

    private Scroller mScroller;
    private VelocityTracker mVelocityTracker;

    private int mCurScreen;
    private int mDefaultScreen = 1;

    private static final int TOUCH_STATE_REST = 0;
    private static final int TOUCH_STATE_SCROLLING = 1;

    private int mTouchState = TOUCH_STATE_REST;
    private static final int SNAP_VELOCITY = 600;

    private int mTouchSlop;

    private float mLastMotionX;
    private float mLastMotionY;

    private OnScrollToScreenListener mOnScrollToScreen = null;

    public LevelControlLayout(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public LevelControlLayout(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        mScroller = new Scroller(context);
        mCurScreen = mDefaultScreen;
        mTouchSlop = ViewConfiguration.get(getContext()).getScaledTouchSlop();
    }

    public boolean isScrolling() {
        return !mScroller.isFinished();
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        int childLeft = 0;
        final int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            final View childView = getChildAt(i);
            if (childView.getVisibility() != View.GONE) {
                final int childWidth = childView.getMeasuredWidth();
                childView.layout(childLeft, 0, childLeft + childWidth,
                        childView.getMeasuredHeight());
                childLeft += childWidth;
            }
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        final int width = MeasureSpec.getSize(widthMeasureSpec);
        final int widthMode = MeasureSpec.getMode(widthMeasureSpec);
        if (widthMode != MeasureSpec.EXACTLY) {
            throw new IllegalStateException(
                    "ScrollLayout only canmCurScreen run at EXACTLY mode!");
        }

        final int heightMode = MeasureSpec.getMode(heightMeasureSpec);
        if (heightMode != MeasureSpec.EXACTLY) {
            throw new IllegalStateException(
                    "ScrollLayout only can run at EXACTLY mode!");
        }
        // The children are given the same width and height as the scrollLayout
        final int count = getChildCount();
        for (int i = 0; i < count; i++) {
            getChildAt(i).measure(widthMeasureSpec, heightMeasureSpec);
        }
        // Log.e(TAG, "moving to screen "+mCurScreen);
        scrollTo(mCurScreen * width, 0);
        doScrollAction(mCurScreen);
    }

    public void setTouchMove(boolean canMove) {
        this.mCanMove = canMove;
    }

    public void snapToDestination() {
        final int screenWidth = getWidth();
        final int destScreen = (getScrollX() + screenWidth / 2) / screenWidth;
        snapToScreen(destScreen);
    }

    public void snapToScreen(int whichScreen) {
        whichScreen = Math.max(0, Math.min(whichScreen, getChildCount() - 1));
        if (getScrollX() != (whichScreen * getWidth())) {
            final int delta = whichScreen * getWidth() - getScrollX();
            mScroller.startScroll(getScrollX(), 0, delta, 0,
                    Math.abs(delta) * 2);
            mCurScreen = whichScreen;
            doScrollAction(mCurScreen);
            invalidate();
        }
    }

    public void setToScreen(int whichScreen) {
        whichScreen = Math.max(0, Math.min(whichScreen, getChildCount() - 1));
        mCurScreen = whichScreen;
        scrollTo(whichScreen * getWidth(), 0);
        doScrollAction(whichScreen);
    }

    public int getCurScreen() {
        return mCurScreen;
    }

    public void autoRecovery() {
        mScroller.abortAnimation();
    }

    @Override
    public void computeScroll() {
        // TODO Auto-generated method stub
        if (mScroller.computeScrollOffset()) {
            scrollTo(mScroller.getCurrX(), mScroller.getCurrY());
            postInvalidate();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (mVelocityTracker == null) {
            mVelocityTracker = VelocityTracker.obtain();
        }

        if (!mCanMove) {
            return true;
        }

        mVelocityTracker.addMovement(event);
        final int action = event.getAction();
        final float x = event.getX();
        // final float y = event.getY();
        switch (action) {
        case MotionEvent.ACTION_DOWN:
            if (!mScroller.isFinished()) {
                mScroller.abortAnimation();
            }
            mLastMotionX = x;
            break;
        case MotionEvent.ACTION_MOVE:
            int deltaX = (int) (mLastMotionX - x);
            int scrollX = (int) getScrollX();
            int childCount = getChildCount();
            mLastMotionX = x;

            if ((deltaX < 0 && Math.abs(deltaX) > scrollX)
                    || (deltaX > 0 && deltaX > ((childCount - 1) * getWidth() - scrollX))) {
                // do nothing
            } else {
                scrollBy(deltaX, 0);
            }
            break;
        case MotionEvent.ACTION_UP:
            final VelocityTracker velocityTrackers = mVelocityTracker;
            mVelocityTracker.computeCurrentVelocity(1000);
            int velocityX = (int) velocityTrackers.getXVelocity();
            if (velocityX > SNAP_VELOCITY && mCurScreen > 0) {
                // Fling enough to move left
                Log.e(TAG, "snap left");
                snapToScreen(mCurScreen - 1);
            } else if (velocityX < -SNAP_VELOCITY
                    && mCurScreen < getChildCount() - 1) {
                // Fling enough to move right
                snapToScreen(mCurScreen + 1);
            } else {
                snapToDestination();
            }

            if (mVelocityTracker != null) {
                mVelocityTracker.recycle();
                mVelocityTracker = null;
            }
            mTouchState = TOUCH_STATE_REST;
            break;
        case MotionEvent.ACTION_CANCEL:
            mTouchState = TOUCH_STATE_REST;
            break;
        default:
            break;
        }
        return true;
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        // TODO Auto-generated method stub
        final int action = ev.getAction();
        if ((action == MotionEvent.ACTION_MOVE)
                && (mTouchState != TOUCH_STATE_REST)) {
            return true;
        }
        final float x = ev.getX();
        final float y = ev.getY();
        switch (action) {
        case MotionEvent.ACTION_DOWN:
            mLastMotionX = x;
            mLastMotionY = y;
            mTouchState = mScroller.isFinished() ? TOUCH_STATE_REST
                    : TOUCH_STATE_SCROLLING;
            break;
        case MotionEvent.ACTION_MOVE:
            final int xDiff = (int) Math.abs(mLastMotionX - x);
            if (xDiff > mTouchSlop) {
                if (Math.abs(mLastMotionY - y) / Math.abs(mLastMotionX - x) < 1) {
                    mTouchState = TOUCH_STATE_SCROLLING;
                }
            }
            break;
        case MotionEvent.ACTION_CANCEL:
        case MotionEvent.ACTION_UP:
            mTouchState = TOUCH_STATE_REST;
            break;
        default:
            break;
        }
        return mTouchState != TOUCH_STATE_REST;
    }

    private void doScrollAction(int whichScreen) {
        if (mOnScrollToScreen != null) {
            mOnScrollToScreen.doAction(whichScreen);
        }
    }

    public void setOnScrollToScreen(
            OnScrollToScreenListener paramOnScrollToScreen) {
        mOnScrollToScreen = paramOnScrollToScreen;
    }

    public void setDefaultScreen(int position) {
        mCurScreen = position;
    }

    public interface OnScrollToScreenListener {
        public void doAction(int whichScreen);
    }
}
