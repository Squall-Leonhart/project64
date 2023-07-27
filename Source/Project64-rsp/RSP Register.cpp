#include <windows.h>

#include "Rsp.h"
#include "cpu/RspTypes.h"
#include <commctrl.h>
#include <stdio.h>

#define GeneralPurpose 1
#define ControlProcessor0 2
#define HiddenRegisters 3
#define Vector1 4
#define Vector2 5

#define IDC_TAB_CONTROL 1000

void Create_RSP_Register_Window(int);
void HideRSP_RegisterPanel(int);
void PaintRSP_HiddenPanel(HWND);
void PaintRSP_CP0Panel(HWND);
void PaintRSP_GPRPanel(HWND);
void PaintRSP_Vector1_Panel(HWND);
void PaintRSP_Vector2_Panel(HWND);
void ShowRSP_RegisterPanel(int);
void SetupRSP_HiddenPanel(HWND);
void SetupRSP_CP0Panel(HWND);
void SetupRSP_GPRPanel(HWND);
void SetupRSP_RegistersMain(HWND);
void SetupRSP_Vect1Panel(HWND);
void SetupRSP_Vect2Panel(HWND);
void UpdateRSPRegistersScreen(void);

LRESULT CALLBACK RefreshRSP_RegProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RSP_Registers_Proc(HWND, UINT, WPARAM, LPARAM);

HWND RSP_Registers_hDlg, hTab, hStatic, hGPR[32], hCP0[16], hHIDDEN[12],
    hVECT1[16], hVECT2[16];
int InRSPRegisterWindow = false;
WNDPROC RefreshProc;

// RSP registers
UWORD32 RSP_GPR[32], RSP_Flags[4];
UDWORD RSP_ACCUM[8];
RSPVector RSP_Vect[32];

char * GPR_Strings[32] = {
    "R0", "AT", "V0", "V1", "A0", "A1", "A2", "A3",
    "T0", "T1", "T2", "T3", "T4", "T5", "T6", "T7",
    "S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7",
    "T8", "T9", "K0", "K1", "GP", "SP", "S8", "RA"};

void Create_RSP_Register_Window(int Child)
{
    DWORD ThreadID;
    if (Child)
    {
        InRSPRegisterWindow = true;
        DialogBoxA((HINSTANCE)hinstDLL, "RSPREGISTERS", NULL, (DLGPROC)RSP_Registers_Proc);
        InRSPRegisterWindow = false;
    }
    else
    {
        if (!InRSPRegisterWindow)
        {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Create_RSP_Register_Window,
                         (LPVOID) true, 0, &ThreadID);
        }
        else
        {
            if (IsIconic((HWND)RSP_Registers_hDlg))
            {
                SendMessage(RSP_Registers_hDlg, WM_SYSCOMMAND, SC_RESTORE, 0);
            }
            SetForegroundWindow(RSP_Registers_hDlg);
        }
    }
}

void Enter_RSP_Register_Window(void)
{
    Create_RSP_Register_Window(false);
}

void HideRSP_RegisterPanel(int Panel)
{
    int count;

    switch (Panel)
    {
    case GeneralPurpose:
        for (count = 0; count < 32; count++)
        {
            ShowWindow(hGPR[count], false);
        }
        break;
    case ControlProcessor0:
        for (count = 0; count < 16; count++)
        {
            ShowWindow(hCP0[count], false);
        }
        break;
    case HiddenRegisters:
        for (count = 0; count < 12; count++)
        {
            ShowWindow(hHIDDEN[count], false);
        }
        break;
    case Vector1:
        for (count = 0; count < 16; count++)
        {
            ShowWindow(hVECT1[count], false);
        }
        break;
    case Vector2:
        for (count = 0; count < 16; count++)
        {
            ShowWindow(hVECT2[count], false);
        }
        break;
    }
}

void InitilizeRSPRegisters(void)
{
    memset(RSP_GPR, 0, sizeof(RSP_GPR));
    for (size_t i = 0, n = sizeof(RSP_Vect) / sizeof(RSP_Vect[0]); i < n; i++)
    {
        RSP_Vect[i] = RSPVector();
    }
}

