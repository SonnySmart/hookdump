#include "Log.h"
#include "myInclude.h"


static uint32_t myMemProtect(void* tgAdress,unsigned size,unsigned uSt)
{
	if(tgAdress==0)
	{
		return 0;
	}

	int ii=0;

	//LOGD("start myMemProtect");

	mprotect((void*)tgAdress,size,uSt);
	unsigned int ulFirstPageStart = (unsigned)tgAdress - ((unsigned)tgAdress % 4096);

	unsigned int ulLastPageEnd = (unsigned)tgAdress + size;
	ulLastPageEnd += 4096 - (ulLastPageEnd % 4096);
	unsigned RealStart=ulFirstPageStart;
	unsigned RealSize=ulLastPageEnd-RealStart;


	//LOGD("mprotect(%08x,%08x,%08x);",RealStart,RealSize,uSt);
	ii=mprotect((void*)RealStart,RealSize,uSt );
	//LOGD("mprotect--> %x %s",ii,strerror(errno));


	//LOGD("end myMemProtect");
	return 1;

}

typedef void (*myown_call)(void);





#ifdef __i386__

//50 50 E8 00 00 00 00 58 8B 40 0D 89 44 24 04 58 C3

//773A8F08    50              push    eax
//773A8F09    50              push    eax
//773A8F0A    E8 00000000     call    773A8F0F
//773A8F0F    58              pop     eax
//773A8F10 >  8B40 0D         mov     eax, dword ptr [eax+D]
//773A8F13    894424 04       mov     dword ptr [esp+4], eax
//773A8F17    58              pop     eax
//773A8F18    C3              retn

//���ֽڶ���

static unsigned char  JmpCode[] ={0x50,0x50,0xE8,0x00,0x00,0x00,0x00,0x58,0x8B,0x40,0x0D,0x89,0x44,0x24,0x04,0x58,0xC3,0x00,0x00,0x00};

