#ifndef _SCENE_H
#define _SCENE_H

class CScene
{
private:
	CImage m_bg;      //����ͼƬ
	CImage m_black;	
	//ÿ�������СΪ20*20,����800*600�Ĵ��ڼ���40*30��С��������
	int m_fogArray[40][30];
public:
	CScene(char *bg);
	~CScene();
public:
	//���Ʊ���
	void DrawBG(CDC &cDC);
	//��������
	void DrawFog(CDC &cDC);
	//������������
	void UpdateFogArea(int x,int y);
};

#endif