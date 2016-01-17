// LeftClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "LeftClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftClientDlg dialog

#define SAFEFREE_HANDLE(p) if(p) {CloseHandle(p); p=NULL;}


CLeftClientDlg::CLeftClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLeftClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeftClientDlg)
	//}}AFX_DATA_INIT

    m_hBjThreadRun = NULL;
	m_weiboname=m_weibonum = m_fllowed = m_fllowing = m_wbuserid = "";
	i_m_fllowed = 0;
    i_m_fllowing = 0;
}


void CLeftClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftClientDlg)
	DDX_Control(pDX, IDC_BUTTON_FANLI, m_BtnFanLi);
	DDX_Control(pDX, IDC_STATIC_DETAIL, m_DetailText);
	DDX_Control(pDX, IDC_BUTTON_QUN2, m_QunBtn2);
	DDX_Control(pDX, IDC_BUTTON_QUN3, m_QunBtn3);
	DDX_Control(pDX, IDC_BUTTON_QUN1, m_QunBtn1);
	DDX_Control(pDX, IDC_BUTTON_HF, m_HfBtn);
	DDX_Control(pDX, IDC_BUTTON_QUESTION, m_questionBtn);
	DDX_Control(pDX, IDC_BUTTON_END, m_wbEndBtn);
	DDX_Control(pDX, IDC_BUTTON_STAR, m_wbStarBtn);
	DDX_Control(pDX, IDC_BUTTON_WEIBO_LOGON, m_wblogBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftClientDlg, CDialog)
	//{{AFX_MSG_MAP(CLeftClientDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_WEIBO_LOGON, OnButtonWeiboLogon)
	ON_BN_CLICKED(IDC_BUTTON_STAR, OnButtonStar)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_HF, OnButtonHf)
	ON_BN_CLICKED(IDC_BUTTON_QUN1, OnButtonQun1)
	ON_BN_CLICKED(IDC_BUTTON_QUN2, OnButtonQun2)
	ON_BN_CLICKED(IDC_BUTTON_QUN3, OnButtonQun3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftClientDlg message handlers

void CLeftClientDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient,rcBtn,rcScroll,rcPic;
	GetClientRect(&rcClient);

	//�ڴ滭ͼ//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	szMemDC.Rectangle(rcClient);
//	szMemDC.Draw3dRect(&rcClient,RGB(0,0,0),RGB(255,255,255));

//	szMemDC.FillSolidRect( &rcClient ,RGB(255,255,255));//RGB( 150,150,150));


    szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	
	


	



	//���߿�
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}

//ƽ�̻�λͼ
long CLeftClientDlg::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
{
	CDC MemDC;
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	int li_Width = bm.bmWidth;
	int li_Height = bm.bmHeight;


	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);

	int x=rc.left;
	int y=rc.top;
	while (y < (rc.Height()+rc.top)) 
 	{
 		while(x < (rc.Width()+rc.left)) 
 		{
			pDC->BitBlt(x, y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
 			x += li_Width;
 		}
		x = rc.left;
 		y += li_Height;
 	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();

	return y-rc.top;
}

//����λͼ�ߴ�
void CLeftClientDlg::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}

//������Դλͼ
void CLeftClientDlg::SetBackImage(int index,CString strName)
{
	SetImage(m_ItemBack[index], strName);
	SetImageSize(&m_ItemBack[index], m_szItemArea[index]);
}


//����Ӳ���ļ�λͼ
void CLeftClientDlg::SetImage(CBitmap &bitmap, CString strName)
{
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString str;
		AfxMessageBox(_T("Not Open SetImage Bitmap"));
		PostQuitMessage(0);
	}

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}



BOOL CLeftClientDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();
	
	CPoint szPoint;
	szPoint.x = 20;
	szPoint.y = 5;

	m_questionBtn.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_questionBtn.SetWindowText("����ָ��");
	m_questionBtn.SetTextPos(szPoint);


	m_wbEndBtn.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_wbEndBtn.SetWindowText("���˵����");
	m_wbEndBtn.SetTextPos(szPoint);

	m_wbStarBtn.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_wbStarBtn.SetWindowText("�Զ�����");
	m_wbStarBtn.SetTextPos(szPoint);

	m_wblogBtn.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_wblogBtn.SetWindowText("΢����½");
	m_wblogBtn.SetTextPos(szPoint);

	CIniFile szIniFile;
	//Ĭ������
	CString strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbbindflag"));
	m_wbbindflag = atoi(strValue);
	if(m_wbbindflag == 1)
	{
		m_wblogBtn.SetWindowText("�Ѿ���");
	}

	//	GetDlgItem(IDC_BUTTON_WEIBO_LOGON)->EnableWindow(FALSE);

	m_QunBtn1.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_QunBtn1.SetWindowText("��ע");
	m_QunBtn1.SetTextPos(szPoint);

	m_QunBtn2.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_QunBtn2.SetWindowText("��˿");
	m_QunBtn2.SetTextPos(szPoint);

	m_QunBtn3.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_QunBtn3.SetWindowText("Χ��");
	m_QunBtn3.SetTextPos(szPoint);


	m_HfBtn.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_HfBtn.SetWindowText("�����ʲ�ѯ");
	m_HfBtn.SetTextPos(szPoint);


	m_BtnFanLi.SetImage(_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"),_T("��ͼ\\��ť.bmp"));
    m_BtnFanLi.SetWindowText("�ҵķ���");
	m_BtnFanLi.SetTextPos(szPoint);
	m_BtnFanLi.ShowWindow(SW_HIDE);//�з����Ż���ʾ
   




	return TRUE;
}