void PaintRSP_HiddenPanel(HWND hWnd)
{
    PAINTSTRUCT ps;
    RECT rcBox;
    HFONT hOldFont;
    int OldBkMode;
    BeginPaint(hWnd, &ps);

    // Get the current DPI
    UINT dpi = GetDpiForWindow(hWnd);
    float scalingFactor = (float)dpi / 96;

    rcBox.left = (int)(41 * scalingFactor);
    rcBox.top = (int)(29 * scalingFactor);
    rcBox.right = (int)(573 * scalingFactor);
    rcBox.bottom = (int)(275 * scalingFactor);
    DrawEdge(ps.hdc, &rcBox, EDGE_ETCHED, BF_RECT);

    hOldFont = (HFONT)SelectObject(ps.hdc,
                                   GetStockObject(DEFAULT_GUI_FONT));
    OldBkMode = SetBkMode(ps.hdc, TRANSPARENT);

    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(46 * scalingFactor), "Accumulator Lo:", 15);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(74 * scalingFactor), "Accumulator Md:", 15);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(102 * scalingFactor), "Accumulator Hi:", 15);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(130 * scalingFactor), "VCO Lo:", 7);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(158 * scalingFactor), "VCO Hi:", 7);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(186 * scalingFactor), "VCC Lo:", 7);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(214 * scalingFactor), "VCC Hi:", 7);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(46 * scalingFactor), "Flag Lo:", 8);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(74 * scalingFactor), "Flag Hi:", 8);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(102 * scalingFactor), "Loop:", 5);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(130 * scalingFactor), "VCE Lo:", 7);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(158 * scalingFactor), "VCE Hi:", 7);

    SelectObject(ps.hdc, hOldFont);
    SetBkMode(ps.hdc, OldBkMode);
    EndPaint(hWnd, &ps);
}

void PaintRSP_CP0Panel(HWND hWnd)
{
    PAINTSTRUCT ps;
    RECT rcBox;
    HFONT hOldFont;
    int OldBkMode;
    BeginPaint(hWnd, &ps);

    // Get the current DPI
    UINT dpi = GetDpiForWindow(hWnd);
    float scalingFactor = (float)dpi / 96;

    rcBox.left = (int)(41 * scalingFactor);
    rcBox.top = (int)(29 * scalingFactor);
    rcBox.right = (int)(573 * scalingFactor);
    rcBox.bottom = (int)(275 * scalingFactor);
    DrawEdge(ps.hdc, &rcBox, EDGE_ETCHED, BF_RECT);

    hOldFont = (HFONT)SelectObject(ps.hdc,
                                   GetStockObject(DEFAULT_GUI_FONT));
    OldBkMode = SetBkMode(ps.hdc, TRANSPARENT);

    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(46 * scalingFactor), "SP_MEM_ADDR_REG:", 16);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(74 * scalingFactor), "SP_DRAM_ADDR_REG:", 17);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(102 * scalingFactor), "SP_RD_LEN_REG:", 14);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(130 * scalingFactor), "SP_WR_LEN_REG:", 14);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(158 * scalingFactor), "SP_STATUS_REG:", 14);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(186 * scalingFactor), "SP_DMA_FULL_REG:", 16);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(214 * scalingFactor), "SP_DMA_BUSY_REG:", 16);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(242 * scalingFactor), "SP_SEMAPHORE_REG:", 17);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(46 * scalingFactor), "DPC_START_REG:", 14);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(74 * scalingFactor), "DPC_END_REG:", 12);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(102 * scalingFactor), "DPC_CURRENT_REG:", 16);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(130 * scalingFactor), "DPC_STATUS_REG:", 15);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(158 * scalingFactor), "DPC_CLOCK_REG:", 14);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(186 * scalingFactor), "DPC_BUFBUSY_REG:", 16);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(214 * scalingFactor), "DPC_PIPEBUSY_REG:", 17);
    TextOutA(ps.hdc, (int)(313 * scalingFactor), (int)(242 * scalingFactor), "DPC_TMEM_REG:", 13);

    SelectObject(ps.hdc, hOldFont);
    SetBkMode(ps.hdc, OldBkMode);
    EndPaint(hWnd, &ps);
}

