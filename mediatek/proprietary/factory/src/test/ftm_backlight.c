/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include <common.h>
#include <miniui.h>
#include <ftm.h>

#ifdef FEATURE_FTM_LCD

#define BRIGHTNESS_FILE "/sys/class/leds/lcd-backlight/brightness"

static int seq_index = 0;
static int brightness_seq[] = {
	50, 100, 150, 200, 255, -1
};

#define TAG	"[BACKLIGHT] "
#define mod_to_backlight(p)	(backlight_module *)((char *)(p) + sizeof(struct ftm_module))

#define ARY_SIZE(x)     (sizeof((x)) / sizeof((x[0])))

enum {
    ITEM_CHANGE_CONTRAST,
    ITEM_PASS,
    ITEM_FAIL
};

typedef enum {
    Image_Red = 0,
    Image_Green,
    Image_Blue,
    Image_White,
    Image_Black,
    Image_Num
};


static item_t backlight_item[] = {
    item(ITEM_CHANGE_CONTRAST, uistr_backlight_level),
    item(ITEM_PASS,             uistr_pass),
    item(ITEM_FAIL,             uistr_fail),
    item(-1, NULL),
};

static item_t backlight_auto_item[] = {
    item(-1, NULL),
};


unsigned int backlight_change_contrast = 0;
unsigned int backlight_test_exit = 0;
int backlight_thread_exit = false;
int backlight_thread_start = false;

static pthread_t backlight_thread;
pthread_cond_t at_cmd_ready;
pthread_mutex_t locker;

typedef struct {
	struct ftm_module *module;

    /* image view */
    struct imageview img_view;
    char img_title[128];
    char img_filename[128];
    text_t img_title_ctext;
    unsigned int img_index;

    /* item view */
	struct itemview *itm_view;
    text_t itm_title_ctext;
} backlight_module;


static int
write_int(char const* path, int value)
{
    int fd;
    
    if (path == NULL)
        return -1;
    
    fd = open(path, O_RDWR);
    if (fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", value);
        int amt = write(fd, buffer, bytes);
        close(fd);
        return amt == -1 ? -errno : 0;
    }
    
    LOGE("write_int failed to open %s\n", path);
    return -errno;
}

static void update_color_screen(void)
{
    unsigned int test_img_index = Image_White;

    switch (test_img_index)
    {
        case Image_Red:
            ui_color(255, 0, 0, 255);
            break;
        case Image_Green:
            ui_color(0, 255, 0, 255);
            break;
        case Image_Blue:
            ui_color(0, 0, 255, 255);
            break;
        case Image_White:
            ui_color(255, 255, 255, 255);
            break;
        case Image_Black:
            ui_color(0, 0, 0, 255);
            break;
        default:
            ui_color(0, 0, 0, 255);
            break;
    }
    
    ui_fill(0, 0, ui_fb_width(), ui_fb_height());
    ui_flip();
}

static void update_contrast(backlight_module *backlight)
{
    write_int(BRIGHTNESS_FILE, brightness_seq[seq_index]);
    if (brightness_seq[seq_index] == -1)
			seq_index = 0;
}

static void *update_backlight_thread(void *priv)
{
    backlight_module *backlight = (backlight_module *)priv;
    int index = 0;

    LOGD("%s: Start\n", __FUNCTION__);
    backlight_thread_start = true;
    backlight_thread_exit = false;
            		
    update_color_screen();

    do 
    {
        if (backlight_test_exit)
        {
            break;
        }
        pthread_mutex_lock(&locker);
        pthread_cond_wait(&at_cmd_ready, &locker);  
	    update_contrast(backlight);
        seq_index++;
        pthread_mutex_unlock(&locker);  
        sleep(1);
    } while (1);

    backlight_thread_start = true;
    backlight_thread_exit = false;
    LOGD("%s: Exit\n", __FUNCTION__);

    pthread_exit(NULL);

    return NULL;
}


static void auto_change_contrast(backlight_module *backlight, void *priv)
{
    struct itemview *iv = backlight->itm_view;

    iv->start_menu(iv, 0);
    iv->redraw(iv);

    pthread_create(&backlight_thread, NULL, update_backlight_thread, priv);        
}

/*
static void change_contrast(void)
{
    write_int(BRIGHTNESS_FILE, brightness_seq[seq_index]);
    if (brightness_seq[++seq_index] == -1)
        seq_index = 0;
}
*/

