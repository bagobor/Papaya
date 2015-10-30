// TODO: Documentation
// TODO: Platform-specific print macro


#ifndef EASYTAB_H
#define EASYTAB_H

#include <stdint.h>

// -----------------------------------------------------------------------------
// wintab.h
#if 1

    DECLARE_HANDLE(HMGR);
    DECLARE_HANDLE(HCTX);

    typedef DWORD WTPKT;
    typedef DWORD FIX32;

    // Messages
    #if 1

        #define WT_DEFBASE          0x7FF0
        #define WT_MAXOFFSET        0xF

        #define _WT_PACKET(b)       ((b)+0)
        #define _WT_CTXOPEN(b)      ((b)+1)
        #define _WT_CTXCLOSE(b)     ((b)+2)
        #define _WT_CTXUPDATE(b)    ((b)+3)
        #define _WT_CTXOVERLAP(b)   ((b)+4)
        #define _WT_PROXIMITY(b)    ((b)+5)
        #define _WT_INFOCHANGE(b)   ((b)+6)
        #define _WT_CSRCHANGE(b)    ((b)+7) /* 1.1 */
        #define _WT_PACKETEXT(b)    ((b)+8) /* 1.4 */
        #define _WT_MAX(b)          ((b)+WT_MAXOFFSET)

        #define WT_PACKET           _WT_PACKET(WT_DEFBASE)
        #define WT_CTXOPEN          _WT_CTXOPEN(WT_DEFBASE)
        #define WT_CTXCLOSE         _WT_CTXCLOSE(WT_DEFBASE)
        #define WT_CTXUPDATE        _WT_CTXUPDATE(WT_DEFBASE)
        #define WT_CTXOVERLAP       _WT_CTXOVERLAP(WT_DEFBASE)
        #define WT_PROXIMITY        _WT_PROXIMITY(WT_DEFBASE)
        #define WT_INFOCHANGE       _WT_INFOCHANGE(WT_DEFBASE)
        #define WT_CSRCHANGE        _WT_CSRCHANGE(WT_DEFBASE) /* 1.1 */
        #define WT_PACKETEXT        _WT_PACKETEXT(WT_DEFBASE) /* 1.4 */
        #define WT_MAX              _WT_MAX(WT_DEFBASE)

    #endif // Messages

    // Flags
    #if 1

        #define CTX_NAME        1
        #define CTX_OPTIONS     2
        #define CTX_STATUS      3
        #define CTX_LOCKS       4
        #define CTX_MSGBASE     5
        #define CTX_DEVICE      6
        #define CTX_PKTRATE     7
        #define CTX_PKTDATA     8
        #define CTX_PKTMODE     9
        #define CTX_MOVEMASK    10
        #define CTX_BTNDNMASK   11
        #define CTX_BTNUPMASK   12
        #define CTX_INORGX      13
        #define CTX_INORGY      14
        #define CTX_INORGZ      15
        #define CTX_INEXTX      16
        #define CTX_INEXTY      17
        #define CTX_INEXTZ      18
        #define CTX_OUTORGX     19
        #define CTX_OUTORGY     20
        #define CTX_OUTORGZ     21
        #define CTX_OUTEXTX     22
        #define CTX_OUTEXTY     23
        #define CTX_OUTEXTZ     24
        #define CTX_SENSX       25
        #define CTX_SENSY       26
        #define CTX_SENSZ       27
        #define CTX_SYSMODE     28
        #define CTX_SYSORGX     29
        #define CTX_SYSORGY     30
        #define CTX_SYSEXTX     31
        #define CTX_SYSEXTY     32
        #define CTX_SYSSENSX    33
        #define CTX_SYSSENSY    34
        #define CTX_MAX         34

        // Context option values
        #define CXO_SYSTEM      0x0001
        #define CXO_PEN         0x0002
        #define CXO_MESSAGES    0x0004
        #define CXO_MARGIN      0x8000
        #define CXO_MGNINSIDE   0x4000
        #define CXO_CSRMESSAGES 0x0008 /* 1.1 */

        #define DVC_NAME        1
        #define DVC_HARDWARE    2
        #define DVC_NCSRTYPES   3
        #define DVC_FIRSTCSR    4
        #define DVC_PKTRATE     5
        #define DVC_PKTDATA     6
        #define DVC_PKTMODE     7
        #define DVC_CSRDATA     8
        #define DVC_XMARGIN     9
        #define DVC_YMARGIN     10
        #define DVC_ZMARGIN     11
        #define DVC_X           12
        #define DVC_Y           13
        #define DVC_Z           14
        #define DVC_NPRESSURE   15
        #define DVC_TPRESSURE   16
        #define DVC_ORIENTATION 17
        #define DVC_ROTATION    18 /* 1.1 */
        #define DVC_PNPID       19 /* 1.1 */
        #define DVC_MAX         19

        #define PK_CONTEXT           0x0001 // reporting context
        #define PK_STATUS            0x0002 // status bits
        #define PK_TIME              0x0004 // time stamp
        #define PK_CHANGED           0x0008 // change bit vector
        #define PK_SERIAL_NUMBER     0x0010 // packet serial number
        #define PK_CURSOR            0x0020 // reporting cursor
        #define PK_BUTTONS           0x0040 // button information
        #define PK_X                 0x0080 // x axis
        #define PK_Y                 0x0100 // y axis
        #define PK_Z                 0x0200 // z axis
        #define PK_NORMAL_PRESSURE   0x0400 // normal or tip pressure
        #define PK_TANGENT_PRESSURE  0x0800 // tangential or barrel pressure
        #define PK_ORIENTATION       0x1000 // orientation info: tilts
        #define PK_ROTATION          0x2000 // rotation info; 1.1

        // constants for use with pktdef.h
        #define PKEXT_ABSOLUTE  1
        #define PKEXT_RELATIVE  2

        #define WTI_DEFCONTEXT  3
        #define WTI_DEFSYSCTX   4
        #define WTI_DEVICES     100
        #define WTI_DDCTXS      400 /* 1.1 */
        #define WTI_DSCTXS      500 /* 1.1 */

    #endif // Flags

    typedef struct tagAXIS {
        LONG    axMin;
        LONG    axMax;
        UINT    axUnits;
        FIX32   axResolution;
    } AXIS, *PAXIS, NEAR *NPAXIS, FAR *LPAXIS;

    #define LCNAMELEN 40
    typedef struct tagLOGCONTEXTA {
        char    lcName[LCNAMELEN];
        UINT    lcOptions;
        UINT    lcStatus;
        UINT    lcLocks;
        UINT    lcMsgBase;
        UINT    lcDevice;
        UINT    lcPktRate;
        WTPKT   lcPktData;
        WTPKT   lcPktMode;
        WTPKT   lcMoveMask;
        DWORD   lcBtnDnMask;
        DWORD   lcBtnUpMask;
        LONG    lcInOrgX;
        LONG    lcInOrgY;
        LONG    lcInOrgZ;
        LONG    lcInExtX;
        LONG    lcInExtY;
        LONG    lcInExtZ;
        LONG    lcOutOrgX;
        LONG    lcOutOrgY;
        LONG    lcOutOrgZ;
        LONG    lcOutExtX;
        LONG    lcOutExtY;
        LONG    lcOutExtZ;
        FIX32   lcSensX;
        FIX32   lcSensY;
        FIX32   lcSensZ;
        BOOL    lcSysMode;
        int     lcSysOrgX;
        int     lcSysOrgY;
        int     lcSysExtX;
        int     lcSysExtY;
        FIX32   lcSysSensX;
        FIX32   lcSysSensY;
    } LOGCONTEXTA, *PLOGCONTEXTA, NEAR *NPLOGCONTEXTA, FAR *LPLOGCONTEXTA;

    typedef struct tagEXTENSIONBASE { /* 1.4 */
        HCTX    nContext;
        UINT    nStatus;
        DWORD   nTime;
        UINT    nSerialNumber;
    } EXTENSIONBASE;