void PaintRSP_GPRPanel(HWND hWnd)
{
    PAINTSTRUCT ps;
    RECT rcBox;
    HFONT hOldFont;
    int OldBkMode;
    BeginPaint(hWnd, &ps);

    // Get the current DPI
    UINT dpi = GetDpiForWindow(hWnd);
    float scalingFactor = (float)dpi / 96;

    rcBox.left = (int)(41 * scalingFactor);
    rcBox.top = (int)(29 * scalingFactor);
    rcBox.right = (int)(573 * scalingFactor);
    rcBox.bottom = (int)(275 * scalingFactor);
    DrawEdge(ps.hdc, &rcBox, EDGE_ETCHED, BF_RECT);

    hOldFont = (HFONT)SelectObject(ps.hdc,
                                   GetStockObject(DEFAULT_GUI_FONT));
    OldBkMode = SetBkMode(ps.hdc, TRANSPARENT);

    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(46 * scalingFactor), "R0 - Reg 0:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(66 * scalingFactor), "AT - Reg 1:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(86 * scalingFactor), "V0 - Reg 2:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(106 * scalingFactor), "V1 - Reg 3:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(126 * scalingFactor), "A0 - Reg 4:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(146 * scalingFactor), "A1 - Reg 5:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(166 * scalingFactor), "A2 - Reg 6:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(186 * scalingFactor), "A3 - Reg 7:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(206 * scalingFactor), "T0 - Reg 8:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(226 * scalingFactor), "T1 - Reg 9:", 11);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(246 * scalingFactor), "T2 - Reg 10:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(46 * scalingFactor), "T3 - Reg 11:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(66 * scalingFactor), "T4 - Reg 12:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(86 * scalingFactor), "T5 - Reg 13:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(106 * scalingFactor), "T6 - Reg 14:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(126 * scalingFactor), "T7 - Reg 15:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(146 * scalingFactor), "S0 - Reg 16:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(166 * scalingFactor), "S1 - Reg 17:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(186 * scalingFactor), "S2 - Reg 18:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(206 * scalingFactor), "S3 - Reg 19:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(226 * scalingFactor), "S4 - Reg 20:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(246 * scalingFactor), "S5 - Reg 21:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(46 * scalingFactor), "S6 - Reg 22:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(66 * scalingFactor), "S7 - Reg 23:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(86 * scalingFactor), "T8 - Reg 24:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(106 * scalingFactor), "T9 - Reg 25:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(126 * scalingFactor), "K0 - Reg 26:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(146 * scalingFactor), "K1 - Reg 27:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(166 * scalingFactor), "GP - Reg 28:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(66 * scalingFactor), "T4 - Reg 12:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(86 * scalingFactor), "T5 - Reg 13:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(106 * scalingFactor), "T6 - Reg 14:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(126 * scalingFactor), "T7 - Reg 15:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(146 * scalingFactor), "S0 - Reg 16:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(166 * scalingFactor), "S1 - Reg 17:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(186 * scalingFactor), "S2 - Reg 18:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(206 * scalingFactor), "S3 - Reg 19:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(226 * scalingFactor), "S4 - Reg 20:", 12);
    TextOutA(ps.hdc, (int)(228 * scalingFactor), (int)(246 * scalingFactor), "S5 - Reg 21:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(46 * scalingFactor), "S6 - Reg 22:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(66 * scalingFactor), "S7 - Reg 23:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(86 * scalingFactor), "T8 - Reg 24:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(106 * scalingFactor), "T9 - Reg 25:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(126 * scalingFactor), "K0 - Reg 26:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(146 * scalingFactor), "K1 - Reg 27:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(166 * scalingFactor), "GP - Reg 28:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(186 * scalingFactor), "SP - Reg 29:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(206 * scalingFactor), "S8 - Reg 30:", 12);
    TextOutA(ps.hdc, (int)(403 * scalingFactor), (int)(226 * scalingFactor), "RA - Reg 31:", 12);

    SelectObject(ps.hdc, hOldFont);
    SetBkMode(ps.hdc, OldBkMode);
    EndPaint(hWnd, &ps);
}

