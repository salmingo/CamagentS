/*!
 * @file AstroDeviceDef.h �������ѧ������Զ���۲���ص�״̬��ָ����͵���صĳ���
 * @version 0.1
 * @date 2017-11-24
 * @version 0.2
 * @date 2020-11-01
 * @note
 * - ���¶���enum�������ƺ���ֵ
 * - ���¶���enum���Ͷ�Ӧ�ַ����Ļ�ȡ��ʽ
 */

#ifndef ASTRO_DEVICE_DEFINE_H_
#define ASTRO_DEVICE_DEFINE_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

 /////////////////////////////////////////////////////////////////////////////
 /*!
  * @brief ���繤��������, �ն��豸�Ĺ���
  */
enum {///< ��Ӧ��������
	PEER_CLIENT,		///< �ͻ���
	PEER_MOUNT,			///< GWAC��Զ��
	PEER_CAMERA,		///< ���
	PEER_MOUNT_ANNEX,	///< ����+����+�촰��
	PEER_CAMERA_ANNEX,	///< ����+������
	PEER_LAST		///< ռλ, ��ʹ��
};

/* ״̬��ָ�� */
/////////////////////////////////////////////////////////////////////////////
static const char* netdev_desc[] = {
	"kv",
	"non-kv"
};

/*!
 * @class TypeNetworkDevice
 * @breif ����: �����豸����
 */
class TypeNetworkDevice {
public:
	enum {///< ����ϵ����
		NETDEV_MIN = -1,
		NETDEV_KV,		///< ��ֵ������ͨ��Э��������豸
		NETDEV_NONKV,	///< �Ǽ�ֵ������ͨ��Э��������豸
		NETDEV_MAX
	};

public:
	static bool IsValid(int type) {
		return type > NETDEV_MIN && type < NETDEV_MAX;
	}

	static const char* ToString(int type) {
		return IsValid(type) ? netdev_desc[type] : NULL;
	}

