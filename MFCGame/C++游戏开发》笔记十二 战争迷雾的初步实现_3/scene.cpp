#include"stdafx.h"
#include"scene.h"

CScene::CScene(char *bg)
{
	m_bg.Load(bg);
	m_black.Load("black.png");
	//��������0,0��ʾΪ��ɫ����״̬
	memset(m_fogArray,0,sizeof(m_fogArray));
}

//���Ʊ���
void CScene::DrawBG(CDC &cDC)
{
	m_bg.Draw(cDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
}

//����ս������
void CScene::DrawFog(CDC &cDC)
{
	for(int i=0;i<40;i++)
		for(int j=0;j<30;j++)
		{
			if(m_fogArray[i][j]==0)
				m_black.Draw(cDC,i*20,j*20,20,20);
		}
}

bool CheckFog(int xBox,int yBox,int xMouse,int yMouse)
{
	//�����˷���false
	if(xBox<0 || xBox>=40 || yBox<0 || yBox>=30)
		return false;
	//δ���磬��������������С��һ���ķ�Χ�ڿɼ�
	if( (xBox-xMouse)*(xBox-xMouse) + (yBox-yMouse)*(yBox-yMouse) <=16)
		return true;
	else return false;
}
//������������
void CScene::UpdateFogArea(int x,int y)
{
	//���ȼ����������ڵĸ���
	int xPosBox=x/20;
	int yPosBox=y/20;
	//����������ԭ
	memset(m_fogArray,0,sizeof(m_fogArray));
	//���ÿɼ�����
	for(int xBox=xPosBox-8;xBox<xPosBox+8;xBox++)
	{
		for(int yBox=yPosBox-8;yBox<yPosBox+8;yBox++)
		{
			if(CheckFog(xBox,yBox,xPosBox,yPosBox))
				m_fogArray[xBox][yBox]=1;
		}
	}
}

