#pragma once
#include "PictueBlock.h"
#include "GameCtrler.h"
#include <map>
using namespace std;
class CBlockManager
{
public:
	friend CGameCtrler;

	static CBlockManager * GetInstance();
	~CBlockManager(void);
	BOOL IntilizeBlock(const CRect & rectPictureWnd,int iRow,int iCol);
	BOOL IntilizeBlockWithRecord(const CRect & rectPictureWnd,int iRow,int iCol,const vector<int> & vIDs);
	BOOL LoadPuzzlePicture(const WCHAR * pszImg,CDC &dc);
	BOOL GetBlockPicture(int iID,CDC & dcBlock);
	void DrawAllBlock(CDC & dc);
	void DrawGridLine(CDC & dc);
	void Clear();
	const map<int,CPictueBlock> & GetBlocks(){return m_BlockMap;}
protected:
	CBlockManager(void);
	void SwapBlock(const int &iID1,const int &iID2);
private:
	class GabageCollector // ����˽����Ƕ��
	{
	public:
		GabageCollector()
		{
			OutputDebugString("����������CUploadManager::GabageCollector����...");
		}
		~GabageCollector()
		{
			if (m_pManager)
			{
				delete m_pManager;
				m_pManager = 0;
				OutputDebugString("����������CUploadManager::GabageCollectorִ�е�����������������...");
			}
		}

	};
	static CBlockManager * m_pManager;
	static GabageCollector m_gc; // ���������������ľ�̬���󣬳��������ϵͳ����øö����������������ִ�жԵ������������
	CDC   m_MemPictureAll;
	CRect  m_WndPictureRect;
	map<int,CPictueBlock> m_BlockMap;
	int m_iRowBlockCount;
	int m_iColBlockCount;
};