void CLeftClientDlg::OnTimer(UINT nIDEvent) 
{


	CDialog::OnTimer(nIDEvent);
}

void CLeftClientDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	

	

	CDialog::OnMouseMove(nFlags, point);
}


void CLeftClientDlg::MouseSelect( const CPoint & point)
{

	

}


BOOL CLeftClientDlg::MouseLDown( const CPoint & point)
{

	
	return TRUE;

}


BOOL CLeftClientDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
   
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CLeftClientDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	

	CDialog::OnLButtonDown(nFlags, point);
}

void CLeftClientDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CLeftClientDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	
	CDialog::OnLButtonUp(nFlags, point);
}






void CLeftClientDlg::OnButtonWeiboLogon() 
{
	//��weibo
	CIniFile szIniFile;
	//Ĭ������
	CString strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbbindflag"));
	m_wbbindflag = atoi(strValue);
	if(m_wbbindflag == 1)
	{
		m_wblogBtn.SetWindowText("�Ѿ���");
	}
	else
	{
		ShellExecute(NULL,NULL,_T("http://www.wgz360.com/index.php"),NULL,NULL,SW_SHOWNORMAL);
	}

	// TODO: Add your control notification handler code here
//	m_iectrlcur->SetUrl(_T("http://www.wgz360.com/index.php"));
	
}


void CLeftClientDlg::getbddetail(CString url,CString itemkey)
{

	long nLenMax = 0;
	CString examplet = "���Ŵ�Խ��ķ���,���١�������а����������Ů�ǿ�ʼ�����Լ��ܹ���Խ�Żص��Ŵ�����һ�ѡ�������������ô�ࡰ�߿Ƽ����ܴ���δ��,ƫƫ������ôһλ��ŮС�����ǡ����봩Խ��������ƭ!(����)asddddddddddddddddddddddddasdddaŮС�����ǡ�Ů";
    nLenMax = examplet.GetLength();

	CInternetSession mySession(NULL,0);
	CHttpFile* myHttpFile=NULL;

	CString myData,SiteInfo,strFind0,strFind1,strTemp,strRet,strgetRet;
	long nPos1,nPos2;

	BOOL bOpenFile(FALSE);
	try
	{
		myHttpFile=(CHttpFile*)mySession.OpenURL(url);
		bOpenFile = TRUE;
	}
	catch(CInternetException* pEx)
	{
	   myHttpFile->Close();
	   myHttpFile=NULL;//�����������ʱ���󣬸�file����ֵ�����򽫼�������

	   OutputDebugString(_T("Error IE\r\n"));
	   Sleep(500);
	   bOpenFile = FALSE;
	   
	}

	if(bOpenFile)
	{

		while(myHttpFile->ReadString(myData))
		{
			SiteInfo +=  myData;
		}


		//ȥͷ
		strFind0 = "<head>";
		nPos1 = SiteInfo.Find(strFind0);
		SiteInfo = SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strFind0.GetLength());




		strFind0 = "<li class=\"xinwen\">";
		nPos1 = SiteInfo.Find(strFind0);
		SiteInfo = SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strFind0.GetLength());

		strFind0 = "<div class=\"tit\"><a href=\"";
		
		while( (nPos1 = SiteInfo.Find(strFind0)  )!= -1)
		{

			SiteInfo = SiteInfo.Right(SiteInfo.GetLength() - nPos1 - strFind0.GetLength());

			strTemp = SiteInfo;
			strFind0 = "\"";

			if( (nPos1 = strTemp.Find(strFind0)  )!= -1)
			{
					//������
					strgetRet =strTemp.Left( nPos1);
					strgetRet.TrimRight();

					strgetRet.TrimRight();
					strgetRet.TrimLeft();
 
					//������
					strFind0 = "<p>";
					if( (nPos1 = strTemp.Find(strFind0)  )!= -1)
					{
						strTemp = strTemp.Right(strTemp.GetLength() - nPos1 - strFind0.GetLength());

						strFind0 = "</p>";
						if( (nPos1 = strTemp.Find(strFind0)  )!= -1)
						{
							
							
							strRet =strTemp.Left( nPos1);
					        strRet.TrimRight();
							strRet.TrimLeft();

							itemkey.TrimRight();
							itemkey.TrimLeft();
							
                            strRet = " " + itemkey + " " + strRet;//�ӱ���

							if(strRet.GetLength() > nLenMax)
							{
								strRet = strRet.Left(nLenMax);
							}
						

                           

							{
								//�߳����Զ�������
								CString strParam = "";
								//��������
								strParam = "";
								if(strRet.GetLength() > 0)
								{
									strRet += "����:";
									strRet = GetGbkToUrlEncode(strRet);

									strParam = "?text=";
									strParam += strRet;

									strParam += strgetRet;
								}
							  
							    strParam = _T("http://www.wgz360.com/") + strParam;

								m_iectrlcur->SetUrl(strParam);
			

								break;
							}



						}

					}
			}
		}
	}
}


