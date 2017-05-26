
// LSBHideDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MyBMP.h"

// CLSBHideDlg 对话框
class CLSBHideDlg : public CDialogEx
{
// 构造
public:
	CLSBHideDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LSBHIDE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnReadClick();
private:
	CString getWorkDir();
	void setPicture(int pictureControlID, CString imageFilePath, int pos);
	char* CStringToChar(CString csStr);
	CStatic m_srcBMP;
	CStatic m_dstBMP;
	CString m_BMPFileTitle;
	CString m_BMPFilePath;
	int m_maxLen;
	MyBMP* m_srcMyBMP;
public:
	afx_msg void OnSaveClick();
	afx_msg void OnHideClick();
	afx_msg void OnExtractClick();
};
