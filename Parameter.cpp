/*!
* @struct Parameter 使用XML文件管理配置参数
*/
#include "stdafx.h"
#include "Parameter.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost;
using namespace property_tree;

Parameter::Parameter() {
	slitOpenPeriod = slitOpenPeriodOld = 1;
}

BOOL Parameter::Load(const char* filePath) {
	this->filePath = filePath;

	try {
		ptree pt;
		read_xml(filePath, pt, xml_parser::trim_whitespace);

		BOOST_FOREACH(ptree::value_type &x, pt.get_child("")) {
			if (iequals(x.first, "SerialComm")) {
				string location = x.second.get("<xmlattr>.Location", "");
				ParamSerialComm* param = location == "Inner" ? &serialCommInner : &serialCommOuter;
				param->portName = x.second.get("<xmlattr>.Name", "COM1");
				param->baudRate = x.second.get("Baudrate.<xmlattr>.Value", 9600);
				param->dataBit = x.second.get("DataBit.<xmlattr>.Value", 8);
				param->parity = x.second.get("Parity.<xmlattr>.Option", 0);
				param->stopBit = x.second.get("StopBit.<xmlattr>.Option", 0);
				param->flowCtl = x.second.get("FlowCtl.<xmlattr>.Option", 0);
				param->enabled = x.second.get("Rainfall.<xmlattr>.Enabled", FALSE);
			}
			else if (iequals(x.first, "DomeSlit")) {
				slitOpenPeriod = x.second.get("<xmlattr>.OpenPeriod", 3);
				slitOpenPeriodOld = slitOpenPeriod;
			}
			else if (iequals(x.first, "NetworkGtoaes")) {
				netGtoaes.enabled = x.second.get("<xmlattr>.Enabled", FALSE);
				netGtoaes.gid = x.second.get("ID.<xmlattr>.Group", "001");
				netGtoaes.uid = x.second.get("ID.<xmlattr>.Unit", "001");
				netGtoaes.cid = x.second.get("ID.<xmlattr>.Camera", "011");
				netGtoaes.ip = x.second.get("Server.<xmlattr>.IP", "192.168.1.2");
				netGtoaes.portCamera = x.second.get("Port.<xmlattr>.Camera", 4012);
				netGtoaes.portAnnex = x.second.get("Port.<xmlattr>.Annex", 4013);
			}
			else if (iequals(x.first, "NetworkFileServer")) {
				netFileSrv.enabled = x.second.get("<xmlattr>.Enabled", FALSE);
				netFileSrv.ip = x.second.get("Server.<xmlattr>.IP", "192.168.1.2");
				netFileSrv.port = x.second.get("Server.<xmlattr>.Port", 4020);
			}
			else if (iequals(x.first, "NetworkNTP")) {
				netNTP.enabled = x.second.get("<xmlattr>.Enabled", FALSE);
				netNTP.ip = x.second.get("<xmlattr>.IP", "192.168.1.4");
			}
			else if (iequals(x.first, "Focuser")) {
				focuser.offset = x.second.get("Position.<xmlattr>.Offset", 0);
				focuser.lastPos = x.second.get("Position.<xmlattr>.Last", 0);
				focuser.limitMin = x.second.get("Limit.<xmlattr>.Min", 0);
				focuser.limitMax = x.second.get("Limit.<xmlattr>.Max", 0);
				focuser.factor = x.second.get("Factor.<xmlattr>.Value", 1);
				if (focuser.factor <= 0) focuser.factor = 1;
			}
		}
		return TRUE;
	}
	catch (xml_parser_error &) {
		Init();
		return FALSE;
	}
}

