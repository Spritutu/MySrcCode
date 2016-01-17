// OutlierDetection.cpp: implementation of the CRansac class.
//
//////////////////////////////////////////////////////////////////////


#include "ransac2.h"
#include <time.h>
#include "MartixEigenvalue.h"

#define expvalue 2.2204e-016
#define BrightValue 250
#define DarkValue 5
#define ROUND(x) ((int) ((x) + 0.5))


bool CalAffineTransformCor2(vector<CPoint2D> vSrcPoint,vector<CPoint2D> vRefPoint,vector<double> &dVCor)
{
	int nCount=vSrcPoint.size();

	if (nCount<3) //��Ը���С��2�����ܼ���
		return false;

	double *pDataB=new double[2*nCount]; //����B������

	double *pDataA=new double[12*nCount];

	for(int i=1;i<=nCount;i++) //������������ֵ
	{
		pDataB[2*(i-1)]	  = vSrcPoint[i-1].x;
		pDataB[2*(i-1)+1] = vSrcPoint[i-1].y;

		pDataA[12*(i-1)]	=1;
		pDataA[12*(i-1)+1]	=vRefPoint[i-1].x;
		pDataA[12*(i-1)+2]	=vRefPoint[i-1].y;
		pDataA[12*(i-1)+3]  =0;
		pDataA[12*(i-1)+4]  =0;
		pDataA[12*(i-1)+5]  =0;
		pDataA[12*(i-1)+6]  =0;
		pDataA[12*(i-1)+7]  =0;
		pDataA[12*(i-1)+8]  =0;
		pDataA[12*(i-1)+9]	=1;
		pDataA[12*(i-1)+10]	=vRefPoint[i-1].x;
		pDataA[12*(i-1)+11]	=vRefPoint[i-1].y;
	}

	ExtMatrix eB(pDataB, 2*nCount,1 );
	ExtMatrix eA(pDataA, 2*nCount,6);

	Matrix mB(2*nCount,1);
	Matrix mA(2*nCount,6);

	mtl::twod_copy_default(eB, mB);
	mtl::twod_copy_default(eA, mA);

	Vector X(6); //ϵ��b1,b2,b3,a1,a2,a3

	bool f = SloveMartix(mA, mB, X);

	RELEASE(pDataB);
	RELEASE(pDataA);

	if (!f)
		return false;

	for (size_t i=0;i<6;i++)
		dVCor.push_back(X[i]);

	return true;
}

void GetAffinePara(CPoint2D SrcPt[],CPoint2D RefPt[],int num,double *M)
{
	if (num<3 || !M)
		return;

	vector<CPoint2D> vSrcTmp,vRefTmp;
	for(int i=0;i<num;i++)
	{
		vSrcTmp.push_back(SrcPt[i]);
		vRefTmp.push_back(RefPt[i]);
	}
	vector<double> dVcor; //�洢����任����b1,b2,b3,a1,a2,a3
	bool bOk = CalAffineTransformCor2(vSrcTmp,vRefTmp,dVcor);
	if (!bOk)
		return;
	for (size_t i=0;i<dVcor.size();i++)
		M[i] = dVcor[i];
}
CRansac::CRansac(double dErr)
{
	m_dErr = dErr;
}