static void change_contrast(int key, void *priv)
{
	backlight_module *lcd = (backlight_module *)priv;
	struct imageview *iv = &lcd->img_view;

	int blk_show_image_flag = 0;
	bool exit = false;

	switch (key)
    {
    case UI_KEY_UP:
    case UI_KEY_VOLUP:
		exit = true;
		break;
    case UI_KEY_DOWN:
    case UI_KEY_VOLDOWN:
   		write_int(BRIGHTNESS_FILE, brightness_seq[seq_index]);
		if (brightness_seq[++seq_index] == -1)
			seq_index = 0;
        break;

    case UI_KEY_BACK:
	case UI_KEY_CONFIRM:
        exit = true;
        break;

    default:
        break;
    }
	if((blk_show_image_flag == 0) && (!exit)) {			
		ui_color(255, 255, 255, 255);
		ui_fill(0, 0, ui_fb_width(), ui_fb_height());
		ui_flip();				
	    blk_show_image_flag = 1;
    }
	if (exit) {
        iv->exit(iv);
	}

}


static void init_main_menu(struct ftm_param *param, backlight_module *backlight)
{
    struct itemview *iv = backlight->itm_view;

    init_text(&backlight->itm_title_ctext, param->name, COLOR_YELLOW);

    iv->set_title(iv, &backlight->itm_title_ctext);
    
    if(get_is_ata())
    {
        iv->set_items(iv, backlight_auto_item, NULL);
    }
    else
    {
        iv->set_items(iv, backlight_item, ITEM_CHANGE_CONTRAST);
    }
}

//need this func to show white image for BLK test
static void blk_update_imageview(backlight_module *lcd)
{
	struct imageview *iv = &lcd->img_view;
	lcd->img_index = lcd->img_index + 1;
    unsigned int i = lcd->img_index;
	
    LOGD(TAG "blk_update_imageview(%d)\n", i);

    init_text(&lcd->img_title_ctext, lcd->img_title, COLOR_YELLOW);

    iv->set_title(iv, NULL);
}

static void blk_test_images(backlight_module *lcd)
{
	struct imageview *iv = &lcd->img_view;

    ui_init_imageview(iv, change_contrast, lcd);

    /* reset all the img index */
   // test_img_index = 0;
    lcd->img_index = 0;

    blk_update_imageview(lcd);
    iv->run(iv);
}

static int backlight_entry(struct ftm_param *param, void *priv)
{
    backlight_module *backlight = (backlight_module *)priv;

    bool exit = false;
    seq_index = 0;

    init_main_menu(param, backlight);
    if(0 == get_is_ata())
    {
        //update_color_screen();
        while(!exit)
        {
            switch(backlight->itm_view->run(backlight->itm_view, NULL))
            {
            case ITEM_CHANGE_CONTRAST:
                blk_test_images(backlight);//show white image and test BLK level
                init_main_menu(param, backlight);
                exit = false;
                break;
            case ITEM_PASS:
                backlight->module->test_result = FTM_TEST_PASS;
                exit = true;
                break;
            case ITEM_FAIL:
                backlight->module->test_result = FTM_TEST_FAIL;
                exit = true;
                break;
            case -1:
                exit = true;
                break;
            default:
                break;
            }
        }
    }
    else
    {
        backlight_test_exit = 0;
        auto_change_contrast(backlight, priv);
        pthread_join(backlight_thread, NULL);
        seq_index = 0;        
    }
    write_int(BRIGHTNESS_FILE, 255);

	return 0;
}


int backlight_init(void)
{
    int r;
    struct ftm_module *mod;
    backlight_module *backlight;
    
    write_int(BRIGHTNESS_FILE, 255);
    
    mod = ftm_alloc(ITEM_BACKLIGHT, sizeof(backlight_module));
    if (!mod)
        return -ENOMEM;
    
    backlight = mod_to_backlight(mod);
    
    backlight->module = mod;
    backlight->itm_view = ui_new_itemview();
    if (!backlight->itm_view)
        return -ENOMEM;
    
    r = ftm_register(mod, backlight_entry, (void*)backlight);
    if (r) {
        LOGD(TAG "register Backlight failed (%d)\n", r);
        return r;
    }
    
    return 0;
}

#endif