	static int FromString(const char* name) {
		int type(NETDEV_MIN);

		if (name) {
			if (isdigit(name[0])) {
				type = atoi(name);
			}
			else {
				for (type = NETDEV_MIN + 1; type < NETDEV_MAX && strcmp(name, netdev_desc[type]); ++type);
			}
		}
		return IsValid(type) ? type : NETDEV_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* coorsys_desc[] = {
	"AltAzimuth",
	"Equatorial",
	"TwoLineElement"
};

/*!
 * @class TypeCoorSys
 * @breif ����: ����ϵ����
 */
class TypeCoorSys {
public:
	enum {///< ����ϵ����
		COORSYS_MIN = -1,
		COORSYS_ALTAZ,	///< ��ƽϵ
		COORSYS_EQUA,	///< ���ϵ
		COORSYS_ORBIT,	///< ����
		COORSYS_MAX
	};

public:
	static bool IsValid(int type) {
		return type > COORSYS_MIN && type < COORSYS_MAX;
	}

	static const char* ToString(int type) {
		return IsValid(type) ? coorsys_desc[type] : NULL;
	}

	static int FromString(const char* name) {
		int type(COORSYS_MIN);

		if (name) {
			if (isdigit(name[0])) {
				type = atoi(name);
			}
			else {
				for (type = COORSYS_MIN + 1; type < COORSYS_MAX && strcmp(name, coorsys_desc[type]); ++type);
			}
		}
		return IsValid(type) ? type : COORSYS_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* mount_desc[] = {
	"Error",
	"Freeze",
	"Homing",
	"Homed",
	"Parking",
	"Parked",
	"Slewing",
	"Tracking"
};

/*!
 * @class StateMount
 * @brief ����: ת̨״̬
 */
class StateMount {
public:
	enum {// ת̨״̬
		MOUNT_MIN = -1,
		MOUNT_ERROR,	///< ����
		MOUNT_FREEZE,	///< ��ֹ
		MOUNT_HOMING,	///< ����
		MOUNT_HOMED,	///< �ҵ����
		MOUNT_PARKING,	///< ��λ
		MOUNT_PARKED,	///< �Ѹ�λ
		MOUNT_SLEWING,	///< ָ��
		MOUNT_TRACKING,	///< ����
		MOUNT_MAX
	};

public:
	static bool IsValid(int state) {
		return state > MOUNT_MIN && state < MOUNT_MAX;
	}

	static const char* ToString(int state) {
		return IsValid(state) ? mount_desc[state] : NULL;
	}

	static int FromString(const char* name) {
		int state(MOUNT_MIN);

		if (name) {
			if (isdigit(name[0])) {
				state = atoi(name);
			}
			else {
				for (state = MOUNT_MIN + 1; state < MOUNT_MAX && strcmp(name, mount_desc[state]); ++state);
			}
		}
		return IsValid(state) ? state : MOUNT_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const TCHAR* slitc_desc[] = {
	_T("�ر�"),
	_T("��"),
	_T("ֹͣ")
};

class CommandSlit {
public:
	enum {// ����ָ��
		SLITC_MIN = -1,
		SLITC_CLOSE,	///< �ر�
		SLITC_OPEN,	///< ��
		SLITC_STOP,	///< ֹͣ
		SLITC_MAX
	};

public:
	static bool IsValid(int cmd) {
		return cmd > SLITC_MIN && cmd < SLITC_MAX;
	}

	static const TCHAR* ToString(int cmd) {
		return IsValid(cmd) ? slitc_desc[cmd] : NULL;
	}

	static int FromString(const TCHAR* name) {
		int cmd(SLITC_MIN);

		if (name) {
			if (_istdigit(name[0])) {
				cmd = _ttoi(name);
			}
			else {
				for (cmd = SLITC_MIN + 1; cmd < SLITC_MAX && _tcscmp(name, slitc_desc[cmd]); ++cmd);
			}
		}
		return IsValid(cmd) ? cmd : SLITC_MIN;
	}
};

static const TCHAR* slit_desc[] = {
	_T("��    ��"),
	_T("���ڴ�"),
	_T("��    ��"),
	_T("��ȫ��"),
	_T("���ڹر�"),
	_T("��    ��")
};

class StateSlit {
public:
	enum {// ����ָ��
		SLIT_MIN = -1,
		SLIT_ERROR,		///< ����
		SLIT_OPENING,	///< ����
		SLIT_OPEN,		///< �Ѵ�
		SLIT_FULLY_OPEN,///< ��ȫ��
		SLIT_CLOSING,	///< �ر���
		SLIT_CLOSED,	///< �ѹر�
		SLIT_MAX
	};

public:
	static bool IsValid(int state) {
		return state > SLIT_MIN && state < SLIT_MAX;
	}

	static const TCHAR* ToString(int state) {
		return IsValid(state) ? slit_desc[state] : NULL;
	}

	static int FromString(const TCHAR* name) {
		int state(SLIT_MIN);

		if (name) {
			if (_istdigit(name[0])) {
				state = _ttoi(name);
			}
			else {
				for (state = SLIT_MIN + 1; state < SLIT_MAX && _tcscmp(name, slit_desc[state]); ++state);
			}
		}
		return IsValid(state) ? state : SLIT_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* mcc_desc[] = {
	"close",
	"open"
};

/*!
 * @class CommandMirrorCover
 * @brief ����: ����ָ��
 */
class CommandMirrorCover {
public:
	enum {// ����ָ��
		MCC_MIN = -1,
		MCC_CLOSE,	///< �رվ���
		MCC_OPEN,	///< �򿪾���
		MCC_MAX
	};

public:
	static bool IsValid(int cmd) {
		return cmd > MCC_MIN && cmd < MCC_MAX;
	}

	static const char* ToString(int cmd) {
		return IsValid(cmd) ? mcc_desc[cmd] : NULL;
	}

	static int FromString(const char* name) {
		int cmd(MCC_MIN);

		if (name) {
			if (isdigit(name[0])) {
				cmd = atoi(name);
			}
			else {
				for (cmd = MCC_MIN + 1; cmd < MCC_MAX && strcmp(name, mcc_desc[cmd]); ++cmd);
			}
		}
		return IsValid(cmd) ? cmd : MCC_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* mc_desc[] = {
	"Error",
	"Opening",
	"Opened",
	"Closing",
	"Closed"
};

/*!
 * @class StateMirrorCover
 * @brief ����: ����״̬
 */
class StateMirrorCover {
public:
	enum {///< ����״̬
		MC_MIN = -1,
		MC_ERROR,	///< ����
		MC_OPENING,	///< ���ڴ�
		MC_OPEN,	///< �Ѵ�
		MC_CLOSING,	///< ���ڹر�
		MC_CLOSED,	///< �ѹر�
		MC_MAX
	};

public:
	static bool IsValid(int state) {
		return state > MC_MIN && state < MC_MAX;
	}

	static const char* ToString(int state) {
		return IsValid(state) ? mc_desc[state] : NULL;
	}

	static int FromString(const char* name) {
		int state(MC_MIN);

		if (name) {
			if (isdigit(name[0])) {
				state = atoi(name);
			}
			else {
				for (state = MC_MIN + 1; state < MC_MAX && strcmp(name, mc_desc[state]); ++state);
			}
		}
		return IsValid(state) ? state : MC_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const TCHAR* focus_desc[] = {
	_T("Error"),
	_T("Freeze"),
	_T("Moving")
};

/*!
 * @class StateFocus
 * @brief ����������״̬
 */
class StateFocus {
public:
	enum {///< ������״̬
		FOCUS_MIN = -1,
		FOCUS_ERROR,	///< ����
		FOCUS_FREEZE,	///< ��ֹ
		FOCUS_MOVING,	///< ��λ
		FOCUS_MAX
	};

public:
	static bool IsValid(int state) {
		return state > FOCUS_MIN && state < FOCUS_MAX;
	}

	static const TCHAR* ToString(int state) {
		return IsValid(state) ? focus_desc[state] : NULL;
	}

	static int FromString(const TCHAR* name) {
		int state(FOCUS_MIN);

		if (name) {
			if (isdigit(name[0])) {
				state = _ttoi(name);
			}
			else {
				for (state = FOCUS_MIN + 1; state < FOCUS_MAX && _tcscmp(name, focus_desc[state]); ++state);
			}
		}
		return IsValid(state) ? state : FOCUS_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* imgtyp_desc[] = {
	"BIAS",
	"DARK"
	"FLAT",
	"OBJECT"
	"LIGHT",
	"FOCUS"
};

/*!
 * @class TypeImage ����: ͼ������
 */
class TypeImage {
public:
	enum {///< ͼ������
		IMGTYP_MIN = -1,
		IMGTYP_BIAS,	///< ����
		IMGTYP_DARK,	///< ����
		IMGTYP_FLAT,	///< ƽ��
		IMGTYP_OBJECT,	///< Ŀ��
		IMGTYP_LIGHT,	///< ���
		IMGTYP_FOCUS,	///< ����
		IMGTYP_MAX
	};

public:
	static bool IsValid(int type) {
		return type > IMGTYP_MIN && type < IMGTYP_MAX;
	}

	static const char* ToString(int type) {
		return IsValid(type) ? imgtyp_desc[type] : NULL;
	}

	static int FromString(const char* name) {
		int type(IMGTYP_MIN);

		if (name) {
			if (isdigit(name[0])) {
				type = atoi(name);
			}
			else {
				for (type = IMGTYP_MIN + 1; type < IMGTYP_MAX && strcmp(name, imgtyp_desc[type]); ++type);
			}
		}
		return IsValid(type) ? type : IMGTYP_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* exp_desc[] = {
	"start",
	"stop",
	"pause",
};

/*!
 * @class CommandExpose
 * @brief ����: ������ƽӿڼ��Ŀ���ָ��
 */
class CommandExpose {
public:
	enum {///< �������ָ��
		EXP_MIN = -1,
		EXP_START,	///< ��ʼ�ع�
		EXP_STOP,	///< ��ֹ�ع�
		EXP_PAUSE,	///< ��ͣ�ع�
		EXP_MAX
	};

public:
	static bool IsValid(int cmd) {
		return cmd > EXP_MIN && cmd < EXP_MAX;
	}

	static const char* ToString(int cmd) {
		return IsValid(cmd) ? exp_desc[cmd] : NULL;
	}

	static int FromString(const char* name) {
		int cmd(EXP_MIN);

		if (name) {
			if (isdigit(name[0])) {
				cmd = atoi(name);
			}
			else {
				for (cmd = EXP_MIN + 1; cmd < EXP_MAX && strcmp(name, exp_desc[cmd]); ++cmd);
			}
		}
		return IsValid(cmd) ? cmd : EXP_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* camctl_desc[] = {
	"Error",
	"Idle",
	"Exposing",
	"Image Ready",
	"Paused",
	"Waiting until Time",
	"Waiting during FLAT"
};

/*!
 * @class StateCameraControl
 * @brief ����: ������ƽӿڹ���״̬
 */
class StateCameraControl {
public:
	enum {///< �������״̬
		CAMCTL_MIN = -1,
		CAMCTL_ERROR,			///< ����
		CAMCTL_IDLE,			///< ����
		CAMCTL_EXPOSING,		///< �ع������
		CAMCTL_IMAGEREADY,		///< ������ع�
		CAMCTL_PAUSEED,			///< ����ͣ�ع�
		CAMCTL_WAITING_TIME,	///< �ȴ��ع�������ʼʱ�䵽��
		CAMCTL_WAITING_FLAT,	///< ƽ����ȴ�--�ȴ�ת̨����ָ��
		CAMCTL_MAX
	};

public:
	static bool IsValid(int state) {
		return state > CAMCTL_MIN && state < CAMCTL_MAX;
	}

	static const char* ToString(int state) {
		return IsValid(state) ? camctl_desc[state] : NULL;
	}

	static int FromString(const char* name) {
		int state(CAMCTL_MIN);

		if (name) {
			if (isdigit(name[0])) {
				state = atoi(name);
			}
			else {
				for (state = CAMCTL_MIN + 1; state < CAMCTL_MAX && strcmp(name, camctl_desc[state]); ++state);
			}
		}
		return IsValid(state) ? state : CAMCTL_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* obsplan_desc[] = {
	"error",
	"cataloged",
	"interrupted",
	"waiting",
	"running",
	"over",
	"abandoned",
	"deleted"
};

/*!
 * class StateObservationPlan
 * @brief ����: �۲�ƻ�����״̬
 */
class StateObservationPlan {
public:
	enum {///< �۲�ƻ�״̬
		OBSPLAN_MIN = -1,
		OBSPLAN_ERROR,		///< ����. ��ָ������plan_snָ��ļƻ�
		OBSPLAN_CATALOGED,	///< ���
		OBSPLAN_INTERRUPTED,///< �ж�
		OBSPLAN_WAITING,	///< �ȴ�ִ��
		OBSPLAN_RUNNING,	///< ִ����
		OBSPLAN_OVER,		///< ���
		OBSPLAN_ABANDONED,	///< �Զ�����
		OBSPLAN_DELETED,	///< �ֶ�ɾ��
		OBSPLAN_MAX
	};

public:
	static bool IsValid(int state) {
		return state > OBSPLAN_MIN && state < OBSPLAN_MAX;
	}

	static const char* ToString(int state) {
		return IsValid(state) ? obsplan_desc[state] : NULL;
	}

	static int FromString(const char* name) {
		int state(OBSPLAN_MIN);

		if (name) {
			if (isdigit(name[0])) {
				state = atoi(name);
			}
			else {
				for (state = OBSPLAN_MIN + 1; state < OBSPLAN_MAX && strcmp(name, obsplan_desc[state]); ++state);
			}
		}
		return IsValid(state) ? state : OBSPLAN_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* odt_desc[] = {
	"daytime",
	"flat",
	"night"
};

/*!
 * @class TypeObservationDuration
 * @brief ����: �۲�ʱ������
 * @note
 * ��ÿ���Ϊ����ʱ��: ����; ҹ��; ƽ��ʱ��(����)
 */
class TypeObservationDuration {
public:
	enum {///< �۲�ʱ�����
		ODT_MIN = -1,
		ODT_DAYTIME,///< ����, ��ִ��BIAS\DARK\FOCUS�ƻ�
		ODT_FLAT,	///< ƽ��, ��ִ��ƽ���ƻ�
		ODT_NIGHT,	///< ҹ��, ��ִ�з�ƽ���ƻ�
		ODT_MAX
	};

public:
	static bool IsValid(int type) {
		return type > ODT_MIN && type < ODT_MAX;
	}

	static const char* ToString(int type) {
		return IsValid(type) ? odt_desc[type] : NULL;
	}

	static int FromString(const char* name) {
		int type(ODT_MIN);

		if (name) {
			if (isdigit(name[0])) {
				type = atoi(name);
			}
			else {
				for (type = ODT_MIN + 1; type < ODT_MAX && strcmp(name, odt_desc[type]); ++type);
			}
		}
		return IsValid(type) ? type : ODT_MIN;
	}
};

/////////////////////////////////////////////////////////////////////////////
static const char* opobs_desc[] = {
	"mount",
	"camera",
	"mount-annex",
	"camera-annex",
	"environment"
};

/*!
 * @class ObservationOperator
 * @brief ����: ���Ĺ۲�ָ��ִ�ж���
 */
class ObservationOperator {
public:
	enum {///< ����ִ�л���
		OPOBS_MIN = -1,
		OPOBS_MOUNT,		///< ת̨
		OPOBS_CAMERA,		///< ���
		OPOBS_MOUNTANNEX,	///< ת̨����
		OPOBS_CAMERAANNEX,	///< �������
		OPOBS_ENVIRONMENT,	///< �������
		OPOBS_MAX
	};

public:
	static bool IsValid(int type) {
		return type > OPOBS_MIN && type < OPOBS_MAX;
	}

	static const char* ToString(int type) {
		return IsValid(type) ? opobs_desc[type] : NULL;
	}

	static int FromString(const char* name) {
		int type(OPOBS_MIN);
		if (name) {
			if (isdigit(name[0])) {
				type = atoi(name);
			}
			else {
				for (type = OPOBS_MIN + 1; type < OPOBS_MAX && strcmp(name, opobs_desc[type]); ++type);
			}
		}
		return IsValid(type) ? type : OPOBS_MIN;
	}
};

#endif
