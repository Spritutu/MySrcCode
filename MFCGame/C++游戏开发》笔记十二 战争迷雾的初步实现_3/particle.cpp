#include"stdafx.h"
#include"particle.h"

CParticle::CParticle(int number)
{
	m_number=number;
	m_pSnow=new struct snow[m_number];
}

//��png��ͼ͸��
void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}
void CParticle::Init()
{
	//����ѩ��ͼ��
	char buf[20];
	for(int i=0;i<7;i++)    //��������ͼ��
	{
		sprintf(buf,"Snow//%d.png",i);
		m_snowMap[i].Load(buf);
		TransparentPNG(&m_snowMap[i]);
	}
	//��ʼ��ѩ������
	for(int i=0;i<m_number;i++)
	{
		m_pSnow[i].x=rand()% WINDOW_WIDTH;   //���ѩ����ˮƽ�������������
		m_pSnow[i].y=rand()% WINDOW_HEIGHT; //��ֱ������Ҳ���������
		m_pSnow[i].number=rand()%7;         //����ѩ���е�һ��
		m_pSnow[i].speed=(rand()%5+1)/20.0;
	}
}

void CParticle::Draw(CDC &cDC)
{
	//����ѩ������
	for(int i=0;i<m_number;i++)
		m_snowMap[m_pSnow[i].number].Draw(cDC,m_pSnow[i].x,m_pSnow[i].y,32,32);
}

void CParticle::Update(float time)
{
	for(int i=0;i<m_number;i++)
	{
		m_pSnow[i].y+=time*m_pSnow[i].speed;
		if(m_pSnow[i].y>WINDOW_HEIGHT)
			m_pSnow[i].y=-32;  
	}
}

CParticle::~CParticle()
{
	delete[] m_pSnow;
}