void PaintRSP_Vector1_Panel(HWND hWnd)
{
    PAINTSTRUCT ps;
    RECT rcBox;
    HFONT hOldFont;
    int OldBkMode;
    BeginPaint(hWnd, &ps);

    // Get the current DPI
    UINT dpi = GetDpiForWindow(hWnd);
    float scalingFactor = (float)dpi / 96;

    rcBox.left = (int)(41 * scalingFactor);
    rcBox.top = (int)(29 * scalingFactor);
    rcBox.right = (int)(573 * scalingFactor);
    rcBox.bottom = (int)(275 * scalingFactor);
    DrawEdge(ps.hdc, &rcBox, EDGE_ETCHED, BF_RECT);

    hOldFont = (HFONT)SelectObject(ps.hdc,
                                   GetStockObject(DEFAULT_GUI_FONT));
    OldBkMode = SetBkMode(ps.hdc, TRANSPARENT);

    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(46 * scalingFactor), "$v00:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(74 * scalingFactor), "$v01:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(102 * scalingFactor), "$v02:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(130 * scalingFactor), "$v03:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(158 * scalingFactor), "$v04:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(186 * scalingFactor), "$v05:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(214 * scalingFactor), "$v06:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(242 * scalingFactor), "$v07:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(46 * scalingFactor), "$v08:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(74 * scalingFactor), "$v09:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(102 * scalingFactor), "$v10:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(130 * scalingFactor), "$v11:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(158 * scalingFactor), "$v12:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(186 * scalingFactor), "$v13:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(214 * scalingFactor), "$v14:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(242 * scalingFactor), "$v15:", 5);

    SelectObject(ps.hdc, hOldFont);
    SetBkMode(ps.hdc, OldBkMode);
    EndPaint(hWnd, &ps);
}

void PaintRSP_Vector2_Panel(HWND hWnd)
{
    PAINTSTRUCT ps;
    RECT rcBox;
    HFONT hOldFont;
    int OldBkMode;
    BeginPaint(hWnd, &ps);

    // Get the current DPI
    UINT dpi = GetDpiForWindow(hWnd);
    float scalingFactor = (float)dpi / 96;

    rcBox.left = (int)(41 * scalingFactor);
    rcBox.top = (int)(29 * scalingFactor);
    rcBox.right = (int)(573 * scalingFactor);
    rcBox.bottom = (int)(275 * scalingFactor);
    DrawEdge(ps.hdc, &rcBox, EDGE_ETCHED, BF_RECT);

    hOldFont = (HFONT)SelectObject(ps.hdc,
                                   GetStockObject(DEFAULT_GUI_FONT));
    OldBkMode = SetBkMode(ps.hdc, TRANSPARENT);

    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(46 * scalingFactor), "$v16:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(74 * scalingFactor), "$v17:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(102 * scalingFactor), "$v18:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(130 * scalingFactor), "$v19:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(158 * scalingFactor), "$v20:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(186 * scalingFactor), "$v21:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(214 * scalingFactor), "$v22:", 5);
    TextOutA(ps.hdc, (int)(53 * scalingFactor), (int)(242 * scalingFactor), "$v23:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(46 * scalingFactor), "$v24:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(74 * scalingFactor), "$v25:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(102 * scalingFactor), "$v26:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(130 * scalingFactor), "$v27:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(158 * scalingFactor), "$v28:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(186 * scalingFactor), "$v29:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(214 * scalingFactor), "$v30:", 5);
    TextOutA(ps.hdc, (int)(343 * scalingFactor), (int)(242 * scalingFactor), "$v31:", 5);

    SelectObject(ps.hdc, hOldFont);
    SetBkMode(ps.hdc, OldBkMode);
    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK RefreshRSP_RegProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int nSel;
    TC_ITEM item;

    switch (uMsg)
    {
    case WM_PAINT:
        nSel = TabCtrl_GetCurSel(hTab);
        if (nSel > -1)
        {
            item.mask = TCIF_PARAM;
            TabCtrl_GetItem(hTab, nSel, &item);
            switch (item.lParam)
            {
            case GeneralPurpose:
                PaintRSP_GPRPanel(hWnd);
                break;
            case ControlProcessor0:
                PaintRSP_CP0Panel(hWnd);
                break;
            case HiddenRegisters:
                PaintRSP_HiddenPanel(hWnd);
                break;
            case Vector1:
                PaintRSP_Vector1_Panel(hWnd);
                break;
            case Vector2:
                PaintRSP_Vector2_Panel(hWnd);
                break;
            }
        }
        break;
    }

    return CallWindowProc(RefreshProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK RSP_Registers_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static RECT rcDisp;
    static int CurrentPanel = GeneralPurpose;
    TC_ITEM item;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        RSP_Registers_hDlg = hDlg;
        SetupRSP_RegistersMain(hDlg);
        break;
    case WM_MOVE:
        // StoreCurrentWinPos("RSP Registers",hDlg);
        break;
    case WM_SIZE:
        GetClientRect(hDlg, &rcDisp);
        TabCtrl_AdjustRect(hTab, false, &rcDisp);
        break;
    case WM_NOTIFY:
        switch (((NMHDR *)lParam)->code)
        {
        case TCN_SELCHANGE:
            InvalidateRect(hTab, &rcDisp, true);
            HideRSP_RegisterPanel(CurrentPanel);
            item.mask = TCIF_PARAM;
            TabCtrl_GetItem(hTab, TabCtrl_GetCurSel(hTab), &item);
            CurrentPanel = (int)item.lParam;
            InvalidateRect(hStatic, NULL, false);
            UpdateRSPRegistersScreen();
            ShowRSP_RegisterPanel(CurrentPanel);
            break;
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            CurrentPanel = GeneralPurpose;
            EndDialog(hDlg, IDCANCEL);
            break;
        }
    default:
        return false;
    }
    return true;
}

