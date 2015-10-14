#ifndef _COHANDWRITE_H_
#define _COHANDWRITE_H_

#define WM_HANDWRITE WM_USER + 11
#define WM_SETRECT	 WM_USER + 12
#define WM_SETMODE	 WM_USER + 13
#define WM_CLEARSTROKE WM_USER + 14
#define WM_STARTRECOG WM_USER + 15
#define WM_GETSIGN    WM_USER + 16
#define WM_SETCOLOR  WM_USER + 17

#define MODE_HW_AUTO		0  // �Զ���дʶ��ģʽ��Ĭ�ϣ�
#define MODE_HW_MANUAL		1  // �ֶ���дʶ��ģʽ
#define MODE_SIGN			2  // ǩ��ģʽ


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
 * ����д����
 * @param
 * hWnd[in]: ���������дʶ����Ŀ�괰�ھ��
 */
void	WINAPI HandWriteStartUp(HWND hWnd);

/*
 * �ر���д����
 */
void	WINAPI HandWriteStop();	

/*
 * ������д���뷶Χ
 * @param
 * pRect[in]: ������д���뷶Χ������������(0, 0)��Ϊ��׼
 */
void	WINAPI HandWriteSetRect(RECT* pRect);

/*
 * ������дģʽ
 * @param
 * nMode[in]: �����õ���дģʽ
 */
void	WINAPI HandWriteSetMode(int nMode);
/*
 * ������д�ʼ���ɫ
 * colorIndex ��ɫ����
 * isColor �Ƿ�Ϊ��ɫ
 */
void    WINAPI HandWriteSetColor(int colorIndex, BOOL isColor);

/*
 * �����ֶ���дʶ��
 */
void	WINAPI HandWriteStartRecognize();

/*
 * ��ȡǩ��ͼƬ
 * @param
 * pWidth[out]: ��ȡ��ǩ��ͼƬ�Ŀ��
 * pHeight[out]: ��ȡ��ǩ��ͼƬ�ĸ߶�
 * @return
 * HBITMAP: ����ǩ��ͼƬ���
 */
HBITMAP	WINAPI HandWriteGetSignBitmap(int* pWidth, int* pHeight);

/*
 * �����д�ʼ�
 */
void	WINAPI HandWriteClearStroke();

#endif