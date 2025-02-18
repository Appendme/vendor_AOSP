/*
 * Copyright (C) 2007 The Android Open Source Project
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
 */

package com.mediatek.common.widget.tests.gridview;


import android.test.ActivityInstrumentationTestCase;
import android.test.suitebuilder.annotation.MediumTest;
import android.widget.GridView;

public class GridStackFromBottomTest extends ActivityInstrumentationTestCase<GridStackFromBottom> {
    private GridStackFromBottom mActivity;
    private GridView mGridView;

    public GridStackFromBottomTest() {
        super("com.mediatek.common.widget.tests", GridStackFromBottom.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        mActivity = getActivity();
        mGridView = getActivity().getGridView();
    }

    @MediumTest
    public void testPreconditions() {
        assertNotNull(mActivity);
        assertNotNull(mGridView);

        // Last item should be selected
        assertEquals(mGridView.getAdapter().getCount() - 1, mGridView.getSelectedItemPosition());
    }
}
