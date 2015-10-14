#ifndef _COHANDWRITE_H_
#define _COHANDWRITE_H_

#define WM_HANDWRITE WM_USER + 11
#define WM_SETRECT	 WM_USER + 12
#define WM_SETMODE	 WM_USER + 13
#define WM_CLEARSTROKE WM_USER + 14
#define WM_STARTRECOG WM_USER + 15
#define WM_GETSIGN    WM_USER + 16
#define WM_SETCOLOR  WM_USER + 17

#define MODE_HW_AUTO		0  // 自动手写识别模式（默认）
#define MODE_HW_MANUAL		1  // 手动手写识别模式
#define MODE_SIGN			2  // 签名模式


#define BLACK_IDX 0
#define RED_IDX 1
#define GREEN_IDX 2
#define YELLOW_IDX 3
#define BLUE_IDX 4
#define MAGENTA_IDX 5
#define CYAN_IDX 6
#define GRAY_IDX 7
#define LTGRAY_IDX 8
#define LTRED_IDX 9
#define LTGREEN_IDX 10
#define LTYELLOW_IDX 11
#define LTBLUE_IDX 12
#define LTMAGENTA_IDX 13
#define LTCYAN_IDX 14
#define LTWHITE_IDX 15
/*
 * 打开手写功能
 * @param
 * hWnd[in]: 传入接收手写识别结果目标窗口句柄
 */
void	WINAPI HandWriteStartUp(HWND hWnd);

/*
 * 关闭手写功能
 */
void	WINAPI HandWriteStop();	

/*
 * 设置手写输入范围
 * @param
 * pRect[in]: 传入手写输入范围，坐标以桌面(0, 0)点为基准
 */
void	WINAPI HandWriteSetRect(RECT* pRect);

/*
 * 设置手写模式
 * @param
 * nMode[in]: 需设置的手写模式
 */
void	WINAPI HandWriteSetMode(int nMode);
/*
 * 设置手写笔迹颜色
 * colorIndex 颜色索引
 * isColor 是否为彩色
 */
void    WINAPI HandWriteSetColor(int colorIndex, BOOL isColor);

/*
 * 启动手动手写识别
 */
void	WINAPI HandWriteStartRecognize();

/*
 * 获取签名图片
 * @param
 * pWidth[out]: 获取到签名图片的宽度
 * pHeight[out]: 获取到签名图片的高度
 * @return
 * HBITMAP: 返回签名图片句柄
 */
HBITMAP	WINAPI HandWriteGetSignBitmap(int* pWidth, int* pHeight);

/*
 * 清除手写笔迹
 */
void	WINAPI HandWriteClearStroke();

#endif