CString CLeftClientDlg::GetGbkToUrlEncode(CString keyWord)
{
	CString strSql,str;
	char *p1 =m_pbuf;
	memset(p1,0x00,sizeof(char)*1024*1024*4);
	wchar_t* wszString1 = m_wszString;
	memset(wszString1,0x00,sizeof(char)*1024*1024*8 + 1);

	strSql = keyWord;

	CString strurldecode = _T("");
	{

		sprintf(p1,"%s",strSql);
		

		int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, p1, strlen(p1), NULL, 0);  
		MultiByteToWideChar(CP_ACP, NULL, p1, strlen(p1), wszString1, wcsLen);
		wszString1[wcsLen] = '\0';  

		memset(p1,0x00,strSql.GetLength()*sizeof(char));
		//UNICODE��ת����UTF-8 ������buf�
		int u8Len = ::WideCharToMultiByte(CP_UTF8, NULL, wszString1, wcslen(wszString1), NULL, 0, NULL, NULL);  
		WideCharToMultiByte(CP_UTF8, NULL, wszString1, wcslen(wszString1), p1, u8Len, NULL, NULL);  
		p1[u8Len] = '\0';  

		long nData[500] = {0};
		long nLen = strlen(p1);
		
		for(long i = 0 ;i < nLen;i++)
		{
			if(p1[i] == 32)
			{//�ո�
			    strurldecode +=_T("%20");
			}
			else if(p1[i] > 0)
			{
				strurldecode += p1[i];
			}
			else
			{
				nData[i] = 256 + (int)p1[i];
	

				str.Format(_T("%x"), nData[i]);
				str = _T("%") + str;
                strurldecode+=str;

				//ת16�����ַ�
			}
			
		}

	}

	return strurldecode;
}

void CLeftClientDlg::getfrombaidu(void)
{

	
	
}


int CLeftClientDlg::bindweibotowgz(void)
{
	//�ȵ�½,ȡ�û���,mima
	CIniFile szIniFile;
	CString email = szIniFile.GetIniFileString(_T("wbidstar"),_T("user"));
    CString pwd = szIniFile.GetIniFileString(_T("wbidstar"),_T("pwd"));

	long nLogTime = 0;
	BOOL bLogStat = FALSE;
	long nFindpos = 0;
	while(nLogTime < 3)
	{
		CString strStarlog = "";
		strStarlog.Format(_T("http://www.wgz360.com/"));
		m_iectrlcur->SetUrl(strStarlog);

		int nTimeget = 0;
		while(1)
		{
			Sleep(500);
			CString strText = m_iectrlcur->GetSataFlag();
			
			if(strText.GetLength() > 0)
			{
				nFindpos = strText.Find(_T("<BODY>"));
				//OutputDebuString(strText); 
				/*CFile file;
                if(file.Open("C:\\555.txt",CFile::modeCreate|CFile::modeReadWrite))
				{
					file.Write(strText,strText.GetLength());
					file.Close();
				}*/

				if(nFindpos > -1)
				{
					strText = strText.Right(strText.GetLength() - nFindpos - strlen("<BODY>"));

					
					nFindpos = strText.Find(_T("</BODY>"));
					if(nFindpos > -1)
					{
						strText = strText.Left(nFindpos);
                        if(strText.Compare("1")==0)
						{
							//��½�ɹ�
							bLogStat = TRUE;//��½�ɹ�
							return 1;
					
						}
						else
						{
							return 2;//δ���ʺ�,�˳� AfxMessageBox("δ�������ʺŵ�������,�˳�,˵ϸ��鿴\"���ʹ��˵��\"��ϵQQ:181300289!");
						}
					}

				}
				
			}

		
			nTimeget++;
			if(nTimeget > 30)
			{
				//return 3;//AfxMessageBox("��ʱ,���µ�½");
				break;
			}

		}


		nLogTime++;

		if(nLogTime > 3)
		{
			return 3;  //AfxMessageBox("��ʱ,���µ�½,�˳��߳�!");
		}
	}

	return 4;//δ֪����
}


