#ifndef _PARTICLE_H
#define _PARTICLE_H

struct snow
{
	float x;
	float y;
	float speed; //�ٶ�
	int number;  //����ͼ����
};
class CParticle
{
private:
	int m_number; //����
	struct snow *m_pSnow;  //ѩ��
	CImage m_snowMap[7]; //����ѩ��ͼ��
public:
	CParticle(int number);
	~CParticle();

public:
	void Init();  //��ʼ������
	void Draw(CDC &cDC);  //��������
	void Update(float time);//��������
};

#endif