CRansac::~CRansac()
{
}
/*
    ���������matchPair����ʼƥ����������
	          nMatchNum����ʼƥ�����������Ŀ
			  index��������ʱ����͵���matchpair�е�����ֵ
			  dbAffine����ϳ��ķ���任ϵ��
			  nAffineNum����ͳ����ĵ���Ŀ

*/
bool CRansac::AFFINERANSAC(Point_Pair* matchPair,int nMatchNum,
								 int*& index,double*& dbAffine,int& nAffineNum)
{
	int s=3;//ƽ�����任
    index = NULL;
    if (ransac(matchPair,nMatchNum,s,dbAffine,index,nAffineNum)) 
        return true;
	else
		return false;
}
bool CRansac::ransac(Point_Pair* matchpair,int nMatchNum,int s,
			double*& dbaffinepara,int*& index,int& nAffineNum)
{
  	double p = 0.99;    
	int maxTrials = 1000;    
	int maxDataTrials = 100; 
   	
	double* M = new double[6];
	memset(M,0,sizeof(double)*6);
	int	trialcount = 0;
    int bestscore =  0;  
	int score; //֧��ĳ����ĵ����Ŀ
    int N = 1;
	int* ind = new int[s];
	int* perMIndex = NULL;
	int* bestMIndex = NULL;
	int i,k;
	int temp;
	while (N > trialcount)
	{
		/*�����������*/
		srand((unsigned)time(NULL)); 
		for(i=0;i<s;i++) //s=3���������Զ��㣬�������Ե㲻����ȫ��ͬ
		{
			temp = rand();
			ind[i] = ROUND(((double)(temp%RAND_MAX))/RAND_MAX*nMatchNum);
			if (ind[i]>=nMatchNum)
				ind[i]= nMatchNum-1;
		}
		while(1)
		{
			if ((ind[0] == ind[1]) && (ind[1]==ind[2])) //s����3������������㲻����ȫ��ͬ
			{
				int temp = rand();
				ind[0] = ROUND(((double)(temp%RAND_MAX))/RAND_MAX*nMatchNum);
				if (ind[0]>=nMatchNum)
					ind[0]= nMatchNum-1;
			}
			else
				break;
		}

 		affinetransform(matchpair,ind,s,M);
		EvaluateDist(matchpair,nMatchNum,M,perMIndex,score);
		if(score > bestscore)   
		{
			bestscore = score;
			
			for(k=0;k<6;k++)
			{
  				if (M[k]<0.0000001) {
					M[k] = 0;
				}
			}
			dbaffinepara = M;
			/*ɾ��ǰһ��͵�����*/
			if (bestMIndex!=NULL) {
				delete[] bestMIndex;
				bestMIndex = NULL;
			}
			bestMIndex = perMIndex;
			double fracinliers =  (double)score/nMatchNum;
			double pNoOutliers = 1 - pow(fracinliers,s);
			pNoOutliers = max(expvalue,pNoOutliers);
			pNoOutliers = min(1-expvalue, pNoOutliers);
			N = int(log(1-p)/log(pNoOutliers));
			trialcount = 0;
			continue;
		}
		else
		{
			if (perMIndex) {
				delete[] perMIndex;
				perMIndex = NULL;
			}
			trialcount = trialcount+1;
		}
		if (trialcount>maxTrials) {
			break;
		}
	}
	if (dbaffinepara==NULL) {
		return false;
	}
	else{
		index = bestMIndex;
		nAffineNum = bestscore;
		return true;
	}
}
void CRansac::affinetransform(Point_Pair* matchpair,int* ind,int s,double* M)
{
	int i,j;
	CPoint2D pPointBase[3];
	CPoint2D pPointSampl[3];
	for(i=0;i<s;i++)
	{
       j = ind[i];
	   pPointBase[i] = matchpair[j].LP; //��Ӱ��ĵ�
	   pPointSampl[i] = matchpair[j].RP;//��Ӱ��ĵ�
	}
	//*��һ��ϵ�� �� R*�ڶ���ϵ������
	GetAffinePara(pPointSampl,pPointBase,3,M);
	return;
}

void CRansac::EvaluateDist(Point_Pair* matchpair,int nMatchNum,double* M,int*& x,int& ninliers)
{
	CPoint2D LP,RP;
	ninliers = 0; 

	double b1 = M[0];
	double b2 = M[1];
	double b3 = M[2];
	double a1 = M[3];
	double a2 = M[4];
	double a3 = M[5];

	x = new int[nMatchNum];
	for(int i=0;i<nMatchNum;i++)
	{
		LP = matchpair[i].LP;
		RP = matchpair[i].RP;

		double dRrrX = b1 + b2 * LP.x + b3 * LP.y - RP.x;
		double dRrrY = a1 + a2 * LP.x + a3 * LP.y - RP.y;

		double dErr = sqrt(M_SQUARE(dRrrX)+M_SQUARE(dRrrY));  //��в�

		if (dErr< m_dErr)
		{
              x[ninliers] = i;
			 ninliers++;	
		}
	}
	return;
}	
	