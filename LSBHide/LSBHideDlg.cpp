
// LSBHideDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSBHide.h"
#include "LSBHideDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLSBHideDlg �Ի���



CLSBHideDlg::CLSBHideDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LSBHIDE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLSBHideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SRC_IMG, m_srcBMP);
	DDX_Control(pDX, IDC_DST_IMG, m_dstBMP);
}

BEGIN_MESSAGE_MAP(CLSBHideDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CLSBHideDlg::OnReadClick)
	ON_COMMAND(ID_32772, &CLSBHideDlg::OnSaveClick)
	ON_COMMAND(ID_32773, &CLSBHideDlg::OnHideClick)
	ON_COMMAND(ID_32774, &CLSBHideDlg::OnExtractClick)
END_MESSAGE_MAP()


// CLSBHideDlg ��Ϣ�������

BOOL CLSBHideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_srcMyBMP = NULL;
	m_maxLen = 0;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLSBHideDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLSBHideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLSBHideDlg::OnReadClick()
{
	// TODO: �ڴ���������������
	CString len;
	//���ļ��Ի���
	CString filter = L"BMPͼƬ (*.bmp; *.BMP)|*.bmp;*.BMP||";
	CFileDialog open(TRUE, getWorkDir(), NULL, NULL, filter, NULL);
	INT_PTR result = open.DoModal();
	if (result == IDOK)
	{
		//���ͼƬ
		m_dstBMP.SetBitmap(NULL);

		m_BMPFilePath = open.GetPathName();
		m_BMPFileTitle = open.GetFileTitle();
		m_BMPFileTitle += "_LSB";

		setPicture(IDC_SRC_IMG, m_BMPFilePath, 0);

		const char* p = CStringToChar(m_BMPFilePath);
		m_srcMyBMP = new MyBMP(p);
		m_maxLen = m_srcMyBMP->length();
		len.Format(L"%d", m_srcMyBMP->length());
		GetDlgItem(IDC_TEXT_LENGTH)->SetWindowTextW(len);
	}

	
}
//��ȡ����Ŀ¼
CString CLSBHideDlg::getWorkDir()
{
	LPWSTR path = new WCHAR[MAX_PATH];
	int pos = GetCurrentDirectory(MAX_PATH, path);
	CString csPath = path;
	if (pos < 0)
	{
		return CString("");
	}
	else
	{
		return csPath;
	}

}

//��ʾͼƬ
void CLSBHideDlg::setPicture(int pictureControlID, CString imageFilePath, int pos)
{
	HBITMAP hbitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(),
		imageFilePath,
		IMAGE_BITMAP,
		340,
		420,
		LR_LOADFROMFILE);
	if (pos == 0) 
	{
		m_srcBMP.SetBitmap(hbitmap);
	}
	else
	{
		m_dstBMP.SetBitmap(hbitmap);
	}
}


void CLSBHideDlg::OnSaveClick()
{
	// TODO: �ڴ���������������
	if (m_srcMyBMP != NULL) {
		CString filter = L"BMPͼƬ (*.bmp)|*.bmp||";
		CFileDialog open(FALSE, getWorkDir(), m_BMPFileTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
		INT_PTR result = open.DoModal();
		CString filepath;
		if (result == IDOK)
		{
			filepath = open.GetPathName();
			//����Ƕ���ͼƬ
			const char* pStr = CStringToChar(filepath);

			m_srcMyBMP->save(pStr);
		}
	}
	
}


void CLSBHideDlg::OnHideClick()
{
	// TODO: �ڴ���������������
	if (m_srcMyBMP != NULL) {
		CString hideData;
		GetDlgItem(IDC_EDIT_INFO)->GetWindowText(hideData);

		int len = WideCharToMultiByte(CP_ACP, 0, hideData, -1, NULL, 0, NULL, NULL);
		if (len < m_maxLen)
		{
			const char* pStr = CStringToChar(hideData);

			m_srcMyBMP->savelsb(pStr);

			CString bmpPath = getWorkDir();
			bmpPath += "\\test.bmp";

			const char* pPath = CStringToChar(bmpPath);

			m_srcMyBMP->save(pPath);

			setPicture(IDC_DST_IMG, bmpPath, 1);
		}
		else 
		{
			MessageBox(L"�������Ϣ�����˷�Χ��");
		}
		
	}
}


void CLSBHideDlg::OnExtractClick()
{
	// TODO: �ڴ���������������
	if (m_srcMyBMP != NULL) {
		CString text(m_srcMyBMP->readlsb());
		GetDlgItem(IDC_EDIT_INFO)->SetWindowTextW(text);
	}
}

char* CLSBHideDlg::CStringToChar(CString csStr)
{
	int i = csStr.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, csStr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, csStr, -1, str, len, NULL, NULL);
	return str;
}