void Parameter::Save() {
	BOOL dirty = serialCommInner.dirty || serialCommOuter.dirty
		|| slitOpenPeriod != slitOpenPeriodOld
		|| netGtoaes.dirty || netFileSrv.dirty || netNTP.dirty;

	if (dirty) {
		try {
			ptree pt;
			read_xml(filePath, pt, xml_parser::trim_whitespace);

			if (serialCommInner.dirty) {// 更新串口参数: 室内
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "SerialComm") && x.second.get("<xmlattr>.Location", "") == "Inner") {
						x.second.put("<xmlattr>.Name", serialCommInner.portName);
						x.second.put("Baudrate.<xmlattr>.Value", serialCommInner.baudRate);
						x.second.put("DataBit.<xmlattr>.Value", serialCommInner.dataBit);
						x.second.put("Parity.<xmlattr>.Option", serialCommInner.parity);
						x.second.put("StopBit.<xmlattr>.Option", serialCommInner.stopBit);
						x.second.put("FlowCtl.<xmlattr>.Option", serialCommInner.flowCtl);
						x.second.put("Rainfall.<xmlattr>.Enabled", serialCommInner.enabled);
						serialCommInner.dirty = FALSE;

						break;
					}
				}
			}

			if (serialCommOuter.dirty) {// 更新串口参数: 室外
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "SerialComm") && x.second.get("<xmlattr>.Location", "") == "Outer") {
						x.second.put("<xmlattr>.Name", serialCommOuter.portName);
						x.second.put("Baudrate.<xmlattr>.Value", serialCommOuter.baudRate);
						x.second.put("DataBit.<xmlattr>.Value", serialCommOuter.dataBit);
						x.second.put("Parity.<xmlattr>.Option", serialCommOuter.parity);
						x.second.put("StopBit.<xmlattr>.Option", serialCommOuter.stopBit);
						x.second.put("FlowCtl.<xmlattr>.Option", serialCommOuter.flowCtl);
						x.second.put("Rainfall.<xmlattr>.Enabled", serialCommOuter.enabled);
						serialCommOuter.dirty = FALSE;
						break;
					}
				}
			}

			if (slitOpenPeriod != slitOpenPeriodOld) {
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "DomeSlit")) {
						x.second.put("<xmlattr>.OpenPeriod", slitOpenPeriod);
						slitOpenPeriodOld = slitOpenPeriod;
						break;
					}
				}
			}

			if (netGtoaes.dirty) {
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "NetworkGtoaes")) {
						x.second.put("<xmlattr>.Enabled", netGtoaes.enabled);
						x.second.put("ID.<xmlattr>.Group", netGtoaes.gid);
						x.second.put("ID.<xmlattr>.Unit", netGtoaes.uid);
						x.second.put("ID.<xmlattr>.Camera", netGtoaes.cid);
						x.second.put("Server.<xmlattr>.IP", netGtoaes.ip);
						x.second.put("Port.<xmlattr>.Camera", netGtoaes.portCamera);
						x.second.put("Port.<xmlattr>.Annex", netGtoaes.portAnnex);
						netGtoaes.dirty = FALSE;
						break;
					}
				}
			}

			if (netFileSrv.dirty) {
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "NetworkFileServer")) {
						x.second.put("<xmlattr>.Enabled", netFileSrv.enabled);
						x.second.put("Server.<xmlattr>.IP", netFileSrv.ip);
						x.second.put("Server.<xmlattr>.Port", netFileSrv.port);
						netFileSrv.dirty = FALSE;
						break;
					}
				}
			}

			if (netNTP.dirty) {
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "NetworkNTP")) {
						x.second.put("<xmlattr>.Enabled", netNTP.enabled);
						x.second.put("<xmlattr>.IP", netNTP.ip);
						netNTP.dirty = FALSE;
						break;
					}
				}
			}

			if (focuser.dirty) {
				BOOST_FOREACH(ptree::value_type & x, pt.get_child("")) {
					if (iequals(x.first, "Focuser")) {
						x.second.put("Position.<xmlattr>.Offset", focuser.offset);
						x.second.put("Position.<xmlattr>.Last", focuser.lastPos);
						x.second.put("Limit.<xmlattr>.Min", focuser.limitMin);
						x.second.put("Limit.<xmlattr>.Max", focuser.limitMax);
						focuser.dirty = FALSE;
						break;
					}
				}
			}

			xml_writer_settings<string> settings(' ', 4);
			write_xml(filePath, pt, std::locale(), settings);
		}
		catch (xml_parser_error &) {
			TRACE(_T("failed to save configuration parameters"));
		}
	}
}

