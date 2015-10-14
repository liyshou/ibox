#pragma once

#define DEVICE_LV1000R			1 //һά��ɨ����
#define DEVICE_LV420R			2 //2ά��ɨ����

#define MAX_RESPONSE_LENGTH 1024

#define WORKMODE_AUTO				1 //�Զ�ʶ��ģʽ
#define WORKMODE_INTERMITTENCE		2 //��Ъʶ��ģʽ
#define WORKMODE_INDUCTION			3 //��Ӧʶ��ģʽ
#define WORKMODE_CONTINUATION		4 //����ʶ��ģʽ
#define WORKMODE_DELAY_INDUCTION	5 //�ӳ�ʽ��Ӧʶ��ģʽ
#define WORKMODE_COMMAND			6 //�����ʶ��ģʽ

#define LOOP_RUNNING		0
#define LOOP_PAUSE			1
#define LOOP_END			2
#define LOOP_WAIT_PAUSE		3
#define	LOOP_WAIT_CONTINUE	4
#define LOOP_WAIT_END		5

#define ERR_UNKNOWN					-1
#define ERR_OK						0
#define ERR_UNKNOWN_DEVICE			1
#define ERR_INVALID_PORT			2
#define ERR_ALREADY_SET_TYPE		3	//�ظ������豸����
#define ERR_EXECUTE					4	//ִ��ʧ��
#define ERR_COM						5	//���ڴ�
#define ERR_ABORT					6	//�ж�
#define ERR_COMMUNICATION			7	//ͨѶʧ��
#define ERR_PARAMETER				8	//��������
#define ERR_UNSUPPORT				9	//�����ִ��
#define ERR_CARDTYPE				10	//����Ŀ�
#define ERR_DLL						11	//���ض�̬��ʧ��
#define ERR_DLLDIRECTORY			12	//���ö�̬��Ŀ¼ʧ��
#define ERR_INOPERATE				13	//ִ����һ��������
#define ERR_TIMEOUT					14	//��ʱ
#define ERR_BUFFER_SMALL			15	//������̫С
#define ERR_VARIFY					16	//����У��ʧ��
#define ERR_LRC						17	//У��LRCʧ��