
// LSBHideDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MyBMP.h"

// CLSBHideDlg �Ի���
class CLSBHideDlg : public CDialogEx
{
// ����
public:
	CLSBHideDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LSBHIDE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