int CLeftClientDlg::getweibostat(void)
{//ȡ��״̬�Ƿ�����,sesson�Ƿ�ʧ������
	long nLogTime = 0;
	BOOL bLogStat = FALSE;
	long nFindpos = 0;
	while(nLogTime < 3)
	{
		m_iectrlcur->SetUrl("http://www.wgz360.com");

		int nTimeget = 0;
		while(1)
		{
			Sleep(500);

			CString strText = m_iectrlcur->GetSataFlag();
			if(strText.GetLength() > 0)
			{
				nFindpos = strText.Find(_T("<RET>"));
				//OutputDebuString(strText); 
				CFile file;
                if(file.Open("C:\\888.txt",CFile::modeCreate|CFile::modeReadWrite))
				{
					file.Write(strText,strText.GetLength());
					file.Close();
				}

				if(nFindpos > -1)
				{
					strText = strText.Right(strText.GetLength() - nFindpos - strlen("<RET>"));

					
					nFindpos = strText.Find(_T("</RET>"));
					if(nFindpos > -1)
					{
						long nLastfllowed=atoi(m_fllowed.GetBuffer(0));
						long nLastflloweind=atoi(m_fllowing.GetBuffer(0));
                        m_fllowed.ReleaseBuffer(m_fllowed.GetLength());
						m_fllowing.ReleaseBuffer(m_fllowing.GetLength());
						i_m_fllowed = nLastfllowed;
                        i_m_fllowing = nLastflloweind;

						BOOL bRightFlag = FALSE;
						nFindpos = strText.Find(_T(","));
						if(nFindpos > -1)
						{
							bRightFlag = TRUE;
							m_wbuserid = strText.Left(nFindpos);//�û�ID
							strText = strText.Right(strText.GetLength() - nFindpos - strlen("<RET>"));
						}

						nFindpos = strText.Find(_T(","));
						if(nFindpos > -1&&bRightFlag==TRUE)
						{
							bRightFlag = TRUE;
							m_weiboname = strText.Left(nFindpos);//�û���
							strText = strText.Right(strText.GetLength() - nFindpos - strlen(","));
						}

						nFindpos = strText.Find(_T(","));
						if(nFindpos > -1&&bRightFlag==TRUE)
						{
							bRightFlag = TRUE;
							m_fllowing = strText.Left(nFindpos);//��ע��
							strText = strText.Right(strText.GetLength() - nFindpos - strlen(","));
						}

						nFindpos = strText.Find(_T(","));
						if(nFindpos > -1&&bRightFlag==TRUE)
						{
							bRightFlag = TRUE;
							m_fllowed = strText.Left(nFindpos);//��˿��
							strText = strText.Right(strText.GetLength() - nFindpos - strlen(","));
						}

						nFindpos = strText.Find(_T(","));
						if(nFindpos > -1&&bRightFlag==TRUE)
						{
							bRightFlag = TRUE;
							m_weibonum = strText.Left(nFindpos);//΢����
							strText = strText.Right(strText.GetLength() - nFindpos - strlen(","));
						}


						if(bRightFlag==TRUE)
						{

							long nnowfllowed=atoi(m_fllowed.GetBuffer(0));
							long nnowflloweind=atoi(m_fllowing.GetBuffer(0));
							m_fllowed.ReleaseBuffer(m_fllowed.GetLength());
							m_fllowing.ReleaseBuffer(m_fllowing.GetLength());
				
							
							CString strnum = "";

							strnum.Format(_T("��ע%s"),m_fllowed);
							if(nnowfllowed-nLastfllowed>3&&nLastfllowed>0)
							{
								strnum.Format(_T("��ע%s(+%d)"),m_fllowed,nnowfllowed-nLastfllowed);
							}
							m_QunBtn2.SetWindowText(strnum);


							
							strnum.Format(_T("��˿%s"),m_fllowing);
							if(nnowflloweind - nLastflloweind>3&&nLastflloweind>0)
							{
								strnum.Format(_T("��˿%s(+%d)"),m_fllowing,nnowflloweind - nLastflloweind);
							}
							m_QunBtn1.SetWindowText(strnum);

							
							m_QunBtn3.SetWindowText("΢��"+m_weibonum);
							return 1;//����
						}
						else
						{
							return 2;//����
						}

						
					}

				}
				
			}

		
			nTimeget++;
			if(nTimeget > 30)
			{
				break;
			}

		}


		nLogTime++;

		if(nLogTime > 3)
		{
			return 3;  //AfxMessageBox("��ʱ,���µ�½,�˳��߳�!");
		}
	}

	return 4;//δ֪����
}


