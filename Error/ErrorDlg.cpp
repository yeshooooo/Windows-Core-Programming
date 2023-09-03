
// ErrorDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Error.h"
#include "ErrorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CErrorDlg dialog



CErrorDlg::CErrorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ERROR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_value);
}

BEGIN_MESSAGE_MAP(CErrorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CErrorDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CErrorDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CErrorDlg message handlers

BOOL CErrorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 初始化
	this->SetDlgItemTextW(IDC_STATIC1, TEXT("请输入错误代码"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CErrorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CErrorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CErrorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CErrorDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CErrorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	// GetLastError获取的错误代码有可能是普通的函数，也可能是windows api的错误
	// 有些函数不是windows的函数如socket的，有可能获取不到

	// 定义一个接受其他类型错误
	DWORD count = 0;

	
	TCHAR* str = NULL;
	this->UpdateData(1);
	// 新版MFC不能直接改变量类型为int了，这里先拿到文本框字符串，然后转成DWORD传给FormatMessage函数使用
	CString value;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(value);
	DWORD errCode = _tcstoul(value, NULL, 10);
	// FORMAT_MESSAGE_ALLOCATE_BUFFER 这个参数可以自动分配内存，所以第6个参数可以写0
	count = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, errCode, NULL, (LPWSTR)&str, 0, NULL);

	// 成功
	if (count)
	{
		this->SetDlgItemTextW(IDC_STATIC1, str);

		// 释放FORMAT_MESSAGE_ALLOCATE_BUFFER自动分配的内存
		LocalFree(str);
	}
	// 失败
	else
	{
		// 失败的话不从系统中获取，从hmodule中获取,如这里从装入的socket dll中获取
		HMODULE hmodule = LoadLibrary(L"msgnet.dll"); //装入网络消息的dll中获取
		if (hmodule) // 如果成功的话
		{
			count = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS
				| FORMAT_MESSAGE_ALLOCATE_BUFFER,
				hmodule, errCode, NULL, (LPWSTR)&str, 0, NULL);
			if (count)
			{
				this->SetDlgItemTextW(IDC_STATIC1, str);

				// 释放FORMAT_MESSAGE_ALLOCATE_BUFFER自动分配的内存
				LocalFree(str);
			}
			// 调用成功了才释放，否则不释放
			FreeLibrary(hmodule);
		}
	}
	if (count == 0)
	{
		this->SetDlgItemTextW(IDC_STATIC1, (LPCTSTR)L"没用找到错误代码信息");
	}
	
}
