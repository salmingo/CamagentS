/*!
 * @class CPLC ��װPLCͨ�Žӿ�, ʵ����Բ��+��ˮ�Ŀ���ָ���״̬���
 * @version 0.1
 * @author ¬����
 * @date 2021-10-03
 * @note
 * �����б�:
 * 1. ά�����ڿ��غͲ�������
 * 2. ��Ӧ�ϲ�ָ��, ����Բ���촰�����ء�ͣ
 * 3. ��ʱ��ѯ�촰״̬�ͽ�ˮ״̬
 * 4. ���ûص������ӿ�, ֪ͨ�ϲ㹤��״̬�ı仯
 */
#pragma once

#include "SerialComm.h"

#define PLC_START	0x02
#define PLC_END		0x03
#define PLC_READ	0x30
#define PLC_WRITE	0x31
#define PLC_ON		0x37
#define PLC_OFF		0x38
#define PLC_SUCCESS	0x06
#define PLC_FAIL	0x15
#define PLC_DATA	0x02
#define PLC_DATA_0	0x10	// �յ���ʼ��, �����ݲ���ȫ

class CPLC
{
public:
	CPLC();
	virtual ~CPLC();

protected:
	// ��������
	typedef boost::shared_array<char> carray;		//< �ַ�������

protected:
	// ��Ա����
	SerialPtr serialPort_;	/// ���ڽӿ�
	unsigned char retCode_;	/// PLC����д�뷴��
	carray bufRcv_;		/// ���ڽ������ݴ洢��, ����������У����
	int dataLen_;		/// ���ڽ������ݴ洢������

public:
	// �ӿ�
	/*!
	 * @brief ���ô��ڲ���
	 * @param portName  ��������
	 * @param baudRate  ������
	 * @param dataBit   ����λ����. 7��8
	 * @param parity    ����ģʽ. 0: ��У��; 1: ��; 2: ż
	 * @param stopBit   ֹͣλ����. 0: 1; 1: 1.5; 2: 2
	 * @param flowCtl   ��������ģʽ. 0: �޿���; 1: ��; 2: Ӳ
	 */
	virtual bool SetSerialPort(const char* portName, int baudRate = 9600, int dataBit = 8, int parity = 0, int stopBit = 1, int flowCtl = 0);
	/*!
	 *@brief ��Ծ��־
	 */
	bool IsActive();
	/*!
	 * @brief ǿ�ƽӵ�ON
	 */
	bool SwitchOnOff(WORD addr, bool onoff = true);
	/*!
	 * @brief ���Դ�ָ����ַ��ȡָ�����ȵ�����
	 */
	bool Read(WORD addr, unsigned char len);
	/*!
	 * @brief ���Խ�ָ�����͵�����д��ָ����ַ 
	 */
	template<typename T> bool Write(WORD addr, T data) {
		if (!IsActive())
			return false;

		int lenType = int(sizeof(T));
		int i;
		unsigned char* dataPtr = ((unsigned char*)&data) + lenType - 1;
		carray buff;
		char * toWrite, * ptr;

		n = 11 + lenType * 2;
		buff.reset(new char[n]);
		toWrite = buff.get();

		toWrite[0] = PLC_START;
		toWrite[1] = PLC_WRITE;
		char2code(unsigned char(HIBYTE(addr)), toWrite + 2);
		char2code(unsigned char(LOBYTE(addr)), toWrite + 4);
		char2code(lenType, toWrite + 6);
		for (i = 0, ptr = toWrite + 8; i < lenType; ++i, ptr += 2, --dataPtr) {
			char2code(*dataPtr, ptr);
		}
		toWrite[lenType * 2 + 8] = PLC_END;
		check_sum(toWrite, 1, lenType * 2 + 8, toWrite + lenType * 2 + 9);
		serialPort_->Write(toWrite, n);

		return true;
	}

protected:
	/*!
	 * @brief ��16������ֵת��ΪPLC����
	 */
	void char2code(unsigned char value, char* code);
	unsigned char code2char(char* code);
	/*!
	 * @brief ����У���
	 */
	void check_sum(const char* data, const int first, const int last, char* crc);
	// ����: ����ͨ��
	virtual void serial_read(SerialPtr ptr, const boost::system::error_code& ec);
};