#endif // wintab.h
// -----------------------------------------------------------------------------

#define PACKETDATA PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE
#define PACKETMODE PK_BUTTONS

// -----------------------------------------------------------------------------
// pktdef.h
#if 1

    // TODO: Simplify this file if we have a fixed packet format.
    //       The macros here are too ugly.

    #ifndef PACKETNAME
    /* if no packet name prefix */
    #define __PFX(x)	x
    #define __IFX(x,y)	x ## y
    #else
    /* add prefixes and infixes to packet format names */
    #define __PFX(x)		__PFX2(PACKETNAME,x)
    #define __PFX2(p,x)		__PFX3(p,x)
    #define __PFX3(p,x)		p ## x
    #define __IFX(x,y)		__IFX2(x,PACKETNAME,y)
    #define __IFX2(x,i,y)	__IFX3(x,i,y)
    #define __IFX3(x,i,y)	x ## i ## y
    #endif

    #define __SFX2(x,s)		__SFX3(x,s)
    #define __SFX3(x,s)		x ## s

    #define __TAG  	__IFX(tag,PACKET)
    #define __TYPES	__PFX(PACKET), * __IFX(P,PACKET), NEAR * __IFX(NP,PACKET), FAR * __IFX(LP,PACKET)

    #define __TAGE  	__IFX(tag,PACKETEXT)
    #define __TYPESE	__PFX(PACKETEXT), * __IFX(P,PACKETEXT), NEAR * __IFX(NP,PACKETEXT), FAR * __IFX(LP,PACKETEXT)

    #define __DATA		(__PFX(PACKETDATA))
    #define __MODE		(__PFX(PACKETMODE))
    #define __EXT(x)	__SFX2(__PFX(PACKET),x)


    typedef struct __TAG {
    #if (__DATA & PK_CONTEXT)
        HCTX			pkContext;
    #endif
    #if (__DATA & PK_STATUS)
        UINT			pkStatus;
    #endif
    #if (__DATA & PK_TIME)
        DWORD			pkTime;
    #endif
    #if (__DATA & PK_CHANGED)
        WTPKT			pkChanged;
    #endif
    #if (__DATA & PK_SERIAL_NUMBER)
        UINT			pkSerialNumber;
    #endif
    #if (__DATA & PK_CURSOR)
        UINT			pkCursor;
    #endif
    #if (__DATA & PK_BUTTONS)
        DWORD			pkButtons;
    #endif
    #if (__DATA & PK_X)
        LONG			pkX;
    #endif
    #if (__DATA & PK_Y)
        LONG			pkY;
    #endif
    #if (__DATA & PK_Z)
        LONG			pkZ;
    #endif
    #if (__DATA & PK_NORMAL_PRESSURE)
    #if (__MODE & PK_NORMAL_PRESSURE)
        /* relative */
        int			pkNormalPressure;
    #else
        /* absolute */
        UINT		pkNormalPressure;
    #endif
    #endif
    #if (__DATA & PK_TANGENT_PRESSURE)
    #if (__MODE & PK_TANGENT_PRESSURE)
        /* relative */
        int			pkTangentPressure;
    #else
        /* absolute */
        UINT		pkTangentPressure;
    #endif
    #endif
    #if (__DATA & PK_ORIENTATION)
        ORIENTATION		pkOrientation;
    #endif
    #if (__DATA & PK_ROTATION)
        ROTATION		pkRotation; /* 1.1 */
    #endif

    #ifndef NOWTEXTENSIONS
                                    /* extensions begin here. */
    #if (__EXT(FKEYS) == PKEXT_RELATIVE) || (__EXT(FKEYS) == PKEXT_ABSOLUTE)
        UINT			pkFKeys;
    #endif
    #if (__EXT(TILT) == PKEXT_RELATIVE) || (__EXT(TILT) == PKEXT_ABSOLUTE)
        TILT			pkTilt;
    #endif
    #endif

    } __TYPES;

    #ifndef NOWTEXTENSIONS
    typedef struct __TAGE {
        EXTENSIONBASE	pkBase;

    #if (__EXT(EXPKEYS) == PKEXT_RELATIVE) || (__EXT(EXPKEYS) == PKEXT_ABSOLUTE)
        EXPKEYSDATA pkExpKeys; /* 1.4 */
    #endif
    #if (__EXT(TOUCHSTRIP) == PKEXT_RELATIVE) || (__EXT(TOUCHSTRIP) == PKEXT_ABSOLUTE)
        SLIDERDATA	pkTouchStrip; /* 1.4 */
    #endif
    #if (__EXT(TOUCHRING) == PKEXT_RELATIVE) || (__EXT(TOUCHRING) == PKEXT_ABSOLUTE)
        SLIDERDATA	pkTouchRing; /* 1.4 */
    #endif

    } __TYPESE;
    #endif

    #undef PACKETNAME
    #undef __TAG
    #undef __TAGE
    #undef __TAG2
    #undef __TYPES
    #undef __TYPESE
    #undef __TYPES2
    #undef __DATA
    #undef __MODE
    #undef __PFX
    #undef __PFX2
    #undef __PFX3
    #undef __IFX
    #undef __IFX2
    #undef __IFX3
    #undef __SFX2
    #undef __SFX3

