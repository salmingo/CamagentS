// DlgFocus.cpp: 实现文件
//

#include "stdafx.h"
#include "CamagentS.h"
#include "DlgFocus.h"
#include "afxdialogex.h"

// CDlgFocus 对话框

IMPLEMENT_DYNAMIC(CDlgFocus, CDialogEx)

CDlgFocus::CDlgFocus(ParamFocuser* param, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FOCUS, pParent)
	, m_min(param->limitMin)
	, m_max(param->limitMax)
{
	param_ = param;
}

CDlgFocus::~CDlgFocus()
{
}

void CDlgFocus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_MIN, m_min);
	DDX_Text(pDX, IDC_ED_MAX, m_max);
}

BEGIN_MESSAGE_MAP(CDlgFocus, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CDlgFocus 消息处理程序
void CDlgFocus::OnOK() {

}

void CDlgFocus::OnCancel() {

}

void CDlgFocus::OnClose()
{
	UpdateData(TRUE);

	int n(0);
	if (m_min != param_->limitMin) {
		++n;
		param_->limitMin = m_min;
	}
	if (m_max != param_->limitMax) {
		++n;
		param_->limitMax = m_max;
	}
	if (n) param_->dirty = true;

	CDialogEx::OnClose();
}
