/*******************************************************************************
 * Software Name : RCS IMS Stack
 *
 * Copyright (C) 2010 France Telecom S.A.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
package com.mediatek.rcse.plugin.phone;

import com.mediatek.rcse.api.Logger;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Video surface view.
 *
 * @author jexa7410
 */
public class VideoSurfaceView extends SurfaceView implements
        VideoSurface {
    /**
     * No aspect ratio.
     */
    public static final float NO_RATIO = 0.0f;
    /**
     * Display area aspect ratio.
     */
    private float mAspectRatio = NO_RATIO;
    /**
     * Surface has been created state.
     */
    private boolean mSurfaceCreated = false;
    /**
     * Surface holder.
     */
    private SurfaceHolder mHolder;

    public int mWidth = -1;
    
    public boolean isReceiver = false;

    /**
     * Constructor.
     *
     * @param context Context
     */
    public VideoSurfaceView(Context context) {
        super(context);
        init();
    }
    /**
     * Constructor.
     *
     * @param context Context
     * @param attrs Attributes
     */
    public VideoSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
    /**
     * Constructor.
     *
     * @param context Context
     * @param attrs Attributes
     * @param defStyle Style
     */
    public VideoSurfaceView(Context context, AttributeSet attrs,
            int defStyle) {
        super(context, attrs, defStyle);
        init();
    }
    /**
     * Set aspect ration according to desired width and height.
     *
     * @param width Width
     * @param height Height
     */
    public void setAspectRatio(int width, int height) {
        setAspectRatio((float) width / (float) height);
    }
    /**
     * Set aspect ratio.
     *
     * @param ratio Ratio
     */
    public void setAspectRatio(float ratio) {
        if (mAspectRatio != ratio) {
            mAspectRatio = ratio;
            requestLayout();
            invalidate();
        }
    }
    /**
     * Ensure aspect ratio.
     *
     * @param widthMeasureSpec Width
     * @param heightMeasureSpec Heigh
     */
    protected void onMeasure(int widthMeasureSpec,
            int heightMeasureSpec) {
        if (mAspectRatio != NO_RATIO) {
            int widthSpecSize = MeasureSpec.getSize(widthMeasureSpec);
            int heightSpecSize = MeasureSpec
                    .getSize(heightMeasureSpec);
            int width = widthSpecSize;
            int height = heightSpecSize;
            if (width > 0 && height > 0) {
                float defaultRatio = ((float) width)
                        / ((float) height);
                if (defaultRatio < mAspectRatio) {
                    // Need to reduce height
                    height = (int) (width / mAspectRatio);
                } else if (defaultRatio > mAspectRatio) {
                    width = (int) (height * mAspectRatio);
                }
                width = Math.min(width, widthSpecSize);
                height = Math.min(height, heightSpecSize);
                if(mWidth!= -1)
                	setMeasuredDimension(mWidth, height);
                else
                	setMeasuredDimension(width, height);
                return;
            }
        }
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }
    /**
     * Set image from a bitmap.
     *
     * @param bmp Bitmap
     */
    public void setImage(Bitmap bmp) {
        if (mSurfaceCreated) {
            Canvas canvas = null;
            try {
                synchronized (mHolder) {
                    canvas = mHolder.lockCanvas();
                }
            } finally {
                if (canvas != null) {
                    // First clear screen
                    canvas.drawARGB(255, 0, 0, 0);
                    // Then draw bmp
                    canvas.drawBitmap(bmp, null,
                            canvas.getClipBounds(), null);
                    mHolder.unlockCanvasAndPost(canvas);
                }
            }
        }
    }
    /* (non-Javadoc)
     * @see com.mediatek.rcse.plugin.phone.VideoSurface#clearImage()
     */
    @Override
    public void clearImage() {
        if (mSurfaceCreated) {
            Canvas canvas = null;
            try {
                synchronized (mHolder) {
                    canvas = mHolder.lockCanvas();
                }
            } finally {
                if (canvas != null) {
                    // Clear screen
                    canvas.drawARGB(255, 0, 0, 0);
                    mHolder.unlockCanvasAndPost(canvas);
                }
            }
        }
    }
    /**
     * Init the view.
     */
    private void init() {
        // Get a surface holder
        mHolder = this.getHolder();
        mHolder.addCallback(mSurfaceCallback);
    }

    /**
     * Surface holder callback.
     */
    private SurfaceHolder.Callback mSurfaceCallback = new SurfaceHolder.Callback() {
        public void surfaceChanged(SurfaceHolder holder, int format,
                int w, int h) {
        }
        public void surfaceCreated(SurfaceHolder holder) {
            mSurfaceCreated = true;
            Logger.d("surfaceCreated", " surfaceCreated commented Entry isReceiver" + isReceiver);
           if(isReceiver)
        	   clearImage();
            
        }
        public void surfaceDestroyed(SurfaceHolder holder) {
        	Logger.d("surfaceCreated", " surfaceDestroyed Entry");
            mSurfaceCreated = false;
        }
    };
}
