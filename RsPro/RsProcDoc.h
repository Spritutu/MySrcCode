
#if !defined(AFX_RSPROCDOC_H__F64D9AA3_15EF_4074_BA07_15AC1DB9A68F__INCLUDED_)
#define AFX_RSPROCDOC_H__F64D9AA3_15EF_4074_BA07_15AC1DB9A68F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/**
*
*CRsProcDoc ��Ķ���,�̳���CDocument
*
*/
class CRsProcDoc : public CDocument
{
protected: // create from serialization only
	CRsProcDoc();
	DECLARE_DYNCREATE(CRsProcDoc)
public:
  int m_nbands;				//!<...������...
  LONG m_nWidth;			//!<...Ӱ����...
  LONG m_nHeigth;			//!<...Ӱ��߶�...
  GDALDataset *poDataset;	//!<...դ�����ݼ�ָ��...
  GDALRasterBand **pbands;	/**<...ָ�򲨶�ָ�������ָ��...*/
  BYTE *pBandBlock[3];		/**<...Ӱ�����ݿ�ָ��...*/
  int max[3];		    	//!<...ͳ��ÿ�����λҶ���Ԫ�������صĸ���...
  int count[3][256];		///<...Ӱ���������εĻҶ�����...
  int max_grey[3];			///<...���ڴ�Ÿ����������Ҷ�ֵ������...
  int min_grey[3];			/*!<...���ڴ�Ÿ���������С�Ҷ�ֵ������...*/

	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	void Calculate();
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
	int SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName);

public:
	virtual ~CRsProcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRsProcDoc)
	afx_msg void OnHistShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSave();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RSPROCDOC_H__F64D9AA3_15EF_4074_BA07_15AC1DB9A68F__INCLUDED_)
