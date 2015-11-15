#include "StdAfx.h"
#include "GameCtrler.h"
#include "DataDefine.h"
#include "Resource.h"
#include "Game2048Dlg.h"
#include <vector>
using namespace std;

CGameCtrler::CGameCtrler(CGame2048Dlg * pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_iSumScore = 0;
	m_iCurScore = 0;
	srand(time(NULL));
}
CGameCtrler::~CGameCtrler(void)
{
}
void CGameCtrler::MoveUp()
{
	RestoreStatus();
	m_iCurScore = 0;
	for (int j=0;j<4;j++)
		ProcessOneColumToUp(j);

	MakeNewBlockInEmptyErea();
	m_pMainWnd->Invalidate(FALSE);
}
void CGameCtrler::MoveRight()
{
	RestoreStatus();
	m_iCurScore = 0;
	for (int j=0;j<4;j++)
		ProcessOneRowToRight(j);

	MakeNewBlockInEmptyErea();
	m_pMainWnd->Invalidate(FALSE);
}
void CGameCtrler::MoveLeft()
{
	RestoreStatus();
	m_iCurScore = 0;
	for (int j=0;j<4;j++)
		ProcessOneRowToLeft(j);

	MakeNewBlockInEmptyErea();
	m_pMainWnd->Invalidate(FALSE);
}
void CGameCtrler::MoveDown()
{
	RestoreStatus();
	m_iCurScore = 0;
	for (int j=0;j<4;j++)
		ProcessOneColumToDown(j);

	MakeNewBlockInEmptyErea();
	m_pMainWnd->Invalidate(FALSE);
}
void CGameCtrler::MakeNewBlockInEmptyErea()
{
	vector<int> vEmptyID;
	for (int i=0;i<16;i++)
	{
		if (m_pMainWnd->m_RectBlocks[i].m_iGameNumShow ==0)
			vEmptyID.push_back(i);
	}
	if (vEmptyID.empty())
	{
		//if (IsGameOver()) // ���ܲ����հ�
		//	MessageBox(m_pMainWnd->m_hWnd,"��Ϸ����!","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
		return;
	}
	if (!StatusChanged())
		return;

	int iIndex = rand()%vEmptyID.size();
	int iNewID = vEmptyID[iIndex]; // �ĵ������ֿ��id

	int iNewNum = (rand()%8)==7 ? 4:2; // ������2����4
	m_pMainWnd->m_RectBlocks[iNewID].m_iGameNumShow = iNewNum;
	m_pMainWnd->m_RectBlocks[iNewID].m_iFlashCount = FLASH_COUNT;
	m_iFlashBlockID = iNewID;
	m_pMainWnd->SetTimer(TIMER_FLASH_BLOCK,TIMER_SPACE_FLASH,NULL);
	if (IsGameOver()) // ���ܲ����հ�
		MessageBox(m_pMainWnd->m_hWnd,"��Ϸ����!","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
}
void CGameCtrler::ProcessOneColumToDown(int iColumId)
{
	BlockNode node[4];
	for (int i=0;i<4;i++)
	{
		int iId =(3-i)*4 + iColumId;
		int iNum = m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow;

		node[i].m_iID = iId;
		node[i].m_iShowNum = iNum;
		if (i<3)
			node[i].m_pNextBlock = &node[i+1];
	}

	ProcessDataListNode(node);
	for (int i=0;i<4;i++)
	{
		int iId =(3-i)*4 + iColumId;
		m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow = node[i].m_iShowNum;
	}
}
void CGameCtrler::ProcessOneColumToUp(int iColumId)
{
	BlockNode node[4];
	for (int i=0;i<4;i++)
	{
		int iId = i*4 + iColumId;
		int iNum = m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow;

		node[i].m_iID = iId;
		node[i].m_iShowNum = iNum;
		if (i<3)
			node[i].m_pNextBlock = &node[i+1];
	}

	ProcessDataListNode(node);
	for (int i=0;i<4;i++)
	{
		int iId = i*4 + iColumId;
		m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow = node[i].m_iShowNum;
	}
}
void CGameCtrler::ProcessOneRowToLeft(int iRowId)
{
	BlockNode node[4];
	for (int i=0;i<4;i++) // ��ѭ��
	{
		int iId = iRowId*4 + i;
		int iNum = m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow;

		node[i].m_iID = iId;
		node[i].m_iShowNum = iNum;
		if (i<3)
			node[i].m_pNextBlock = &node[i+1];
	}

	ProcessDataListNode(node);
	for (int i=0;i<4;i++) // ��ѭ��
	{
		int iId = iRowId*4 + i;
		m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow = node[i].m_iShowNum;
	}
}
void CGameCtrler::ProcessOneRowToRight(int iRowId)
{
	BlockNode node[4];
	for (int i=0;i<4;i++) // ��ѭ��
	{
		int iId = iRowId*4 +(3-i);
		int iNum = m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow;

		node[i].m_iID = iId;
		node[i].m_iShowNum = iNum;
		if (i<3)
			node[i].m_pNextBlock = &node[i+1];
	}

	ProcessDataListNode(node);
	for (int i=0;i<4;i++) // ��ѭ��
	{
		int iId = iRowId*4 +(3-i);
		m_pMainWnd->m_RectBlocks[iId].m_iGameNumShow = node[i].m_iShowNum;
	}
}
void CGameCtrler::ProcessDataListNode(BlockNode * pNodeHeader)
{
	// �ƶ�����
	BlockNode *pNodeCur = pNodeHeader;
	vector<int> vNums(4,0);
	int iIndexNoneZero = 0;
	while(pNodeCur)
	{
		if (pNodeCur->m_iShowNum!=0)
		{
			vNums[iIndexNoneZero] = pNodeCur->m_iShowNum;
			iIndexNoneZero++;
		}
		pNodeCur = pNodeCur->m_pNextBlock;
	}
	pNodeCur = pNodeHeader;
	int i = 0;
	while(pNodeCur)
	{
		pNodeCur->m_iShowNum = vNums[i];
		pNodeCur = pNodeCur->m_pNextBlock;
		i++;
	}
	// ������
	BOOL bAddSuccess = FALSE;
	for (int i=0;i<3;i++)
	{
		if (vNums[i] == vNums[i+1] && vNums[i]!=0)
		{
			vNums[i] += vNums[i+1];
			vNums[i+1] = 0;
			m_iCurScore += vNums[i];
			m_iSumScore += vNums[i];
			bAddSuccess = TRUE;
			i++;
		}
	}

	if (bAddSuccess) // ���ڵ���ӳɹ�
	{
		vector<int> vNumTmp(4,0);
		iIndexNoneZero = 0;
		for (int i=0;i<4;i++)
		{
			if (vNums[i]!=0)
				vNumTmp[iIndexNoneZero++] = vNums[i];
		}
		vNums.clear();
		vNums = vNumTmp;

		pNodeCur = pNodeHeader;
		int i = 0;
		while(pNodeCur)
		{
			pNodeCur->m_iShowNum = vNums[i];
			pNodeCur = pNodeCur->m_pNextBlock;
			i++;
		}
	}
}
BOOL CGameCtrler::IsGameOver()
{ 
	// ���ÿһ���Ƿ��п���ӵĻ����пհ�λ��
	for (int i=0;i<4;i++)
	{
		int iPreNum = m_pMainWnd->m_RectBlocks[i*4].m_iGameNumShow;
		if (iPreNum==0)
			return FALSE;

		for (int j=1;j<4;j++)
		{
			int iCurNum = m_pMainWnd->m_RectBlocks[i*4 + j].m_iGameNumShow;
			if (iCurNum==0 || iPreNum==iCurNum)
				return FALSE;

			iPreNum = iCurNum;
		}
	}

	// ���ÿһ���Ƿ��п���ӵ�
	for (int j=0;j<4;j++)
	{
		int iPreNum = m_pMainWnd->m_RectBlocks[j].m_iGameNumShow;
		if (iPreNum==0)
			return FALSE;

		for (int i=1;i<4;i++)
		{
			int iCurNum = m_pMainWnd->m_RectBlocks[i*4 + j].m_iGameNumShow;
			if (iCurNum==0 || iPreNum==iCurNum)
				return FALSE;

			iPreNum = iCurNum;
		}
	}

	return TRUE;
}
void  CGameCtrler::RestoreStatus()
{
	for(int i=0;i<16;i++)
		m_iRestoreStatus[i] = m_pMainWnd->m_RectBlocks[i].m_iGameNumShow;
}
BOOL CGameCtrler::StatusChanged()
{
	for(int i=0;i<16;i++)
		if(m_iRestoreStatus[i] != m_pMainWnd->m_RectBlocks[i].m_iGameNumShow)
			return TRUE;

	return FALSE;
}
void CGameCtrler::UpdateFlashTimeCount()
{
	m_pMainWnd->m_RectBlocks[m_iFlashBlockID].m_iFlashCount --;
	if (m_pMainWnd->m_RectBlocks[m_iFlashBlockID].m_iFlashCount==0)
	{
		m_pMainWnd->KillTimer(TIMER_FLASH_BLOCK);
	}
}