void SetupRSP_HiddenPanel(HWND hDlg)
{
    // Get the current DPI
    UINT dpi = GetDpiForWindow(hDlg);
    float scalingFactor = (float)dpi / 96;

    int count;

    for (count = 0; count < 8; count++)
    {
        hHIDDEN[count] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(225 * scalingFactor), (int)((count * 28 + 53) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                         hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hHIDDEN[count], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
    for (count = 0; count < 4; count++)
    {
        hHIDDEN[count + 8] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(485 * scalingFactor), (int)((count * 28 + 53) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                             hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hHIDDEN[count + 8], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
}

void SetupRSP_CP0Panel(HWND hDlg)
{
    // Get the current DPI
    UINT dpi = GetDpiForWindow(hDlg);
    float scalingFactor = (float)dpi / 96;

    int count;

    for (count = 0; count < 8; count++)
    {
        hCP0[count] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(225 * scalingFactor), (int)((count * 28 + 53) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                      hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hCP0[count], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
    for (count = 0; count < 8; count++)
    {
        hCP0[count + 8] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(485 * scalingFactor), (int)((count * 28 + 53) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                          hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hCP0[count + 8], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
}

void SetupRSP_GPRPanel(HWND hDlg)
{
    // Get the current DPI
    UINT dpi = GetDpiForWindow(hDlg);
    float scalingFactor = (float)dpi / 96;

    int count;

    for (count = 0; count < 11; count++)
    {
        hGPR[count] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(135 * scalingFactor), (int)((count * 20 + 50) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                      hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hGPR[count], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
    for (count = 0; count < 11; count++)
    {
        hGPR[count + 11] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(315 * scalingFactor), (int)((count * 20 + 50) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                           hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hGPR[count + 11], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
    for (count = 0; count < 10; count++)
    {
        hGPR[count + 22] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(485 * scalingFactor), (int)((count * 20 + 50) * scalingFactor), (int)(75 * scalingFactor), (int)(19 * scalingFactor),
                                           hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hGPR[count + 22], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
}

void SetupRSP_RegistersMain(HWND hDlg)
{
    // Get the current DPI
    UINT dpi = GetDpiForWindow(hDlg);
    float scalingFactor = (float)dpi / 96;

    // Scale the window size
    int WindowWidth = (int)(630 * scalingFactor);
    int WindowHeight = (int)(325 * scalingFactor);
    DWORD X, Y;

    hTab = CreateWindowExA(0, WC_TABCONTROLA, "", WS_TABSTOP | WS_CHILD | WS_VISIBLE, 5, 6, (int)(616 * scalingFactor), (int)(290 * scalingFactor),
                           hDlg, (HMENU)IDC_TAB_CONTROL, (HINSTANCE)hinstDLL, NULL);
    if (hTab)
    {
        TC_ITEMA item;
        SendMessage(hTab, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
        item.mask = TCIF_TEXT | TCIF_PARAM;
        item.pszText = " General Purpose ";
        item.lParam = GeneralPurpose;
        (int)::SendMessageA(hTab, TCM_INSERTITEMA, (WPARAM)(int)(0), (LPARAM)(const TC_ITEMA *)(&item));
        item.lParam = ControlProcessor0;
        item.pszText = " Control Processor 0 ";
        (int)::SendMessageA(hTab, TCM_INSERTITEMA, (WPARAM)(int)(1), (LPARAM)(const TC_ITEMA *)(&item));
        item.lParam = HiddenRegisters;
        item.pszText = " Hidden Registers ";
        (int)::SendMessageA(hTab, TCM_INSERTITEMA, (WPARAM)(int)(2), (LPARAM)(const TC_ITEMA *)(&item));
        item.lParam = Vector1;
        item.pszText = " RSP Vectors $v0 - $v15 ";
        (int)::SendMessageA(hTab, TCM_INSERTITEMA, (WPARAM)(int)(3), (LPARAM)(const TC_ITEMA *)(&item));
        item.lParam = Vector2;
        item.pszText = " RSP Vectors $v16 - $v31 ";
        (int)::SendMessageA(hTab, TCM_INSERTITEMA, (WPARAM)(int)(4), (LPARAM)(const TC_ITEMA *)(&item));
    }

    SetupRSP_HiddenPanel(hDlg);
    SetupRSP_CP0Panel(hDlg);
    SetupRSP_GPRPanel(hDlg);
    SetupRSP_Vect1Panel(hDlg);
    SetupRSP_Vect2Panel(hDlg);

    hStatic = CreateWindowExA(0, "STATIC", "", WS_CHILD | WS_VISIBLE, 5, 6, (int)(616 * scalingFactor), (int)(290 * scalingFactor), hDlg, 0, (HINSTANCE)hinstDLL, NULL);
#ifdef _M_IX86
    RefreshProc = (WNDPROC)SetWindowLong(hStatic, GWL_WNDPROC, (long)RefreshRSP_RegProc);
#else
    DebugBreak();
#endif

    UpdateRSPRegistersScreen();
    ShowRSP_RegisterPanel(GeneralPurpose);
    SetWindowTextA(hDlg, " RSP Registers");

    // if (!GetStoredWinPos("RSP Registers", &X, &Y)) {
    X = (GetSystemMetrics(SM_CXSCREEN) - WindowWidth) / 2;
    Y = (GetSystemMetrics(SM_CYSCREEN) - WindowHeight) / 2;
    //}
    SetWindowPos(hDlg, NULL, X, Y, WindowWidth, WindowHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void SetupRSP_Vect1Panel(HWND hDlg)
{
    // Get the current DPI
    UINT dpi = GetDpiForWindow(hDlg);
    float scalingFactor = (float)dpi / 96;

    int count;

    for (count = 0; count < 8; count++)
    {
        hVECT1[count] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(55 * scalingFactor), (int)((count * 28 + 52) * scalingFactor), (int)(254 * scalingFactor), (int)(19 * scalingFactor),
                                        hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hVECT1[count], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
    for (count = 0; count < 8; count++)
    {
        hVECT1[count + 8] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(345 * scalingFactor), (int)((count * 28 + 52) * scalingFactor), (int)(254 * scalingFactor), (int)(19 * scalingFactor),
                                            hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hVECT1[count + 8], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
}

void SetupRSP_Vect2Panel(HWND hDlg)
{
    // Get the current DPI
    UINT dpi = GetDpiForWindow(hDlg);
    float scalingFactor = (float)dpi / 96;

    int count;

    for (count = 0; count < 8; count++)
    {
        hVECT2[count] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(55 * scalingFactor), (int)((count * 28 + 52) * scalingFactor), (int)(254 * scalingFactor), (int)(19 * scalingFactor),
                                        hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hVECT2[count], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
    for (count = 0; count < 8; count++)
    {
        hVECT2[count + 8] = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | ES_READONLY | WS_BORDER | WS_TABSTOP, (int)(345 * scalingFactor), (int)((count * 28 + 52) * scalingFactor), (int)(254 * scalingFactor), (int)(19 * scalingFactor),
                                            hDlg, 0, (HINSTANCE)hinstDLL, NULL);
        SendMessage(hVECT2[count + 8], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
    }
}

void ShowRSP_RegisterPanel(int Panel)
{
    int count;

    switch (Panel)
    {
    case GeneralPurpose:
        for (count = 0; count < 32; count++)
        {
            ShowWindow(hGPR[count], true);
        }
        break;
    case ControlProcessor0:
        for (count = 0; count < 16; count++)
        {
            ShowWindow(hCP0[count], true);
        }
        break;
    case HiddenRegisters:
        for (count = 0; count < 12; count++)
        {
            ShowWindow(hHIDDEN[count], true);
        }
        break;
    case Vector1:
        for (count = 0; count < 16; count++)
        {
            ShowWindow(hVECT1[count], true);
        }
        break;
    case Vector2:
        for (count = 0; count < 16; count++)
        {
            ShowWindow(hVECT2[count], true);
        }
        break;
    }
}

void UpdateRSPRegistersScreen(void)
{
    char RegisterValue[100];
    int count, nSel;
    TC_ITEM item;

    if (!InRSPRegisterWindow)
    {
        return;
    }
    nSel = TabCtrl_GetCurSel(hTab);
    if (nSel > -1)
    {
        item.mask = TCIF_PARAM;
        TabCtrl_GetItem(hTab, nSel, &item);
        switch (item.lParam)
        {
        case GeneralPurpose:
            for (count = 0; count < 32; count++)
            {
                sprintf(RegisterValue, " 0x%08X", RSP_GPR[count].UW);
                SetWindowTextA(hGPR[count], RegisterValue);
            }
            break;
        case ControlProcessor0:
            if (RSPInfo.SP_MEM_ADDR_REG)
            {
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_MEM_ADDR_REG);
                SetWindowTextA(hCP0[0], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_DRAM_ADDR_REG);
                SetWindowTextA(hCP0[1], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_RD_LEN_REG);
                SetWindowTextA(hCP0[2], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_WR_LEN_REG);
                SetWindowTextA(hCP0[3], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_STATUS_REG);
                SetWindowTextA(hCP0[4], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_DMA_FULL_REG);
                SetWindowTextA(hCP0[5], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_DMA_BUSY_REG);
                SetWindowTextA(hCP0[6], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.SP_SEMAPHORE_REG);
                SetWindowTextA(hCP0[7], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_START_REG);
                SetWindowTextA(hCP0[8], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_END_REG);
                SetWindowTextA(hCP0[9], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_CURRENT_REG);
                SetWindowTextA(hCP0[10], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_STATUS_REG);
                SetWindowTextA(hCP0[11], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_CLOCK_REG);
                SetWindowTextA(hCP0[12], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_BUFBUSY_REG);
                SetWindowTextA(hCP0[13], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_PIPEBUSY_REG);
                SetWindowTextA(hCP0[14], RegisterValue);
                sprintf(RegisterValue, " 0x%08X", *RSPInfo.DPC_TMEM_REG);
                SetWindowTextA(hCP0[15], RegisterValue);
            }
            break;
        case HiddenRegisters:
            for (count = 0; count < 8; count++)
            {
                sprintf(RegisterValue, " 0x%08X - %08X", RSP_ACCUM[count].W[1], RSP_ACCUM[count].W[0]);
                SetWindowTextA(hHIDDEN[count], RegisterValue);
            }
            for (count = 0; count < 3; count++)
            {
                sprintf(RegisterValue, " 0x%04X", RSP_Flags[count].UHW[0]);
                SetWindowTextA(hHIDDEN[count + 8], RegisterValue);
            }
            sprintf(RegisterValue, " 0x%04X", RSP_Flags[2].UHW[0]);
            SetWindowTextA(hHIDDEN[11], RegisterValue);
            break;
        case Vector1:
            for (count = 0; count < 16; count++)
            {
                sprintf(RegisterValue, " 0x%08X - %08X - %08X - %08X", RSP_Vect[count].s32(0),
                        RSP_Vect[count].s32(1), RSP_Vect[count].s32(2), RSP_Vect[count].s32(3));
                SetWindowTextA(hVECT1[count], RegisterValue);
            }
            break;
        case Vector2:
            for (count = 0; count < 16; count++)
            {
                sprintf(RegisterValue, " 0x%08X - %08X - %08X - %08X", RSP_Vect[count + 16].s32(0),
                        RSP_Vect[count + 16].s32(1), RSP_Vect[count + 16].s32(2), RSP_Vect[count + 16].s32(3));
                SetWindowTextA(hVECT2[count], RegisterValue);
            }
            break;
        }
    }
}