void Parameter::Init() {
	try {
		ptree pt;

		// 室内串口
		ptree& node_sci = pt.add("SerialComm", "");
		node_sci.add("<xmlattr>.Location", "Inner");
		node_sci.add("<xmlattr>.Name", serialCommInner.portName = "COM1");
		node_sci.add("Baudrate.<xmlattr>.Value", serialCommInner.baudRate = 9600);
		node_sci.add("DataBit.<xmlattr>.Value", serialCommInner.dataBit = 7);
		node_sci.add("Parity.<xmlattr>.Option", serialCommInner.parity = 2);
		node_sci.add("StopBit.<xmlattr>.Option", serialCommInner.stopBit = 0);
		node_sci.add("FlowCtl.<xmlattr>.Option", serialCommInner.flowCtl = 0);
		node_sci.add("Rainfall.<xmlattr>.Enabled", serialCommInner.enabled = TRUE);
		// 室外串口
		ptree& node_sco = pt.add("SerialComm", "");
		node_sco.add("<xmlattr>.Location", "Outer");
		node_sco.add("<xmlattr>.Name", serialCommOuter.portName = "COM2");
		node_sco.add("Baudrate.<xmlattr>.Value", serialCommOuter.baudRate = 9600);
		node_sco.add("DataBit.<xmlattr>.Value", serialCommOuter.dataBit = 7);
		node_sco.add("Parity.<xmlattr>.Option", serialCommOuter.parity = 2);
		node_sco.add("StopBit.<xmlattr>.Option", serialCommOuter.stopBit = 0);
		node_sco.add("FlowCtl.<xmlattr>.Option", serialCommOuter.flowCtl = 0);
		node_sco.add("Rainfall.<xmlattr>.Enabled", serialCommOuter.enabled = FALSE);
		// 天窗定时打开周期
		ptree& slit = pt.add("DomeSlit", "");
		slit.add("<xmlattr>.OpenPeriod", slitOpenPeriod = slitOpenPeriodOld = 3);
		// 网络参数: 调度服务
		ptree& node_gtoaes = pt.add("NetworkGtoaes", "");
		node_gtoaes.add("<xmlattr>.Enabled", netGtoaes.enabled = FALSE);
		node_gtoaes.add("ID.<xmlattr>.Group", netGtoaes.gid = "001");
		node_gtoaes.add("ID.<xmlattr>.Unit", netGtoaes.uid = "001");
		node_gtoaes.add("ID.<xmlattr>.Camera", netGtoaes.cid = "011");
		node_gtoaes.add("Server.<xmlattr>.IP", netGtoaes.ip = "192.168.1.2");
		node_gtoaes.add("Port.<xmlattr>.Camera", netGtoaes.portCamera = 4012);
		node_gtoaes.add("Port.<xmlattr>.Annex", netGtoaes.portAnnex = 4013);
		// 网络参数: 文件服务
		ptree& node_fs = pt.add("NetworkFileServer", "");
		node_fs.add("<xmlattr>.Enabled", netFileSrv.enabled = FALSE);
		node_fs.add("Server.<xmlattr>.IP", netFileSrv.ip = "192.168.1.2");
		node_fs.add("Server.<xmlattr>.Port", netFileSrv.port = 4020);
		// 网络参数: 时间服务
		ptree& node_ntp = pt.add("NetworkNTP", "");
		node_ntp.add("<xmlattr>.Enabled", netNTP.enabled = FALSE);
		node_ntp.add("<xmlattr>.IP", netNTP.ip = "192.168.1.4");
		// 调焦
		ptree& node_focus = pt.add("Focuser", "");
		node_focus.add("Position.<xmlattr>.Offset", focuser.offset = 0);
		node_focus.add("Position.<xmlattr>.Last", focuser.lastPos = 0);
		node_focus.add("Limit.<xmlattr>.Min", focuser.limitMin = 0);
		node_focus.add("Limit.<xmlattr>.Max", focuser.limitMax = 0);
		node_focus.add("Factor.<xmlattr>.Value", focuser.factor = 1);

		xml_writer_settings<string> settings(' ', 4);
		write_xml(filePath, pt, std::locale(), settings);
	}
	catch (xml_parser_error&) {
	}
}