#endif // pktdef.h
// -----------------------------------------------------------------------------

typedef UINT (WINAPI * WTINFOA) (UINT, UINT, LPVOID);
typedef HCTX (WINAPI * WTOPENA) (HWND, LPLOGCONTEXTA, BOOL);
typedef BOOL (WINAPI * WTGETA) (HCTX, LPLOGCONTEXTA);
typedef BOOL (WINAPI * WTSETA) (HCTX, LPLOGCONTEXTA);
typedef BOOL (WINAPI * WTCLOSE) (HCTX);
typedef BOOL (WINAPI * WTENABLE) (HCTX, BOOL);
typedef BOOL (WINAPI * WTPACKET) (HCTX, UINT, LPVOID);
typedef BOOL (WINAPI * WTOVERLAP) (HCTX, BOOL);
typedef BOOL (WINAPI * WTSAVE) (HCTX, LPVOID);
typedef BOOL (WINAPI * WTCONFIG) (HCTX, HWND);
typedef HCTX (WINAPI * WTRESTORE) (HWND, LPVOID, BOOL);
typedef BOOL (WINAPI * WTEXTSET) (HCTX, UINT, LPVOID);
typedef BOOL (WINAPI * WTEXTGET) (HCTX, UINT, LPVOID);
typedef BOOL (WINAPI * WTQUEUESIZESET) (HCTX, int);
typedef int  (WINAPI * WTDATAPEEK) (HCTX, UINT, UINT, int, LPVOID, LPINT);
typedef int  (WINAPI * WTPACKETSGET) (HCTX, int, LPVOID);
typedef HMGR (WINAPI * WTMGROPEN) (HWND, UINT);
typedef BOOL (WINAPI * WTMGRCLOSE) (HMGR);
typedef HCTX (WINAPI * WTMGRDEFCONTEXT) (HMGR, BOOL);
typedef HCTX (WINAPI * WTMGRDEFCONTEXTEX) (HMGR, UINT, BOOL);

