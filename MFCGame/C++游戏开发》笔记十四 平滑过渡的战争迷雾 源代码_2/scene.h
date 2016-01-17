#ifndef _SCENE_H
#define _SCENE_H

class CScene
{
private:
	CImage m_bg;      //����ͼƬ
	CImage m_black[2];	
	int m_mode;  //��ʾģʽ
	//ÿ�������СΪ128*128,����1280*640�Ĵ��ڼ���10*5��С��������
	int m_fogArray[11][6];
	//ÿ���Ƿ񱻵����
	int m_clickArray[11][6];
public:
	CScene(char *bg);
	~CScene();
public:
	void ChangeMode();
	//���Ʊ���
	void DrawBG(CDC &cDC);
	//��������
	void DrawFog(CDC &cDC);
	//������������
	void UpdateFogArea(int x,int y);
};

#endif