#pragma once
class CPictueBlock
{
public:
	CPictueBlock(int iPosID,int iCurPosID,CRect PicturRect);
	~CPictueBlock(void);
	virtual void DrawBlock(CDC & dcAllBlock);
public:
	int m_iPosID;   // ��ȷ��λ�ñ��
	int m_iCurPosID; // ʵ��λ�ñ��
	CRect m_RectBlock; //���ݿ���������ͼ���е�λ��
};