struct EasyTabInfo
{
    HINSTANCE Dll;
    HCTX      Context;

    LONG  PosX, PosY;
    float Pressure;

    LONG RangeX, RangeY;
    UINT MaxPressure;

    WTINFOA           WTInfoA;
    WTOPENA           WTOpenA;
    WTGETA            WTGetA;
    WTSETA            WTSetA;
    WTCLOSE           WTClose;
    WTPACKET          WTPacket;
    WTENABLE          WTEnable;
    WTOVERLAP         WTOverlap;
    WTSAVE            WTSave;
    WTCONFIG          WTConfig;
    WTRESTORE         WTRestore;
    WTEXTSET          WTExtSet;
    WTEXTGET          WTExtGet;
    WTQUEUESIZESET    WTQueueSizeSet;
    WTDATAPEEK        WTDataPeek;
    WTPACKETSGET      WTPacketsGet;
    WTMGROPEN         WTMgrOpen;
    WTMGRCLOSE        WTMgrClose;
    WTMGRDEFCONTEXT   WTMgrDefContext;
    WTMGRDEFCONTEXTEX WTMgrDefContextEx;
};

static EasyTabInfo* EasyTab;

#endif // EASYTAB_H

// =============================================================================

#ifdef EASYTAB_IMPLEMENTATION