DWORD WINAPI CLeftClientDlg::StarCovertDbToBjData(LPVOID lpParameter)
{
	CLeftClientDlg* lpWnd=(CLeftClientDlg*)lpParameter;
    //http://weibo.com/u/1656086880
	
	
    
	

	//�߳����Զ�������
	CString text;
	CString strid;
	CString picurl;
	CString temp;
	CString strParam = "";


	CTime StartTime,StartTime1,StartTime2,StartTime3,StartTime4,runTime;
	StartTime = CTime::GetCurrentTime();

	StartTime  = StartTime - CTimeSpan(0,   0,   5,   0  );
     StartTime1 =StartTime2 =StartTime3 =StartTime4 = StartTime;
	int nHfengIndex=0;//���ۼ������
	int nXhIndex=0;//�Ѿ�����Ц������

	BOOL bSatr = FALSE;


	//�ȵ�½,ȡ�û���,mima
	CIniFile szIniFile;


	int nlogret = lpWnd->bindweibotowgz();
	if(nlogret==1)
	{
		//�ɹ�
	}
	else if(nlogret==2)
	{//δ���ʺ�,�˳�
		//δ���ʺ�,�˳�
		AfxMessageBox("δ�������ʺŵ�������,�˳�,˵ϸ��鿴\"���ʹ��˵��\"��ϵQQ:181300289!");
		lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
		SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
		return  0;
	}
	else if(nlogret==3)
	{
		AfxMessageBox("���γ�ʱ,���µ�½,��������,�˳��߳�!");
		lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
		SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
		return  0;
	}
	else if(nlogret==4)
	{
		AfxMessageBox("δ֪����,��ϵQQ: 280888954,181300289!");
		lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
		SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
		return  0;
	}
	


	//�ɹ�logon����ȡ�»�������
	nlogret = lpWnd->getweibostat();
    if(nlogret!=1)
	{
		//ʧ��.�ص�½
		nlogret = lpWnd->bindweibotowgz();
		if(nlogret==1)
		{
			//�ɹ�
		}
		else if(nlogret==2)
		{//δ���ʺ�,�˳�
			//δ���ʺ�,�˳�
			AfxMessageBox("δ�������ʺŵ�������,�˳�,˵ϸ��鿴\"���ʹ��˵��\"��ϵQQ:181300289!");
			lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
			SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
			return  0;
		}
		else if(nlogret==3)
		{
			AfxMessageBox("���γ�ʱ,���µ�½,��������,�˳��߳�!");
			lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
			SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
			return  0;
		}
		else if(nlogret==4)
		{
			AfxMessageBox("δ֪����,��ϵQQ: 280888954,181300289!");
			lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
			SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
			return  0;
		}
	}

	lpWnd->m_szBaiduItems = new BaiduItems[10];
	lpWnd->m_pbuf = new char[1024*1024*4]; // 4M
    lpWnd->m_wszString = new wchar_t[1024*1024*16 + 1];


	lpWnd->getfrombaidu();//ȡ�ٶ�����



	int barraysend[5] = {0};
	int bTotalSend = 0;
	int bhasend = 0;

	BOOL bCheck10= FALSE,bCheck11= FALSE,bCheck12= FALSE,bCheck13= FALSE,bCheck14= FALSE,bCheck15= FALSE,bCheck16= FALSE,bCheck17= FALSE;
	CString strValue;
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag10"));
	if(strValue.Compare("1")==0)
	{//����
		bCheck10 = TRUE;
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag11"));
	if(strValue.Compare("1")==0)
	{//����
		bCheck11 = TRUE;
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag12"));
	if(strValue.Compare("1")==0)
	{//�ٶ�
		bCheck12 = TRUE;
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag13"));
	if(strValue.Compare("1")==0)
	{//Ц��
		bCheck13 = TRUE;
		barraysend[bTotalSend]=0;
		bTotalSend++;
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag14"));
	if(strValue.Compare("1")==0)
	{//΢��
		bCheck14 = TRUE;
		barraysend[bTotalSend]=1;
		bTotalSend++;
	}
	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag15"));
	if(strValue.Compare("1")==0)
	{//��ͼ
		bCheck15 = TRUE;
		barraysend[bTotalSend]=2;
		bTotalSend++;
	}



	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag16"));
	if(strValue.Compare("1")==0)
	{//����΢��¼
		bCheck16 = TRUE;
		barraysend[bTotalSend]=3;
		bTotalSend++;
	}

	strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("setflag17"));
	if(strValue.Compare("1")==0)
	{//����Ӣ��¼
		bCheck17 = TRUE;
		barraysend[bTotalSend]=4;
		bTotalSend++;
	}

	while(1)
	{
        runTime = CTime::GetCurrentTime();

		long nDay = 0;
	    long nhour = 0;
	    long nMin = 10;
	    long nSec = 0;

        //i_m_fllowed = 0;//��˿
        //i_m_fllowing = 0;//��ע
	    if(runTime - CTimeSpan(0,   0,   13,   0  ) > StartTime && bCheck11 == TRUE && (lpWnd->i_m_fllowed < 3500 && lpWnd->i_m_fllowing < 1980))//20����һ�η�����,������ѹ���
		{
			StartTime = runTime;
			CString strParamHfeng = "http://wgz360.com/wbhfeng/sendwb2.php";
			
			lpWnd->m_iectrlcur->SetUrl(strParamHfeng);
	
		    bSatr = TRUE;

		}

		if(runTime - CTimeSpan(0,   0,   20,   0  ) > StartTime1 && bSatr == TRUE&& bCheck10 == TRUE&& (lpWnd->i_m_fllowed < 3500 && lpWnd->i_m_fllowing < 1980))//20�����ڰ�û�л��۵ļ�Ϊ��˿
		{
			StartTime1 = runTime;
			
			
			lpWnd->m_iectrlcur->SetUrl(_T("http://wgz360.com/wbhfeng/wbhufeng.php"));
		
		}

		if(runTime - CTimeSpan(0,   0,   29,   0  ) > StartTime2 && bSatr == TRUE&& bCheck12 == TRUE)//20���ӷ�һ������
		{
			StartTime2 = runTime;

			for(int k = 0 ; k < 10;k++)
			{
				 if(lpWnd->m_szBaiduItems[k].UpLoadFlag == 0)
				 {//��������weibo

					 lpWnd->getbddetail(lpWnd->m_szBaiduItems[k].tempURL,lpWnd->m_szBaiduItems[k].itemname);
					 lpWnd->m_szBaiduItems[k].UpLoadFlag = 1;
					 break;
         
				 }
			}

			
		}

		
        if(runTime - CTimeSpan(0,   0,   6,   0  ) > StartTime3  && bTotalSend > 0)//9���ӷ�һ��Ц��
		{
			//13,14,15 && bCheck13 == TRUE
			//9���ӷ�һ��Ц��,ͼ�������Ķ��п���
			StartTime3 = runTime;


			
			//����
			
			bhasend++;
			if(bhasend >= bTotalSend)
				bhasend = 0.;

		    lpWnd->sendweiboall(barraysend[bhasend]);

			
		}


		if(runTime - CTimeSpan(0,   0,   8,   0  ) > StartTime4  && bTotalSend > 0)//40���Ӽ��һ��״̬�����о��ص�½
		{
	
			StartTime4 = runTime;
			

			//�ɹ�logon����ȡ�»�������
			nlogret = lpWnd->getweibostat();
			if(nlogret!=1)
			{
				//ʧ��.�ص�½
				nlogret = lpWnd->bindweibotowgz();
				if(nlogret==1)
				{
					//�ɹ�
				}
				else if(nlogret==2)
				{//δ���ʺ�,�˳�
					//δ���ʺ�,�˳�
					AfxMessageBox("δ�������ʺŵ�������,�˳�,˵ϸ��鿴\"���ʹ��˵��\"��ϵQQ:181300289!");
					lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
					SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
					return  0;
				}
				else if(nlogret==3)
				{
					AfxMessageBox("���γ�ʱ,���µ�½,��������,�˳��߳�!");
					lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
					SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
					return  0;
				}
				else if(nlogret==4)
				{
					AfxMessageBox("δ֪����,��ϵQQ: 280888954,181300289!");
					lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
					SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
					return  0;
				}
			}


			
		}

		Sleep(10000);
	}

    


   


	if(lpWnd->m_szBaiduItems)
		delete [] lpWnd->m_szBaiduItems;

	if(lpWnd->m_pbuf)
	{
		delete lpWnd->m_pbuf;
		lpWnd->m_pbuf = NULL;
	}

	if(lpWnd->m_wszString)
	{
		delete lpWnd->m_wszString;
		lpWnd->m_wszString = NULL;
	}

	lpWnd->m_wbStarBtn.SetWindowText("�Զ�����");
	SAFEFREE_HANDLE(lpWnd->m_hBjThreadRun);
	return 0;
}

void CLeftClientDlg::UTF_8ToUnicode(wchar_t* pOut,char *pText)
{
    char* uchar = (char *)pOut;
    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
    return;
}


void CLeftClientDlg::UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // ע�� WCHAR�ߵ��ֵ�˳��,���ֽ���ǰ�����ֽ��ں�
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}


void CLeftClientDlg::UTF_8ToGB2312(CString &pOut, char *pText, int pLen)
{

	if(pLen > 0)
	{
		char * newBuf = new char[pLen*2];
		memset(newBuf,0x00,sizeof(char)*pLen*2);
		char Ctemp[4];
		memset(Ctemp,0,4);
		int i =0;
		int j = 0;
    
		while(i < pLen)
		{
			if(pText[i] > 0)
			{
					newBuf[j++] = pText[i++];                       
			}
			else                 
			{
					WCHAR Wtemp;
					UTF_8ToUnicode(&Wtemp,pText + i);
        
					UnicodeToGB2312(Ctemp,Wtemp);
            
					newBuf[j] = Ctemp[0];
					newBuf[j + 1] = Ctemp[1];
					i += 3;    
					j += 2;   
			}
		}
		newBuf[j] = ' ';
    
		pOut = newBuf;

		if(newBuf)
		{
			delete []newBuf;
			newBuf = NULL;
		}
	}
    
    return; 
}  

void CLeftClientDlg::UnicodeToGB2312(char* pOut,wchar_t uData)
{
    WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
    return;
}     

 void CLeftClientDlg::sendweiboall(int nType)
 {

	 CString strParam;
	 if(nType == 0)
	 {
		 strParam.Format(_T("http://www.wgz360.com/"),m_idStar0);
	 }
	 else if(nType == 1)
	 {
		 strParam.Format(_T("http://www.wgz360.com/"),m_idStar1);
	 }
	 else if(nType == 2)
	 {
		 strParam.Format(_T("http://www.wgz360.com/"),m_idStar2);
	 }
	 else if(nType == 3)
	 {
		 strParam.Format(_T("http://www.wgz360.com/"),m_idStar3);
	 }
	 else if(nType == 4)
	 {
		 strParam.Format(_T("http://www.wgz360.com/"),m_idStar4);
	 }

	



	m_iectrlcur->SetUrl(strParam);



	//
    BOOL bRetGet = FALSE;
	int nTimeget = 0;
	int nFindpos = 0;
	int pageGet = 0;
	while(1)
	{
		CString strText = m_iectrlcur->GetSataFlag();
		Sleep(500);
		if(strText.GetLength() > 0)
		{
			nFindpos = strText.Find(_T("<PAGEID>"));
			//OutputDebuString(strText); 
			/*CFile file;
            if(file.Open("C:\\666.txt",CFile::modeCreate|CFile::modeReadWrite))
			{
				file.Write(strText,strText.GetLength());
				file.Close();
			}*/

			if(nFindpos > -1)
			{
				strText = strText.Right(strText.GetLength() - nFindpos - strlen("<PAGEID>"));

				
				nFindpos = strText.Find(_T("</PAGEID>"));
				if(nFindpos > -1)
				{
					strText = strText.Left(nFindpos);
                    
					pageGet  = atoi(strText.GetBuffer(0));
					strText.ReleaseBuffer(strText.GetLength());
					bRetGet = TRUE;
					break;//�ɹ���
				}

			}
			
		}

	
		nTimeget++;
		if(nTimeget > 30)
		{
			//AfxMessageBox("��ʱ,���µ�½");
			break;
		}

	}


	if(bRetGet > 0)
	 {//���ͳɹ�
		 OutputDebugString("����Ц���ɹ�");
		 CIniFile szIniFile;


		 pageGet++; //Ҫ��һ


		 CString SiteInfo;
         SiteInfo.Format(_T("%d"),pageGet);

		if(nType == 0)
		 {
			 m_idStar0 = pageGet;
			 szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail0"),SiteInfo);
		 }
		 else if(nType == 1)
		 {
			 m_idStar1 = pageGet;
			 szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail1"),SiteInfo);
		 }
		 else if(nType == 2)
		 {
			 m_idStar2 = pageGet;
			 szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail2"),SiteInfo);
		 }
		 else if(nType == 3)
		 {
			 m_idStar3 = pageGet;
			 szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail3"),SiteInfo);
		 }
		 else if(nType == 4)
		 {
			 m_idStar4 = pageGet;
			 szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail4"),SiteInfo);
		 }
	}

 }


		
void CLeftClientDlg::OnButtonStar() 
{
	if(m_hBjThreadRun==NULL)
	{
		CIniFile szIniFile;
		//Ĭ������
		CString strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbidstardetail0"));
		m_idStar0 = atoi(strValue);

		

		if(m_idStar0 == 0)
			m_idStar0 = 1;

		strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbidstardetail1"));
		m_idStar1 = atoi(strValue);

		if(m_idStar1 == 0)
			m_idStar1 = 1;

		strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbidstardetail2"));
		m_idStar2 = atoi(strValue);

		if(m_idStar2 == 0)
			m_idStar2 = 1;

		strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbidstardetail3"));
		m_idStar3 = atoi(strValue);

		if(m_idStar3 == 0)
			m_idStar3 = 1;


		strValue = szIniFile.GetIniFileString(_T("wbidstar"),_T("wbidstardetail4"));
		m_idStar4 = atoi(strValue);

		if(m_idStar4 == 0)
			m_idStar4 = 1;


		DWORD dwSockThreadId = 0;
		m_hBjThreadRun=CreateThread(0,0,StarCovertDbToBjData,(LPVOID)this,0,&dwSockThreadId);
		if(!m_hBjThreadRun)	
		{
			OutputDebugString("CGetbjDlg::Run �����߳� ERROR!\n");
			return ;
		}
		m_wbStarBtn.SetWindowText("ֹͣ����");
		
	}
	else
	{
		//ǿ�ƽ����߳�
		if(m_hBjThreadRun)
		{
			DWORD code ;
			GetExitCodeThread(m_hBjThreadRun,&code) ; 
			if (code == STILL_ACTIVE)
			{
			   TerminateThread(m_hBjThreadRun,code) ;
				CloseHandle(m_hBjThreadRun) ;
			}

			GetDlgItem(IDC_BUTTON_STAR)->EnableWindow(TRUE);

				//��IDд������������ȥ
			CIniFile szIniFile;
			CString starid;
			starid.Format(_T("%d"),m_idStar0);
			szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail0"),starid);

			starid.Format(_T("%d"),m_idStar1);
			szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail1"),starid);

			starid.Format(_T("%d"),m_idStar2);
			szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail2"),starid);

			starid.Format(_T("%d"),m_idStar3);
			szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail3"),starid);

			starid.Format(_T("%d"),m_idStar4);
			szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail4"),starid);

			m_wbStarBtn.SetWindowText("�Զ�����");
		}
		else
		{
			//AfxMessageBox("������Ѿ����Զ���,�������⣬�ɹر������������!");
		}

	
	}

}


void CLeftClientDlg::OnButtonEnd() 
{
	//ǿ�ƽ����߳�
	if(m_hBjThreadRun)
	{
		DWORD code ;
		GetExitCodeThread(m_hBjThreadRun,&code) ; 
		if (code == STILL_ACTIVE)
		{
		   TerminateThread(m_hBjThreadRun,code) ;
			CloseHandle(m_hBjThreadRun) ;
		}

		GetDlgItem(IDC_BUTTON_STAR)->EnableWindow(TRUE);

			//��IDд������������ȥ
		CIniFile szIniFile;
		CString starid;
		starid.Format(_T("%d"),m_idStar0);
		szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail0"),starid);

		starid.Format(_T("%d"),m_idStar1);
		szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail1"),starid);

		starid.Format(_T("%d"),m_idStar2);
		szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail2"),starid);

		starid.Format(_T("%d"),m_idStar3);
		szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail3"),starid);

		starid.Format(_T("%d"),m_idStar4);
		szIniFile.WriteIniFile(_T("wbidstar"),_T("wbidstardetail4"),starid);

		m_wbStarBtn.SetWindowText("�Զ�����");
	}
}

void CLeftClientDlg::OnButtonHf() 
{
	// TODO: Add your control notification handler code here
	
    ShellExecute(NULL,NULL,_T("http://hufen.sinaapp.com/"),NULL,NULL,SW_SHOWNORMAL);
}

void CLeftClientDlg::OnButtonQun1() 
{
	CString strurl = "http://weibo.com/"+m_wbuserid+"/follow";
	ShellExecute(NULL,NULL,strurl,NULL,NULL,SW_SHOWNORMAL);

}

void CLeftClientDlg::OnButtonQun2() 
{
	CString strurl = "http://weibo.com/"+m_wbuserid+"/fans";
	ShellExecute(NULL,NULL,strurl,NULL,NULL,SW_SHOWNORMAL);
//	ShellExecute(NULL,NULL,_T("http://q.weibo.com/219393"),NULL,NULL,SW_SHOWNORMAL);
	
}

void CLeftClientDlg::OnButtonQun3() 
{
	CString strurl = "http://weibo.com/"+m_wbuserid+"/profile";
	ShellExecute(NULL,NULL,strurl,NULL,NULL,SW_SHOWNORMAL);
	// TODO: Add your control notification handler code here
	//ShellExecute(NULL,NULL,_T("http://q.weibo.com/905125"),NULL,NULL,SW_SHOWNORMAL);
}

/*
BOOL CLeftClientDlg::PreTranslateMessage(MSG *pMsg)
{ 
    if(pMsg-> message   ==WM_KEYDOWN) 
        { 
                if(pMsg-> wParam   ==VK_RETURN   ) 
                { 
                   // return false;   
                } 
         } 
    return     CDialog::PreTranslateMessage   (pMsg); 

}*/
