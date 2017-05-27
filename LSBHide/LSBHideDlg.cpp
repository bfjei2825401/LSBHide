
// LSBHideDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LSBHide.h"
#include "LSBHideDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLSBHideDlg 对话框



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


// CLSBHideDlg 消息处理程序

BOOL CLSBHideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_srcMyBMP = NULL;
	m_maxLen = 0;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLSBHideDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLSBHideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLSBHideDlg::OnReadClick()
{
	// TODO: 在此添加命令处理程序代码
	CString len;
	//打开文件对话框
	CString filter = L"BMP图片 (*.bmp; *.BMP)|*.bmp;*.BMP||";
	CFileDialog open(TRUE, getWorkDir(), NULL, NULL, filter, NULL);
	INT_PTR result = open.DoModal();
	if (result == IDOK)
	{
		//清空图片
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
//获取工作目录
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

//显示图片
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
	// TODO: 在此添加命令处理程序代码
	if (m_srcMyBMP != NULL) {
		CString filter = L"BMP图片 (*.bmp)|*.bmp||";
		CFileDialog open(FALSE, getWorkDir(), m_BMPFileTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
		INT_PTR result = open.DoModal();
		CString filepath;
		if (result == IDOK)
		{
			filepath = open.GetPathName();
			//保存嵌入的图片
			const char* pStr = CStringToChar(filepath);

			m_srcMyBMP->save(pStr);
		}
	}
	
}


void CLSBHideDlg::OnHideClick()
{
	// TODO: 在此添加命令处理程序代码
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
			MessageBox(L"输入的信息超出了范围！");
		}
		
	}
}


void CLSBHideDlg::OnExtractClick()
{
	// TODO: 在此添加命令处理程序代码
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