#define GetProcessAddress( name ) \
	while(1)\
	{\
		void* pForTest=dlsym(handle, #name );\
		myMemProtect((void*)& name ,sizeof(JmpStruct),PROT_READ |PROT_WRITE |PROT_EXEC);\
		memcpy( name .code,JmpCode,20);\
		/*LOGD("pForTest %08x",(uint32_t)pForTest);*/\
		 name .address=(uint32_t)pForTest;\
		break;\
	};


#else

//46 ???? 00802DE9                      push {pc}
//47 ???? 01002DE9                      push {r0}
//48 0040 00802DE9                      push {pc}
//49 0044 0100BDE8                      pop {r0}
//49 ???? 0C0090E5                      ldr r0,[r0,#12]
//50 ???? 08008DE5                      str r0,[sp,#8]
//51 ???? 0100BDE8                      pop {r0}
//52 ???? 0080BDE8                      pop {pc}
//53	  140090E5                      ldr r0,[r0,#20]

static unsigned char  JmpCode[] ={0x00,0x80,0x2d,0xe9,
								0x01,0x00,0x2d,0xe9,
								0x00,0x80,0x2d,0xe9,
								0x01,0x00,0xbd,0xe8,
								0x10,0x00,0x90,0xe5,//!!!!ldr r0,[r0,#16]
								0x04,0x00,0x8d,0xe5,//str r0,[sp,#8]
								0x01,0x00,0xbd,0xe8,
								0x00,0x80,0xbd,0xe8};

#define GetProcessAddress( name ) \
	while(1)\
	{\
		void* pForTest=dlsym(handle, #name );\
		myMemProtect((void*)& name ,sizeof(JmpStruct),PROT_READ |PROT_WRITE |PROT_EXEC);\
		memcpy( name .code,JmpCode,36);\
		 name .address=(uint32_t)pForTest;\
        /*LOGD("%s --> %p" , #name , pForTest );*/\
		break;\
	};

#endif

__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct g_TssGame;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_setuserinfo_ex;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_setgamestatus;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_init;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_ioctl;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_get_report_data;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_del_report_data;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_rcv_anti_data;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_decryptpacket;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tss_gamedemo_Game_init;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tss_gamedemo_Game_onPause;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tss_gamedemo_Game_onResume;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct _ZN3tp210gen_randomEv;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct _ZN6TssSdk16sdt_report_errorEv;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct _ZN6TssSdk10gen_randomEv;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_setuserinfo;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_encryptpacket;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_enable_get_report_data;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_ischeatpacket;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_log_str;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_getver;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_setgamestatus;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_setoptions;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_sdk_ioctl;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_sdk_init;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_sdk_init_ex;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_setuserinfo;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_get_report_data2;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_unity_is_enable;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_unity_str;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tp2_free_anti_data;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_set_token;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_gen_session_data;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_sdk_wait_verify;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct g_AllTssExportFunc;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct GetTssExportFunc2;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_isToastEnabled;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_isRookitRunning;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_isAppOnFrontend;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_hasMatchRate;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_setrootkittipstate;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_setcancelupdaterootkit;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_loadMalwareScanInfo;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_forceExit;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_setsenddatatosvrcb;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_senddatatosdk;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_senddatatosvr;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_onruntimeinfo;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_init;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_setuserinfo;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_setuserinfoex;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_setgamestatus;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_getsdkantidata;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct TssJavaMethod_SendCmd;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_loadConfig;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_loadRootkitTipStr;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct Java_com_tencent_tp_TssSdk_loadMessageBoxInfo;
__attribute__((section(".text")))  __attribute__((visibility("default"))) JmpStruct tss_jni_cmd;

static void my_init(void)
{

    LOGD("start my_init");

#ifdef __i386__
    //void* handleAllStartHere = dlopen("/system/lib/libnative-lib.so", RTLD_NOW);
	void* handle = dlopen("/system/lib/libGLESv1_CM_me.so", RTLD_NOW);
#else
    //void* handleAllStartHere = dlopen("/system/lib/libnative-lib.so", RTLD_NOW);
	void* handle = dlopen("/system/lib/libGLESv1_CM_me.so", RTLD_NOW);
#endif
    //LOGD("this is fake handle %p %p %p",handle,&TESTaDDRESS,my_init);
//    typedef void* (*type_handleAllStartHere)(void*);
//    type_handleAllStartHere func_handleAllStartHere= (type_handleAllStartHere)dlsym(handleAllStartHere,"handleAllStartHere");
//    func_handleAllStartHere(0);

//	void* pForTest=dlsym(handle,"fortest");
//	//	typedef JNIEnv* (*_Z15dvmCreateJNIEnvP6Thread)(void*);
//	//	_Z15dvmCreateJNIEnvP6Thread dvmCreateJNIEnv=(_Z15dvmCreateJNIEnvP6Thread)dlsym(handle,"_Z15dvmCreateJNIEnvP6Thread");
//	////////////////////////////////////////////
//
//	//0x00F09FE5
//
//	myMemProtect((void*)&testtest,sizeof(JmpStruct),PROT_READ |PROT_WRITE |PROT_EXEC);
//	memcpy(testtest.code,JmpCode,36);
//
//	testtest.address=(uint32_t)pForTest;

	GetProcessAddress( g_TssGame );
	GetProcessAddress( tss_sdk_setuserinfo_ex );
	GetProcessAddress( tss_sdk_setgamestatus );
	GetProcessAddress( tss_sdk_init );
	GetProcessAddress( tss_sdk_ioctl );
	GetProcessAddress( tss_get_report_data );
	GetProcessAddress( tss_del_report_data );
	GetProcessAddress( tss_sdk_rcv_anti_data );
	GetProcessAddress( tss_sdk_decryptpacket );
	GetProcessAddress( Java_com_tss_gamedemo_Game_init );
	GetProcessAddress( Java_com_tss_gamedemo_Game_onPause );
	GetProcessAddress( Java_com_tss_gamedemo_Game_onResume );
	GetProcessAddress( _ZN3tp210gen_randomEv );
	GetProcessAddress( _ZN6TssSdk16sdt_report_errorEv );
	GetProcessAddress( _ZN6TssSdk10gen_randomEv );
	GetProcessAddress( tss_sdk_setuserinfo );
	GetProcessAddress( tss_sdk_encryptpacket );
	GetProcessAddress( tss_enable_get_report_data );
	GetProcessAddress( tss_sdk_ischeatpacket );
	GetProcessAddress( tss_log_str );
	GetProcessAddress( tp2_getver );
	GetProcessAddress( tp2_setgamestatus );
	GetProcessAddress( tp2_setoptions );
	GetProcessAddress( tp2_sdk_ioctl );
	GetProcessAddress( tp2_sdk_init );
	GetProcessAddress( tp2_sdk_init_ex );
	GetProcessAddress( tp2_setuserinfo );
	GetProcessAddress( tss_get_report_data2 );
	GetProcessAddress( tss_unity_is_enable );
	GetProcessAddress( tss_unity_str );
	GetProcessAddress( tp2_free_anti_data );
	GetProcessAddress( tss_sdk_set_token );
	GetProcessAddress( tss_sdk_gen_session_data );
	GetProcessAddress( tss_sdk_wait_verify );
	GetProcessAddress( g_AllTssExportFunc );
	GetProcessAddress( GetTssExportFunc2 );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_isToastEnabled );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_isRookitRunning );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_isAppOnFrontend );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_hasMatchRate );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_setrootkittipstate );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_setcancelupdaterootkit );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_loadMalwareScanInfo );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_forceExit );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_setsenddatatosvrcb );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_senddatatosdk );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_senddatatosvr );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_onruntimeinfo );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_init );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_setuserinfo );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_setuserinfoex );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_setgamestatus );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_getsdkantidata );
	GetProcessAddress( TssJavaMethod_SendCmd );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_loadConfig );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_loadRootkitTipStr );
	GetProcessAddress( Java_com_tencent_tp_TssSdk_loadMessageBoxInfo );
	GetProcessAddress( tss_jni_cmd );
}

#define DECODEKEY
#ifdef DECODEKEY
myown_call testinit __attribute__((section(".init_array")))=my_init;
#endif