#define GETPROCADDRESS(type, func)                                              \
    EasyTab->func = (type)GetProcAddress(EasyTab->Dll, #func);                  \
    if (!EasyTab->func)                                                         \
    {                                                                           \
        OutputDebugStringA("Function " #func " not found in Wintab32.dll.\n");  \
        return false;                                                           \
    }

bool EasyTab_Load(HWND Window)
{
    EasyTab = (EasyTabInfo*)calloc(1, sizeof(EasyTabInfo)); // We want init to zero, hence calloc.
    if (!EasyTab) { return false; }

    // Load EasyTab DLL and get function addresses
    {
        EasyTab->Dll = LoadLibraryA("Wintab32.dll");
        if (!EasyTab->Dll)
        {
            OutputDebugStringA("Wintab32.dll not found.\n");
            return false;
        }

        GETPROCADDRESS(WTINFOA           , WTInfoA);
        GETPROCADDRESS(WTOPENA           , WTOpenA);
        GETPROCADDRESS(WTGETA            , WTGetA);
        GETPROCADDRESS(WTSETA            , WTSetA);
        GETPROCADDRESS(WTCLOSE           , WTClose);
        GETPROCADDRESS(WTPACKET          , WTPacket);
        GETPROCADDRESS(WTENABLE          , WTEnable);
        GETPROCADDRESS(WTOVERLAP         , WTOverlap);
        GETPROCADDRESS(WTSAVE            , WTSave);
        GETPROCADDRESS(WTCONFIG          , WTConfig);
        GETPROCADDRESS(WTRESTORE         , WTRestore);
        GETPROCADDRESS(WTEXTSET          , WTExtSet);
        GETPROCADDRESS(WTEXTGET          , WTExtGet);
        GETPROCADDRESS(WTQUEUESIZESET    , WTQueueSizeSet);
        GETPROCADDRESS(WTDATAPEEK        , WTDataPeek);
        GETPROCADDRESS(WTPACKETSGET      , WTPacketsGet);
        GETPROCADDRESS(WTMGROPEN         , WTMgrOpen);
        GETPROCADDRESS(WTMGRCLOSE        , WTMgrClose);
        GETPROCADDRESS(WTMGRDEFCONTEXT   , WTMgrDefContext);
        GETPROCADDRESS(WTMGRDEFCONTEXTEX , WTMgrDefContextEx);
    }

    if (!EasyTab->WTInfoA(0, 0, NULL))
    {
        OutputDebugStringA("Wintab services not available.\n");
        return false;
    }

    // Open context
    {
        LOGCONTEXTA LogContext = {0};
        AXIS        RangeX     = {0};
        AXIS        RangeY     = {0};
        AXIS        Pressure   = {0};

        EasyTab->WTInfoA(WTI_DDCTXS, 0, &LogContext);
        EasyTab->WTInfoA(WTI_DEVICES, DVC_X, &RangeX);
        EasyTab->WTInfoA(WTI_DEVICES, DVC_Y, &RangeY);
        EasyTab->WTInfoA(WTI_DEVICES, DVC_NPRESSURE, &Pressure);

        LogContext.lcPktData = PACKETDATA; // ??
        LogContext.lcOptions |= CXO_SYSTEM;
        LogContext.lcOptions |= CXO_MESSAGES;
        LogContext.lcPktMode = PACKETMODE;
        LogContext.lcMoveMask = PACKETDATA;
        LogContext.lcBtnUpMask = LogContext.lcBtnDnMask;

        LogContext.lcInOrgX = 0;
        LogContext.lcInOrgY = 0;
        LogContext.lcInExtX = RangeX.axMax;
        LogContext.lcInExtY = RangeY.axMax;

        LogContext.lcOutOrgX = 0;
        LogContext.lcOutOrgY = 0;
        LogContext.lcOutExtX = GetSystemMetrics(SM_CXSCREEN);
        LogContext.lcOutExtY = -GetSystemMetrics(SM_CYSCREEN);

        LogContext.lcSysOrgX = 0;
        LogContext.lcSysOrgY = 0;
        LogContext.lcSysExtX = GetSystemMetrics(SM_CXSCREEN);
        LogContext.lcSysExtY = GetSystemMetrics(SM_CYSCREEN);

        EasyTab->Context = EasyTab->WTOpenA(Window, &LogContext, TRUE);
        
        if (!EasyTab->Context)
        {
            OutputDebugStringA("Wintab context couldn't be opened.\n");
            return false;
        }

        // Get tablet capabilites
        {
            EasyTab->MaxPressure = Pressure.axMax;
            EasyTab->RangeX      = RangeX.axMax;
            EasyTab->RangeY      = RangeY.axMax;
        }
    }

    return true;
}

#undef GETPROCADDRESS

void EasyTab_HandleEvent(HWND Window, LPARAM LParam, WPARAM WParam)
{
    PACKET Packet = { 0 };
    if ((HCTX)LParam == EasyTab->Context &&
        EasyTab->WTPacket(EasyTab->Context, WParam, &Packet))
    {
        POINT Point = { 0 };
        Point.x = Packet.pkX;
        Point.y = Packet.pkY;
        ScreenToClient(Window, &Point); // TODO: Is ScreenToClient() required? 
                                        //       Do we want it to be optional?
        EasyTab->PosX = Point.x;
        EasyTab->PosY = Point.y;

        EasyTab->Pressure = (float)Packet.pkNormalPressure / (float)EasyTab->MaxPressure;
    }
}

void EasyTab_Unload()
{
    EasyTab->WTClose(EasyTab->Context);

    if (EasyTab->Dll)
    {
        FreeLibrary(EasyTab->Dll);
    }

    free(EasyTab);
}

#endif // EASYTAB_IMPLEMENTATION
