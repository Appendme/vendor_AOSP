#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <utils/Log.h>
#include <cutils/log.h>
#include <netutils/ifc.h>
#if defined(MTK_THERMAL_PA_VIA_ATCMD)
#include <cutils/sockets.h>
#include <assert.h>
#endif

#define THERMAL_MD_TP
#define MD_UL_DR_THROTTLE

#define MD_3G_UL_THROTTLE


#if defined(MTK_THERMAL_PA_VIA_ATCMD)
#define MD_TOGGLE_SUPPORT
#if defined(MTK_C2K_SUPPORT)
#define MD_TOGGLE_SUPPORT_C2K
#endif
#endif

#ifdef MD_TOGGLE_SUPPORT_C2K
#include <dlfcn.h>
#endif

#if defined(THERMAL_MD_TP)
#include <sys/time.h>
#endif

#if defined(MD_UL_DR_THROTTLE)
#include <time.h>
#endif

#if defined(MTK_THERMAL_PA_VIA_ATCMD)
#define uint8 unsigned char
#define uint32 unsigned int
#endif

static int debug_on = 0;

#define TM_LOG_TAG "thermal_repeater"
#define TM_DBG_LOG(_fmt_, args...) \
    do { \
        if (1 == debug_on) { \
            LOG_PRI(ANDROID_LOG_INFO, TM_LOG_TAG, _fmt_, ##args); \
        } \
    } while(0)

#define TM_INFO_LOG(_fmt_, args...) \
    do { LOG_PRI(ANDROID_LOG_INFO, TM_LOG_TAG, _fmt_, ##args); } while(0)

#define ONE_MBITS_PER_SEC 1000
#define PROCFS_TM_PID "/proc/driver/thermal/clwmt_pid"

#ifdef MD_TOGGLE_SUPPORT
#define PROCFS_CLMUTT_TM_PID "/proc/driver/thermal/clmutt_tm_pid"
#endif

#define COOLER_NUM 3
#if defined(MD_UL_DR_THROTTLE)
#define PROCFS_MD_UL_TM_PID "/proc/amddulthro/tm_pid"
#endif


#if defined(MD_3G_UL_THROTTLE)
#define PROCFS_3GMD_UL_TM_PID "/proc/driver/thermal/tm_3Gpid"
static int md_suspend=0;
static int md_active=0;
static int md_active_timer=0;
static int md_timer2=0;
#define CCMNI_MAX_NUM (3)
#define CCMNI_CALLER_THERMAL (1<<9)
#include <hardware/ccci_intf.h>
#include <netinet/in.h>
#include <linux/if.h>
#endif


#define WLAN_IFC_PATH "/sys/class/net/wlan0/operstate"
#define AP_IFC_PATH "/sys/class/net/ap0/operstate"
#define P2P_IFC_PATH "/sys/class/net/p2p0/operstate"
#if defined(MD_UL_DR_THROTTLE)
#define CCEMNI0_IFC_PATH "/sys/class/net/ccemni0/operstate"
#define CCEMNI1_IFC_PATH "/sys/class/net/ccemni1/operstate"
#define CCEMNI2_IFC_PATH "/sys/class/net/ccemni2/operstate"
#define CCMNI_IFC_PATH "/sys/class/net/ccmni/operstate"
#endif

enum {
	WLAN_IFC = 0,
	AP_IFC = 1,
	P2P_IFC = 2,

	IFC_NUM /*Last one*/
};

#if defined(MD_UL_DR_THROTTLE)
enum {
	CCEMNI0 = 0,
    CCEMNI1 = 1,
    CCEMNI2 = 2,
    CCMNI = 3,

	MD_IFC_NUM /*Last one*/
};
#endif

static char IFC_NAME[IFC_NUM][17] = {
      "wlan0"
    , "ap0"
    , "p2p0"
    };

#if defined(MD_UL_DR_THROTTLE)
static char MD_IFC_NAME[MD_IFC_NUM][17] = {
      "ccemni0"
    , "ccemni1"
    , "ccemni2"
    , "ccmni"
    };
#endif

static char IFC_PATH[IFC_NUM][50] = {
      WLAN_IFC_PATH
    , AP_IFC_PATH
    , P2P_IFC_PATH
    };

#if defined(MD_UL_DR_THROTTLE)
static char MD_IFC_PATH[MD_IFC_NUM][50] = {
      CCEMNI0_IFC_PATH
    , CCEMNI1_IFC_PATH
    , CCEMNI2_IFC_PATH
    , CCMNI_IFC_PATH
    };
#endif


#ifdef MD_TOGGLE_SUPPORT
/*========  Prototype Declaration ========= */
static int set_md_toggle_throttle(int);

/*========  Prototype Declaration ========= */
#endif

#ifdef MD_TOGGLE_SUPPORT_C2K
#define RPC_RIL_PATH "/system/lib64/librpcril.so"
#define RIL_REQUEST_SET_MODEM_THERMAL 2832

int *mHandle = NULL;  //for rpc
int (*sendRpcRequest)(int, int) = NULL;

void RilRPC_release(void)
{
	TM_INFO_LOG("%s \n", __FUNCTION__);

	if (mHandle != NULL)
	{
		dlclose(mHandle);
		mHandle = NULL;
	}

	sendRpcRequest = NULL;
}

void RilRPC_init(void)
{
	TM_INFO_LOG("%s \n", __FUNCTION__);

	mHandle = (int*)dlopen(RPC_RIL_PATH, RTLD_NOW);
	if (mHandle == NULL)
	{
		TM_INFO_LOG("%s dlopen fail: %s \n", __FUNCTION__, dlerror());
		return;
	}

	sendRpcRequest = (int (*)(int, int))dlsym(mHandle, "sendRpcRequest");
	if (NULL == sendRpcRequest)
	{
		TM_INFO_LOG("%s can not find function sendRpcRequest \n", __FUNCTION__);
		RilRPC_release();
		return;
	}

	TM_INFO_LOG("%s end \n", __FUNCTION__);
}

int RilRPC_send(int v1, int v2)
{
	TM_INFO_LOG("%s v1= %d, v2 = %d \n", __FUNCTION__, v1, v2);

    int ret = 1;

    if(sendRpcRequest)
        ret= sendRpcRequest(v1,v2);

    return ret;
}
#endif

#ifdef NEVER
static char THROTTLE_SCRIPT_PATH[] = "/system/etc/throttle.sh";

static void exe_cmd(int wifi_ifc, int level)
{
	if (0 == access(THROTTLE_SCRIPT_PATH, R_OK | X_OK) && wifi_ifc >= 0) {
		char cmd[256] = {0};

		sprintf(cmd, "%s %s %d %d", THROTTLE_SCRIPT_PATH, IFC_NAME[wifi_ifc], level * ONE_MBITS_PER_SEC, level * ONE_MBITS_PER_SEC);

		TM_INFO_LOG("cmd=%s", cmd);

		/*Need to execute twice to effect the command*/
		int ret = system(cmd);
		if ((-1 == ret) || (0 != WEXITSTATUS(ret))) {
			TM_INFO_LOG("1. executing %s failed: %s", THROTTLE_SCRIPT_PATH, strerror(errno));
		}

		ret = system(cmd);
		if ((-1 == ret) || (0 != WEXITSTATUS(ret))) {
			TM_INFO_LOG("2. executing %s failed: %s", THROTTLE_SCRIPT_PATH, strerror(errno));
		}
	} else {
		TM_INFO_LOG("failed to access %s", THROTTLE_SCRIPT_PATH);
	}
}
#endif /* NEVER */

static void set_wifi_throttle(int level)
{
	int i = 0;
	for ( i=0; i<IFC_NUM; i++) {
		TM_DBG_LOG("checking %s", IFC_PATH[i]);
		if (0 == access(IFC_PATH[i], R_OK)) {
			char buf[80];
			int fd = open(IFC_PATH[i], O_RDONLY);
			if (fd < 0) {
				TM_INFO_LOG("Can't open %s: %s", IFC_PATH[i], strerror(errno));
				continue;
			}

			int len = read(fd, buf, sizeof(buf) - 1);
			if (len < 0) {
				TM_INFO_LOG("Can't read %s: %s", IFC_PATH[i], strerror(errno));
                close(fd);
				continue;
			}
			close(fd);
			if(!strncmp (buf, "up", 2)) {
				//ifc_set_throttle(IFC_NAME[i], level * ONE_MBITS_PER_SEC, level * ONE_MBITS_PER_SEC);
				ifc_set_throttle(IFC_NAME[i], level, level);    //[star] unit: Kbytes

				#ifdef NEVER
			 	exe_cmd(i, level);
				#endif /* NEVER */
			} else
				TM_DBG_LOG("%s is down!", IFC_NAME[i]);
		}
	}
}

#if defined(MD_3G_UL_THROTTLE)

//stop or start uplink transfer API
/* md_id: MD_SYS1/MD_SYS2/MD_SYS3/MD_SYS5  */
/* index: ccmni interface index, 0/1/2...                       */
/* state: state[0]= 0, stop uplink transfer; state[0]=1, start uplink transfer;
                  State[9]=1, thermal caller; state[8]=1, Rild caller*/
void setUplinkTransferState(int md_id, int index, int state)
{
	struct ifreq ifr;
	char* name = "";
	memset(&ifr, 0, sizeof(ifr));
	//memset(&ifr, 0, sizeof(struct ifreq));
	name = ccci_get_node_name(USR_NET_0 + index, md_id);
	sprintf(ifr.ifr_name, "%s", name);
	ifc_set_txq_state(ifr.ifr_name, state);
	TM_DBG_LOG("setUplinkTransmitState: md_id=%d, ccmni_index=%d, name=%s, state =0x%x",
			md_id, index, ifr.ifr_name, state);
}



static void set_3Gmd_ul_throttle(int level)
{
	int i = 0;
	if(level==-1){//start to unlimit
		TM_DBG_LOG("set_3Gmd_ul_throttle <<unlimit>> level=%d\n", level);
		//start UL transfer of all ccmni interface
		for (i = 0; i < CCMNI_MAX_NUM; i++)
			setUplinkTransferState (MD_SYS1, i, (1| CCMNI_CALLER_THERMAL));
	}
	else{//start to do limit
		TM_DBG_LOG("set_3Gmd_ul_throttle ++limit++ level=%d\n", level);
		//stop UL transfer of all ccmni interface
		for (i = 0; i < CCMNI_MAX_NUM; i++)
		setUplinkTransferState (MD_SYS1, i, (0| CCMNI_CALLER_THERMAL));
	}
}


static timer_t base_timer_3Gmd;
static timer_t level_timer_3Gmd;
static int level_timeout_3Gmd = 10;

void timer_thread_3Gmd(union sigval v)
{

    // restart base_timer
    if(md_active_timer==1)//active timer already start, start suspend here
    {
    	md_active_timer=0;//activer timer already start

        // unlimit
	    set_3Gmd_ul_throttle(1);

        struct itimerspec it;
#if 0
		it.it_interval.tv_sec = md_suspend;//15sec
		it.it_interval.tv_nsec =0;//md_suspend*1000 000000;//15sec;
		it.it_value.tv_sec = md_suspend;//15sec
		it.it_value.tv_nsec = 0;//md_suspend*1000000000;//15sec;

#else
		if (md_suspend >= 1001)
		{
			it.it_interval.tv_sec = 0xFFFF;//md_suspend
			it.it_interval.tv_nsec =0;
			it.it_value.tv_sec = 0xFFFF;//md_suspend;//15sec
			it.it_value.tv_nsec =0;
		}
		else
		{
			it.it_interval.tv_sec = 0;
			it.it_interval.tv_nsec =md_suspend*1000000;//100 000000=>100ms
			it.it_value.tv_sec = 0;
			it.it_value.tv_nsec = md_suspend*1000000;//100 000000=>100ms
		}
        //TM_INFO_LOG("1 it.it_value.tv_nsec=0x%x\n",it.it_value.tv_nsec);

#endif
        if (timer_settime(base_timer_3Gmd, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
        }
    }
    else
	{
		md_active_timer=1;//start activer timer first

        // unlimit
	    set_3Gmd_ul_throttle(-1);


        struct itimerspec it;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_nsec = md_active*1000000;//100 000000=>100ms
        it.it_value.tv_sec = 0;
        it.it_value.tv_nsec = md_active*1000000;//100 000000=>100ms
        //TM_INFO_LOG("2 it.it_value.tv_nsec=0x%x\n",it.it_value.tv_nsec);
        if (timer_settime(base_timer_3Gmd, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
        }
    }
}


static int reset_timers_3Gmd(int active, int suspend)
{
    TM_DBG_LOG("reset_timers_3Gmd,active=0x%x,suspend=0x%x\n",active,suspend);

    // unlimit
    set_3Gmd_ul_throttle(-1);
	TM_INFO_LOG("reset_timers_3Gmd unlimit\n");
    // restart base_timer
    {
        struct itimerspec it;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_nsec = active*1000000;
        it.it_value.tv_sec = 0;
        it.it_value.tv_nsec = active*1000000;//200 000000=>200ms

        if (timer_settime(base_timer_3Gmd, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
            return -1;
        }
        md_active_timer=1;//start activer timer first
    }

    return 0;
}

static int stop_timers_3Gmd(void)
{
    TM_DBG_LOG("stop_timers\n");

    // restart base_timer
    {
        struct itimerspec it;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_nsec = 0;
        it.it_value.tv_sec = 0;
        it.it_value.tv_nsec = 0;

        if (timer_settime(base_timer_3Gmd, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
            return -1;
        }
    }

    return 0;
}

static int thermal_create_timers_3Gmd(void)
{
    struct sigevent evp;

    TM_DBG_LOG("thermal_create_timers_3Gmd\n");

    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_int = 111;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timer_thread_3Gmd;

    if (timer_create(CLOCK_REALTIME, &evp, &base_timer_3Gmd) == -1)
    {
        TM_INFO_LOG("fail to create base_timer\n");
        return -1;
    }

    return 0;
}


static void set_md_3G_throttle(int active,int suspend)
{
    TM_DBG_LOG("set_md_3G_throttle %d, %d\n", active, suspend);

    if (active == 0 || suspend == 0 )
    {
    	set_3Gmd_ul_throttle(-1);
        stop_timers_3Gmd();
    }
    else
    {
        reset_timers_3Gmd(active,suspend);
    }
}
#endif /* MD_3G_UL_THROTTLE */



#if defined(MD_UL_DR_THROTTLE)
static void set_md_ul_throttle(int level)
{
	int i = 0;

	TM_DBG_LOG("set_md_ul_throttle %d\n", level);

	for ( i=0; i<MD_IFC_NUM; i++) {
		TM_DBG_LOG("checking %s", MD_IFC_PATH[i]);
		if (0 == access(MD_IFC_PATH[i], R_OK)) {
			char buf[80];
			int fd = open(MD_IFC_PATH[i], O_RDONLY);
			if (fd < 0) {
				TM_INFO_LOG("Can't open %s: %s", MD_IFC_PATH[i], strerror(errno));
				continue;
			}

			int len = read(fd, buf, sizeof(buf) - 1);
			if (len < 0) {
				TM_INFO_LOG("Can't read %s: %s", MD_IFC_PATH[i], strerror(errno));
				close(fd);
				continue;
			}
			close(fd);
#if 0
			if(!strncmp (buf, "up", 2)) {
				ifc_set_throttle(MD_IFC_NAME[i], -1, level);

				#ifdef NEVER
			 	exe_cmd(i, level);
				#endif /* NEVER */
			} else
				TM_DBG_LOG("%s is down!", MD_IFC_NAME[i]);
#else
			if(!strncmp (buf, "down", 4)) {
			    TM_DBG_LOG("%s is down!", MD_IFC_NAME[i]);
			} else {
                ifc_set_throttle(MD_IFC_NAME[i], -1, level);

				#ifdef NEVER
			 	exe_cmd(i, level);
				#endif /* NEVER */
			}
#endif
		}
	}
}

static timer_t base_timer;
static timer_t level_timer;
static int level_timeout = 10;

void timer_thread(union sigval v)
{
    TM_DBG_LOG("timer_thread function! %d\n", v.sival_int);

    if (v.sival_int == 111)
    {
        // unlimit
        set_md_ul_throttle(-1);

        // restart base_timer
        {
            struct itimerspec it;
            it.it_interval.tv_sec = 10;
            it.it_interval.tv_nsec = 0;
            it.it_value.tv_sec = 10;
            it.it_value.tv_nsec = 0;

            if (timer_settime(base_timer, 0, &it, NULL) == -1)
            {
                TM_INFO_LOG("fail to timer_settime base_timer\n");
            }
        }
        // restart level_timer
        {
            struct itimerspec it;
            it.it_interval.tv_sec = 10;
            it.it_interval.tv_nsec = 0;
            it.it_value.tv_sec = level_timeout;
            it.it_value.tv_nsec = 0;

            if (timer_settime(level_timer, 0, &it, NULL) == -1)
            {
                TM_INFO_LOG("fail to timer_settime base_timer\n");
            }
        }
    }
    else if (v.sival_int == 222)
    {
        // limit
        set_md_ul_throttle(1);
    }
}

static int reset_timers(void)
{
    TM_DBG_LOG("reset_timers\n");

    // unlimit
    set_md_ul_throttle(-1);

    // restart base_timer
    {
        struct itimerspec it;
        it.it_interval.tv_sec = 10;
        it.it_interval.tv_nsec = 0;
        it.it_value.tv_sec = 10;
        it.it_value.tv_nsec = 0;

        if (timer_settime(base_timer, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
            return -1;
        }
    }
    // restart level_timer
    {
        struct itimerspec it;
        it.it_interval.tv_sec = 10;
        it.it_interval.tv_nsec = 0;
        it.it_value.tv_sec = level_timeout;
        it.it_value.tv_nsec = 0;

        if (timer_settime(level_timer, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
            return -1;
        }
    }

    return 0;
}

static int stop_timers(void)
{
    TM_DBG_LOG("stop_timers\n");

    // restart base_timer
    {
        struct itimerspec it;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_nsec = 0;
        it.it_value.tv_sec = 0;
        it.it_value.tv_nsec = 0;

        if (timer_settime(base_timer, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
            return -1;
        }
    }
    // restart level_timer
    {
        struct itimerspec it;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_nsec = 0;
        it.it_value.tv_sec = 0;
        it.it_value.tv_nsec = 0;

        if (timer_settime(level_timer, 0, &it, NULL) == -1)
        {
            TM_INFO_LOG("fail to timer_settime base_timer\n");
            return -1;
        }
    }

    return 0;
}

static int thermal_create_timers(void)
{
    struct sigevent evp;

    TM_DBG_LOG("thermal_create_timers\n");

    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_int = 111;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timer_thread;

    if (timer_create(CLOCK_REALTIME, &evp, &base_timer) == -1)
    {
        TM_INFO_LOG("fail to create base_timer\n");
        return -1;
    }

    memset(&evp, 0, sizeof(struct sigevent));
    evp.sigev_value.sival_int = 222;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timer_thread;

    if (timer_create(CLOCK_REALTIME, &evp, &level_timer) == -1)
    {
        TM_INFO_LOG("fail to create level_timer\n");
        return -1;
    }

    return 0;
}

static void set_md_dul_throttle(int level) // level 1~10, 1: unlimit for 1s and zero for 9s, ..., 10: unlimit
{
    TM_DBG_LOG("set_md_dul_throttle %d\n", level);

    if (level >=1  && level < 10)
    {
        level_timeout = level;
        reset_timers();
    }
    else if (level == 10)
    {
        set_md_ul_throttle(-1);
        stop_timers();
    }
    else
        TM_INFO_LOG("invalid level %d, ignored\n", level);
    // if 10 set unlimit and stop timer

    // if timer not created, create timer

    // if timer created, reset timer

    // unlimit ul, and start timer for level s

    // when times out, limit ul to 1kbps, and start timer for 10 - level s
}
#endif

#ifdef MD_TOGGLE_SUPPORT
/*
 * use "si_errno" for client identify
 * for tm_pid (/system/bin/thermal)
 */
enum {
/*	TM_CLIENT_clwmt = 0,
	TM_CLIENT_mdulthro =1,
	TM_CLIENT_mddlthro =2,	*/
	TM_CLIENT_clmutt = 3
};
#endif
static void signal_handler(int signo, siginfo_t *si, void *uc)
{
	static int cur_thro = 0;
#if defined(MD_UL_DR_THROTTLE)
    static int md_cur_thro = 0;
    static int md_cur_dthro = 0; // discontinuous ul
#endif

#ifdef MD_TOGGLE_SUPPORT
	static int md_cur_toggle_thro;
#endif

#if defined(MD_3G_UL_THROTTLE)
	int set_thro1=0;
	int set_thro2=0;
#endif

    TM_INFO_LOG("signal_handler signo=%d  si->si_errno=%d, si->si_code=0x%x\n", signo , si->si_errno , si->si_code);

	int set_thro = si->si_code;
	int err = si->si_errno;

	switch(si->si_signo) {
		case SIGIO:
			if (cur_thro != set_thro && err == 0) {
                TM_DBG_LOG("set_wifi_throttle cur=%d set=%d\n", cur_thro, set_thro);
				//set_thro = set_thro?:1; /*If set_thro is 0, set 1Mb/s*/
				//set_thro = set_thro?:1000; //[star] 1KBytes
				set_thro = ((set_thro>=0)&&(set_thro<1000))?1000:set_thro; //1Kbps
				set_wifi_throttle(set_thro);
				cur_thro = set_thro;
			}

#if defined(MD_UL_DR_THROTTLE)
			if (err == 1 && md_cur_thro != set_thro) {
                TM_DBG_LOG("set_md_ul_throttle cur=%d set=%d\n", md_cur_thro, set_thro);
			    set_thro = set_thro?:1; /*If set_thro is 0, set 1kb/s*/
			    set_md_ul_throttle(set_thro);
			    md_cur_thro = set_thro;
			}

			if (err == 2 && md_cur_dthro != set_thro) {
                TM_DBG_LOG("set_md_dul_throttle cur=%d set=%d\n", md_cur_dthro, set_thro);
			    set_md_dul_throttle(set_thro);
			    md_cur_dthro = set_thro;
			}
#endif

#ifdef MD_TOGGLE_SUPPORT
		if (err == TM_CLIENT_clmutt && md_cur_toggle_thro != set_thro) {
			TM_INFO_LOG("set_md_toggle_throttle cur=%d set=%d\n", md_cur_toggle_thro, set_thro);
			if (0 == set_md_toggle_throttle(set_thro))  /*If set_thro is 1, set MD off*/
				md_cur_toggle_thro = set_thro;
		}
#endif

#if defined(MD_3G_UL_THROTTLE)
            if (err == 4 ) {
	            set_thro1 = (set_thro & 0x000000FF);
	            set_thro2 = (set_thro & 0x0000FF00) >> 8;
                TM_DBG_LOG("set_thro1 = 0x%x, set_thro2 = 0x%x\n", set_thro1, set_thro2);

				md_active  = ((set_thro1 & 0xF0) >> 4)*100+((set_thro1 & 0x0F))*10;
				md_suspend = ((set_thro2 & 0xF0) >> 4)*100+((set_thro2 & 0x0F))*10;


                TM_INFO_LOG("md_active = 0x%x, md_suspend = 0x%x\n", md_active, md_suspend);


				//0x1040=>md_suspend 0x10, md_active=0x40
				//0x2010=>md_suspend 0x20, md_active=0x10
				//0x4010=>md_suspend 0x40, md_active=0x10
				//0xf010=>md_suspend 0xf0, md_active=0x10


			    set_md_3G_throttle(md_active,md_suspend);
			}

#endif


		break;
		default:
			TM_INFO_LOG("what!!!\n");
		break;
	}
}

#if defined(MTK_THERMAL_PA_VIA_ATCMD)

static void handle_pipe(int sig) {
    TM_INFO_LOG("sigpipe test recv sig = %d\n", sig);
    // handle pipe break signal
}

#if defined(THERMAL_MD_TP)
static int setMdTpThreshold(int sock, int slotId, int sensorType, int threshold)
{
    int ret = -1, strLen = 0, count = 1;
	char *strParm = NULL, *pTmp = NULL;
	char temp[64] = {0};

	assert(sock > 0);

	sprintf(temp, "THERMAL,%d,0,%d,%d\n", slotId, sensorType, threshold);
	strLen = strlen(temp) + 1;
	strParm	= (char *)malloc(strLen);
	strParm[strLen - 1] = '\0';
	strncpy(strParm, temp, strLen);
	TM_DBG_LOG("%d %s will sent to rild\n", strLen, strParm);

	ret = send(sock, (int)&count, sizeof(int), 0);
	if(sizeof(int) == ret)
		ret = send(sock, &strLen, sizeof(strLen), 0);
	else
	{
		ret = -4;
		goto failed;
	}

	if (sizeof(strLen) == ret)
	{
		ret = send(sock, strParm, strLen, 0);
		if (strLen == ret)
		{
			ret = 0;
			TM_DBG_LOG("%s ok\n", __FUNCTION__);
		}
		else
		{
			ret = -5;
			goto failed;
		}
	}
	else
	{
		ret = -3;
	}

failed:
	free(strParm);
	TM_INFO_LOG("oh, %s (%d)%s\n", __FUNCTION__, ret, strerror(errno));

	return ret;
}
#endif

#ifdef MD_TOGGLE_SUPPORT
static int RildConnect(void)
{
	int sock = -1;
	sock = socket_local_client("rild-oem", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
	if(sock < 0)
		TM_DBG_LOG("RildConnect %s\n", strerror(errno));

	return sock;
}

static int RildDisconnect(int sock)
{
	int ret;

	assert(sock > 0);
	ret = close(sock);

	return ret;
}


static int MdToggle(int sock, int state)
{
	int ret = -1, strLen = 0, count = 1;
	char *strParm = NULL;
	char temp[32] = {0};

	assert(sock > 0);

	sprintf(temp, "MDTM_TOG,%d\n", state);
	strLen = strlen(temp) + 1;
	strParm = (char *)malloc(strLen);
	strParm[strLen - 1] = '\0';
	strncpy(strParm, temp, strLen);
	TM_DBG_LOG("%d %s will sent to rild\n", strLen, strParm);

	ret = send(sock, (const void *)&count, sizeof(int), 0);
	if(sizeof(int) == ret)
		ret = send(sock, (const void *)&strLen, sizeof(strLen), 0);
	else
	{
		ret = -4;
		goto failed;
	}

	if (sizeof(strLen) == ret)
	{
		ret = send(sock, strParm, strLen, 0);
		if (strLen == ret)
		{
			ret = 0;
			TM_DBG_LOG("%s ok\n", __FUNCTION__);
		}
		else
		{
			ret = -5;
			goto failed;
		}
	}
	else
	{
		ret = -3;
	}

failed:
	free(strParm);
	TM_INFO_LOG("oh, %s (%d)%s\n", __FUNCTION__, ret, strerror(errno));

	return ret;
}

/* throttle level 0: MD on; throttle level 1: MD off */
static int set_md_toggle_throttle(int level)
{
	int ret = -1;
	int socket;
	static int state;

	/* MD state is reversed throttle state  */
	if (0 == level)
		state = 1;
	else
		state = 0;


	{
		socket = RildConnect();
		TM_DBG_LOG("socket %d\n", socket);
		while (socket == -1) {
				sleep(5);
				socket = RildConnect();
				TM_DBG_LOG("socket %d\n", socket);
		}
	}

#ifdef MD_TOGGLE_SUPPORT_C2K
	ret = RilRPC_send(RIL_REQUEST_SET_MODEM_THERMAL, state);

	if (ret != 0) {
		TM_INFO_LOG("%s: RilRPC_send ret =%d \n", __FUNCTION__, ret);

		RildDisconnect(socket);
		return ret;
	}
#endif

	ret = MdToggle(socket, state);

	TM_DBG_LOG("%s state: %d ret: %d \n", __FUNCTION__, level, ret);

	RildDisconnect(socket);

	return ret;
}

#endif

static int queryMdThermalInfo(int sock, int slotId, int opcode)
{
	int ret = -1, strLen = 0, count = 1;
	char *strParm = NULL, *pTmp = NULL;
	char temp[32] = {0};

	assert(sock > 0);

	//strLen = strlen("THERMAL") + 5;
	//strParm	= (char *)malloc(strLen);
	//strParm[strLen - 1] = '\0';
	//strcpy(strParm, "THERMAL");
	//strcat(strParm, ",");
	sprintf(temp, "THERMAL,%d,%d\n", slotId, opcode);
	strLen = strlen(temp) + 1;
	strParm	= (char *)malloc(strLen);
	strParm[strLen - 1] = '\0';
	strncpy(strParm, temp, strLen);
	TM_DBG_LOG("%d %s will sent to rild\n", strLen, strParm);

	ret = send(sock, (const void *)&count, sizeof(int), 0);
	if(sizeof(int) == ret)
		ret = send(sock, (const void *)&strLen, sizeof(strLen), 0);
	else
	{
		ret = -4;
		goto failed;
	}

	if (sizeof(strLen) == ret)
	{
		ret = send(sock, strParm, strLen, 0);
		if (strLen == ret)
		{
			ret = 0;
			TM_DBG_LOG("%s ok\n", __FUNCTION__);
		}
		else
		{
			ret = -5;
			goto failed;
		}
	}
	else
	{
		ret = -3;
	}

failed:
	free(strParm);
	TM_INFO_LOG("oh, %s (%d)%s\n", __FUNCTION__, ret, strerror(errno));

	return ret;
}

#define SOCKET_RECV_LEN (128)

static int recvMdThermalInfo(int sock, int slotId, int opcode)
{
	int ret = -1, strLen = SOCKET_RECV_LEN-1;
	uint8 strParm[SOCKET_RECV_LEN] = {0};

	assert(sock > 0);

	//ret = recv(sock, (uint8 *)&strLen, sizeof(strLen), 0);
    //TM_INFO_LOG("[%s] ret=%d, strLen=%d\n", __FUNCTION__, ret, strLen);
    TM_DBG_LOG("[%s]\n", __FUNCTION__);

	//if (sizeof(strLen) == ret)
	{
		//strParm = (uint8 *)malloc(strLen + 1);
		memset(strParm, '\0', strLen+1);
		strParm[SOCKET_RECV_LEN-1] = '\0';
		ret = recv(sock, strParm, strLen, 0);
		TM_INFO_LOG("[%s] ret=%d, strLen=%d, %s\n", __FUNCTION__, ret, strLen, strParm);

		if (0 < ret)
		{
		    if (strncmp("ERROR", (const char *)strParm, 5) != 0)
		    {
#if defined(THERMAL_MD_TP)
                if (strncmp("URC", (const char *)strParm, 3) == 0)
                {
                    // handle URC format
                    int sensorType = -1, temp = -127, opval0 = 0, opval1 = 0;
                    int tok;
    		        tok = sscanf((const char *)strParm, "URC,%d,%d", &sensorType, &temp);

    		        if (tok == 2)
                    {
                        // handle URC format
                        int mdinfoex_idx = -1;

                        TM_INFO_LOG("[%s] sensor=%d temp=%d\n", strParm, sensorType, temp);

                        switch (sensorType)
                        {
                        case 0: // DRAM MD
                            mdinfoex_idx = 1;
                            break;
                        case 1: // NTC MD
                            mdinfoex_idx = 0;
                            break;
                        case 2: // NTC PA not available for now
                            mdinfoex_idx = 2;
                            break;
                        default:
                            break;
                        }

                        if (mdinfoex_idx > -1)
                        {
                            int fd = open("/proc/driver/thermal/mdm_mdinfoex", O_RDWR);
                            char mdinfo_string[SOCKET_RECV_LEN] = {0};
                            if (fd >= 0){
	                            sprintf(mdinfo_string, "%d,%d", mdinfoex_idx, temp);
	                            ret = write(fd, mdinfo_string, sizeof(char) * strlen(mdinfo_string));
	                    		if (ret <= 0)	{
	                    			TM_DBG_LOG("Fail to write %s to %s %x\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex", ret);
	                    		} else {
	                    			TM_DBG_LOG("Success to write %s to %s\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex");
	                    		}
	                    		close(fd);
                            }
                        }

                        return 0;
                    }

                    return -1;
                }
#endif

		        if (opcode == -1)
		        {
    		        int md = 0, temp = 32767, tx = -127, opval0 = 0, opval1 = 0, opval2 = 0;
    		        int tok;
    		        tok = sscanf((const char *)strParm, "%d,%d,%d,%d,%d,%d", &md, &temp, &tx, &opval0, &opval1, &opval2);
    		        if (tok == 2) // tx not exist
    		        {
                        TM_DBG_LOG("[%s] tok=%d md=%d temp=%d tx=%d opval0=%d opval1=%d opval2=%d\n", strParm, tok, md, temp, tx, opval0, opval1, opval2);
                        tok = sscanf((const char *)strParm, "%d,%d,,%d,%d,%d", &md, &temp, &opval0, &opval1, &opval2);

                    }
                    TM_DBG_LOG("[%s] tok=%d md=%d temp=%d tx=%d opval0=%d opval1=%d opval2=%d\n", strParm, tok, md, temp, tx, opval0, opval1, opval2);

                    // write to proc
                    {
                        int fd = open("/proc/driver/thermal/mdm_mdinfo", O_RDWR);
                        char mdinfo_string[SOCKET_RECV_LEN] = {0};
                        if (fd >= 0){
	                        sprintf(mdinfo_string, "%d,%d,%d,%d", slotId, md, temp, ((tok>2)?tx:-127));
	                        ret = write(fd, mdinfo_string, sizeof(char) * strlen(mdinfo_string));
	                		if (ret <= 0)	{
	                			TM_DBG_LOG("Fail to write %s to %s %x\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfo", ret);
	                		} else {
	                			TM_DBG_LOG("Success to write %s to %s\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfo");
	                		}
	                		close(fd);
                        }
                    }
                    {
                        int fd = open("/proc/driver/thermal/mdm_mdinfoex", O_RDWR);
                        char mdinfo_string[SOCKET_RECV_LEN] = {0};
                        sprintf(mdinfo_string, "%d,%d", 0, opval0);
                        if (fd >= 0){
	                        ret = write(fd, mdinfo_string, sizeof(char) * strlen(mdinfo_string));
	                		if (ret <= 0)	{
	                			TM_DBG_LOG("Fail to write %s to %s %x\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex", ret);
	                		} else {
	                			TM_DBG_LOG("Success to write %s to %s\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex");
	                		}
	                		memset(mdinfo_string, 0x0, SOCKET_RECV_LEN);
	                		sprintf(mdinfo_string, "%d,%d", 1, opval1);
	                        ret = write(fd, mdinfo_string, sizeof(char) * strlen(mdinfo_string));
	                        if (ret <= 0)	{
	                			TM_DBG_LOG("Fail to write %s to %s %x\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex", ret);
	                		} else {
	                			TM_DBG_LOG("Success to write %s to %s\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex");
	                		}
	                		memset(mdinfo_string, 0x0, SOCKET_RECV_LEN);
	                		sprintf(mdinfo_string, "%d,%d", 2, opval2);
	                        ret = write(fd, mdinfo_string, sizeof(char) * strlen(mdinfo_string));
	                        if (ret <= 0)	{
	                			TM_DBG_LOG("Fail to write %s to %s %x\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex", ret);
	                		} else {
	                			TM_DBG_LOG("Success to write %s to %s\n", mdinfo_string, "/proc/driver/thermal/mdm_mdinfoex");
	                		}
	                		close(fd);
                        }
                    }
                }
#if 0
                else
                {
                    int temp;
    		        int tok;
    		        tok = sscanf((const char *)strParm, "%d", &temp);

                    TM_DBG_LOG("[%s] tok=%d op=%d temp=%d\n", strParm, tok, opcode, temp);

                    {
                        int fd = open("/proc/mtk_mdm_txpwr/mdinfoex", O_RDWR);
                        char mdinfo_string[64] = {0};
                        sprintf(mdinfo_string, "%d,%d", opcode, temp);
                        ret = write(fd, mdinfo_string, sizeof(char) * strlen(mdinfo_string));
                		if (ret <= 0)	{
                			TM_DBG_LOG("Fail to write %s to %s %x\n", mdinfo_string, "/proc/mtk_mdm_txpwr/mdinfoex", ret);
                		} else {
                			TM_DBG_LOG("Success to write %s to %s\n", mdinfo_string, "/proc/mtk_mdm_txpwr/mdinfoex");
                		}
                		close(fd);
                    }
                }
#endif
            }
            else
            {
                TM_DBG_LOG("[%s] ERROR\n", strParm);
                ret = -1;
            }

		}
		//free(strParm);
	}

	return ret;
}

static int connectToRild(int *solt_id)
{
	int sock = -1;
    sock = socket_local_client("rild-oem", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
    if(sock < 0)
        TM_DBG_LOG("connectToRild %s\n", strerror(errno));
#if 0
	char telephony_mode[] = "0", first_md[] = "0";

	property_get("ril.telephony.mode", telephony_mode, NULL);
	property_get("ril.first.md", first_md, NULL);
	wpa_printf(MSG_DEBUG, "RIL: slot=%d, ril.telephony.mode=%s, ril.first.md=%s",*solt_id , telephony_mode, first_md);
	if(telephony_mode[0]=='1' || telephony_mode[0]=='3')
	{
		sock = socket_local_client("rild-debug",
			ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
		wpa_printf(MSG_DEBUG, "RIL: try to connect to rild-debug");
	}
	else if(telephony_mode[0]=='2' || telephony_mode[0]=='4')
	{
		sock = socket_local_client("rild-debug-md2",
			ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
		if(sock < 0)
		{
			sock = socket_local_client("rild-debug", //6572,6582 is single modem project, only has MD1
				ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
			wpa_printf(MSG_DEBUG, "RIL: try to connect to rild-debug");
		}
		else
			wpa_printf(MSG_DEBUG, "RIL: try to connect to rild-debug-md2");
	}
	else if(telephony_mode[0]>='5' && telephony_mode[0]<='8')
	{
		if(first_md[0]-'1' == *solt_id) //ril.first.md==1 indicate MD1 connect to SIM1, ==2 indicate MD1 connect to SIM2
		{
			sock = socket_local_client("rild-debug",
				ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
			wpa_printf(MSG_DEBUG, "RIL: try to connect to rild-debug");
		}
		else
		{
			sock = socket_local_client("rild-debug-md2",
				ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
			wpa_printf(MSG_DEBUG, "RIL: try to connect to rild-debug-md2");
		}
		*solt_id = 0;
		wpa_printf(MSG_DEBUG, "RIL: Reset slot to slot0");
	}
	else
	{
		wpa_printf(MSG_DEBUG, "RIL: unsupport ril.telephony.mode, try to connect to default socket rild-debug");
		sock = socket_local_client("rild-debug",
			ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
	}
	if(sock < 0)
		wpa_printf(MSG_ERROR, "connectToRild %s", strerror(errno));
#endif
	return sock;
}

static int disconnectRild(int sock)
{
	int ret;

	assert(sock > 0);
	ret = close(sock);

	return ret;
}


#if defined(THERMAL_MD_TP)
static int new_md_tp_flow(void)
{
    int count = 0;
	int socket;
	int i = 0;
	static int thresholds[3] = { 85, 85, 85 };
	static int new_thresholds[3] = { 85, 85, 85 };

	sleep(60);

	signal(SIGPIPE, handle_pipe);

    // 1. get new configuration about new trip points and sensing period

    // 1.1 connect to ril ==> need to reopen every time
#if 0
    {
        socket = connectToRild(NULL);
        TM_INFO_LOG("socket %d\n", socket);
        while (socket == -1) {
            sleep(5);
            socket = connectToRild(NULL);
            TM_INFO_LOG("socket %d\n", socket);
        }
    }
#endif

#if 0
    // read thresholds
    {
        if (0 == access("/proc/mtk_mdm_txpwr/mdinfoex_thre", R_OK)) {
            char buf[80];
			int fd = open("/proc/mtk_mdm_txpwr/mdinfoex_thre", O_RDONLY);
			if (fd < 0) {
				TM_INFO_LOG("Can't open mdinfoex_thre: %s", strerror(errno));
			}

			int len = read(fd, buf, sizeof(buf) - 1);
			if (len < 0) {
				TM_INFO_LOG("Can't read mdinfoex_thre: %s", strerror(errno));
			}
			close(fd);
			if (3 == sscanf(buf, "%d,%d,%d,", &thresholds[1], &thresholds[0], &thresholds[2]))
			else
				TM_DBG_LOG("%s\n", buf);
        }
    }

    // 2. set threshold first, must check OK
    // TODO: fix this to read from configuration
    for (; i<3; i++)
    {
        {
            socket = connectToRild(NULL);
            TM_DBG_LOG("socket %d\n", socket);
            while (socket == -1) {
                sleep(5);
                socket = connectToRild(NULL);
                TM_DBG_LOG("socket %d\n", socket);
            }
        }
        setMdTpThreshold(socket, 0, i, thresholds[i]);

        if (0 > recvMdThermalInfo(socket, 0, 0))
            TM_DBG_LOG("set threshold failed %d\n", i);

        disconnectRild(socket);
    }
#endif

    // 3. enter infinite loop
    while (1)
    {
        // read thresholds
        {
            if (0 == access("/proc/driver/thermal/mdm_mdinfoex_thre", R_OK)) {
                char buf[80];
    			int fd = open("/proc/driver/thermal/mdm_mdinfoex_thre", O_RDONLY);
    			if (fd < 0) {
    				TM_INFO_LOG("Can't open mdinfoex_thre: %s", strerror(errno));
    			} else {
	                buf[79] = (char) 0x0;
	    			int len = read(fd, buf, sizeof(buf) - 1);
	    			if (len < 0) {
	    				TM_INFO_LOG("Can't read mdinfoex_thre: %s", strerror(errno));
	    			}
	    			close(fd);
	    			if (3 == sscanf(buf, "%d,%d,%d,", &new_thresholds[1], &new_thresholds[0], &new_thresholds[2]))
	    			    TM_DBG_LOG("new thresholds: %d,%d,%d\n", new_thresholds[0], new_thresholds[1], new_thresholds[2]);
	    			else
	    				TM_DBG_LOG("%s\n", buf);
                }
            }
        }

        // 2. set threshold first, must check OK
        for (i=0 ; i<3; i++)
        {
            if (new_thresholds[i] == thresholds[i]);
            else
            {

                thresholds[i] = new_thresholds[i];

                {
                    socket = connectToRild(NULL);
                    TM_DBG_LOG("socket %d\n", socket);
                    while (socket == -1) {
                        sleep(5);
                        socket = connectToRild(NULL);
                        TM_INFO_LOG("socket %d\n", socket);
                    }
                }
                setMdTpThreshold(socket, 0, i, thresholds[i]);

                if (0 > recvMdThermalInfo(socket, 0, 0))
                    TM_DBG_LOG("set threshold failed %d\n", i);

                disconnectRild(socket);
            }
        }

        count++;
        TM_DBG_LOG("count %d\n", count);

        {
            socket = connectToRild(NULL);
            TM_DBG_LOG("socket %d\n", socket);
            while (socket == -1) {
                sleep(5);
                socket = connectToRild(NULL);
                TM_INFO_LOG("socket %d\n", socket);
            }
        }

        // 4. query THERMAL status
        // For RF temp & all
        queryMdThermalInfo(socket, 0, -1);

        // 5. recv immediate response
        recvMdThermalInfo(socket, 0, -1);

#if 0   // only needs to close socket before send...
        disconnectRild(socket);

        // 6. select for interval
        {
            socket = connectToRild(NULL);
            TM_INFO_LOG("socket %d\n", socket);
            while (socket == -1) {
                sleep(5);
                socket = connectToRild(NULL);
                TM_INFO_LOG("socket %d\n", socket);
            }
        }
#endif

        {
            int ret = 0;
            fd_set rfds;
            struct timeval timeout;

            timeout.tv_sec = 5;
            timeout.tv_usec = 0;

            TM_DBG_LOG("waiting for URC, count %d timeout %d\n", count, timeout.tv_sec);

            FD_ZERO(&rfds);
            FD_SET(socket, &rfds);

            ret = select(socket + 1, &rfds, NULL, NULL, &timeout);
            if (ret < 0)
            {
                if (errno == EINTR) {
	                TM_INFO_LOG("Fail to select. error (%d)\n", errno);
                    sleep(5);
	                disconnectRild(socket);
	                continue;
                }
                return -1; // TODO: maybe need to redo allover again
            }

            if (ret == 0) // timesout
            {
                TM_INFO_LOG("select timeout\n");
                sleep(5);
                disconnectRild(socket);
                continue;
            }

            if (FD_ISSET(socket, &rfds))
            {
                recvMdThermalInfo(socket, 0, -1);
            }

            disconnectRild(socket);
        }
    }

    // 7. close // need to disconnect everytime
#if 0
    disconnectRild(socket);
#endif

    return 0;
}
#endif

#endif

int main(int argc, char *argv[])
{
	if(argc == 3) {
        // for manual testing
		char ifc[17] = {0};
		char tmp[17] = {0};
		int thro = 0;
		int i = 0;

		ifc[17 - 1] = '\0';
		tmp[17 - 1] = '\0';
		strncpy(ifc, argv[1], sizeof(char)*16);
		strncpy(tmp, argv[2], sizeof(char)*16);
		thro = atoi(tmp);

		TM_INFO_LOG("CMD MODE %s %d", ifc, thro);

		for ( i=0; i<IFC_NUM; i++) {
			if(!strncmp (IFC_NAME[i], ifc, 2)) {
				ifc_set_throttle(IFC_NAME[i], thro * ONE_MBITS_PER_SEC, thro * ONE_MBITS_PER_SEC);
				#ifdef NEVER
			 	exe_cmd(i, thro);
				#endif
			} else
				TM_DBG_LOG("NOT %s!", IFC_NAME[i]);
		}

#if defined(MD_UL_DR_THROTTLE)
		for ( i=0; i<MD_IFC_NUM; i++) {
			if(!strncmp (MD_IFC_NAME[i], ifc, 7)) {
				ifc_set_throttle(MD_IFC_NAME[i], -1, thro);
				#ifdef NEVER
			 	exe_cmd(i, thro);
				#endif
			} else
				TM_DBG_LOG("NOT %s!", IFC_NAME[i]);
		}
#endif
	} else {
		int fd = open(PROCFS_TM_PID, O_RDWR);
		int pid = getpid();
		int ret = 0;
		char pid_string[32] = {0};
		struct sigaction act;

#if defined(MTK_THERMAL_PA_VIA_ATCMD)
		int count = 0;
		int socket;
#endif

		TM_INFO_LOG("START+++++++++ %d", getpid());

		/* Create signal handler */
		memset(&act, 0, sizeof(act));
		act.sa_flags = SA_SIGINFO;
		//act.sa_handler = signal_handler;
		act.sa_sigaction = signal_handler;
		sigemptyset(&act.sa_mask);

		sigaction(SIGIO, &act, NULL);

		sprintf(pid_string, "%d", pid);

        /* Write pid to procfs */
        if (fd >=0 ){
			ret = write(fd, pid_string, sizeof(char) * strlen(pid_string));
			if (ret <= 0)	{
				TM_INFO_LOG("Fail to write %d to %s %x\n", pid, PROCFS_TM_PID, ret);
			} else {
				TM_INFO_LOG("Success to write %d to %s\n", pid, PROCFS_TM_PID);
			}
			close(fd);
        }
		fd = 0;

#ifdef MD_TOGGLE_SUPPORT
		int Mfd = open(PROCFS_CLMUTT_TM_PID, O_RDWR);
			if (Mfd >=0 ){
			ret = write(Mfd, pid_string, sizeof(char) * strlen(pid_string));
			if (ret <= 0)	{
				TM_INFO_LOG("Fail to write %d to %s %x\n", pid, PROCFS_CLMUTT_TM_PID, ret);
			} else {
				TM_INFO_LOG("Success to write %d to %s\n", pid, PROCFS_CLMUTT_TM_PID);
			}
			close(Mfd);
        }
		Mfd = 0;

#ifdef MD_TOGGLE_SUPPORT_C2K
		RilRPC_init();
#endif
#endif

#if defined(MD_UL_DR_THROTTLE)
        fd = open(PROCFS_MD_UL_TM_PID, O_RDWR);
		if (fd >=0 ){
	        ret = write(fd, pid_string, sizeof(char) * strlen(pid_string));
			if (ret <= 0)	{
				TM_INFO_LOG("Fail to write %d to %s %x\n", pid, PROCFS_TM_PID, ret);
			} else {
				TM_INFO_LOG("Success to write %d to %s\n", pid, PROCFS_TM_PID);
			}
			close(fd);
        }
		fd = 0;


		// new
		//thermal_create_timers();
#endif

#if defined(MD_3G_UL_THROTTLE)
		int fd_3G = open(PROCFS_3GMD_UL_TM_PID, O_RDWR);
		if (fd_3G >= 0){
			ret = write(fd_3G, pid_string, sizeof(char) * strlen(pid_string));
			if (ret <= 0)	{
				TM_INFO_LOG("Fail to write %d to %s %x\n", pid, PROCFS_3GMD_UL_TM_PID, ret);
			} else {
				TM_INFO_LOG("Success to write %d to %s\n", pid, PROCFS_3GMD_UL_TM_PID);
			}
			close(fd_3G);
        }
		fd_3G = 0;

        thermal_create_timers_3Gmd();
#endif


#ifdef NEVER
		/* Check throttl.sh */
		if (0 == access(THROTTLE_SCRIPT_PATH, R_OK | X_OK)) {
			ret = chmod(THROTTLE_SCRIPT_PATH, S_ISUID | S_ISVTX | S_IRUSR | S_IXUSR);
			if (ret == 0)	{
				TM_INFO_LOG("Success to chomd\n");
			} else {
				TM_INFO_LOG("Fail to chmod %x\n", ret);
			}
		} else {
			TM_INFO_LOG("failed to access %s", THROTTLE_SCRIPT_PATH);
		}
#endif /* NEVER */

#if defined(MTK_THERMAL_PA_VIA_ATCMD)

#if defined(THERMAL_MD_TP)
        new_md_tp_flow();


#else


        sleep(60);

		TM_INFO_LOG("Enter infinite loop\n");

		signal(SIGPIPE, handle_pipe);


		while(1)
		{
            count++;

            TM_DBG_LOG("count %d\n", count);

            sleep(5); // delay 5s

            // connect to rild...
            socket = connectToRild(NULL);
            while (socket == -1) {
                sleep(1);
                socket = connectToRild(NULL);
                TM_DBG_LOG("socket %d\n", socket);
            }

			{
			    // For RF temp & all
                queryMdThermalInfo(socket, 0, -1);

                recvMdThermalInfo(socket, 0, -1);
			}

			disconnectRild(socket);

#if 0
			// connect to rild...
			socket = connectToRild(NULL);
            while (socket == -1) {
                sleep(1);
                socket = connectToRild(NULL);
                TM_INFO_LOG("socket %d\n", socket);
            }

			// For AUX ADC
            queryMdThermalInfo(socket, 0, 0);

            recvMdThermalInfo(socket, 0, 0);

            disconnectRild(socket);

            // connect to rild...
            socket = connectToRild(NULL);
            while (socket == -1) {
                sleep(1);
                socket = connectToRild(NULL);
                TM_INFO_LOG("socket %d\n", socket);
            }

			// For PoP DRAM
            queryMdThermalInfo(socket, 0, 1);

            recvMdThermalInfo(socket, 0, 1);

            disconnectRild(socket);
#endif
		}
#endif

		TM_INFO_LOG("END-----------\n");
#else
 		TM_INFO_LOG("Enter infinite loop");

		while(1) {
			sleep(100);
		}

		TM_INFO_LOG("END-----------");
#endif
	}

	return 0;
}
