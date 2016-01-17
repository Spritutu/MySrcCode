#include"stdafx.h"
#include"scene.h"

CScene::~CScene()
{
	m_bg.Destroy();
	m_black[0].Destroy();
	m_black[1].Destroy();
}
CScene::CScene(char *bg)
{
	m_bg.Load(bg);
	m_black[0].Load("fog.png");
	m_black[1].Load("fog2.png");
	m_mode=0;
	//��������0,0��ʾΪ��ɫ����״̬
	memset(m_fogArray,0,sizeof(m_fogArray));
	memset(m_clickArray,0,sizeof(m_clickArray));
}

//���Ʊ���
void CScene::DrawBG(CDC &cDC)
{
	m_bg.Draw(cDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
}

//����ս������
void CScene::DrawFog(CDC &cDC)
{
	for(int i=0;i<10;i++)
		for(int j=0;j<5;j++)
			m_black[m_mode].Draw(cDC,i*128,j*128,128,128,(m_fogArray[i][j]/4)*128,(m_fogArray[i][j]%4)*128,128,128);
}

void Add(int fogArray[][6],int i,int j,int num)
{
	fogArray[i][j]+=num;
	if(fogArray[i][j]>15)
		fogArray[i][j]=15;
}
//������������
void CScene::UpdateFogArea(int x,int y)
{
	//���ȼ����������ڵĸ���
	int xPosBox=x/128;
	int yPosBox=y/128;

	if(m_clickArray[xPosBox][yPosBox]==0)
	{
		//����+4������+8������+1,����+2
		Add(m_fogArray,xPosBox,yPosBox,4);
		Add(m_fogArray,xPosBox+1,yPosBox,8);
		Add(m_fogArray,xPosBox,yPosBox+1,1);
		Add(m_fogArray,xPosBox+1,yPosBox+1,2);
		//����ĵط��Ѿ�ɢ����һ���ˣ��Ͳ��ٵ���
		m_clickArray[xPosBox][yPosBox]=1;
	}
}

void CScene::ChangeMode()
{
	m_mode^=1;
}
