#ifndef _CANNYALG_H
#define _CANNYALG_H

/*
*@ brief ���ø�˹�˲���ͼ������˲����˲��ȶ�x������У�Ȼ���y�����˲�
*@ param pImgData		ָ��ͼ�����ݵ�ָ��
*@ param nWidth			ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param sigma			��˹�����ı�׼��
*@ param pImgOutData	ָ�򾭹�ƽ��֮���ͼ������
*/
void GaussianSmooth(unsigned char *pImgData, int nWidth, int nHeight, 
					double sigma, unsigned char * pImgOutData);
/*
*@ brief ����������㷽���������õ�΢��������(-1 0 1) �� (-1 0 1)T(ת��) �����ʱ��Ա߽����ز��������⴦��
*@ param pGaussSmthedData		ָ��ͼ�����ݵ�ָ��
*@ param nWidth			ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param pnGradX			��˹�����ı�׼��
*@ param pnGradY	ָ�򾭹�ƽ��֮���ͼ������
*/
void DirGrad(unsigned char *pGaussSmthedData, int nWidth, int nHeight,
			 int *pnGradX , int *pnGradY);

/*
*@ brief				���÷����������ݶȷ��ȣ���������DirGrad��������Ľ��
*@ param nWidth			ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param pnGradX		��˹�����ı�׼��
*@ param pnGradY	    ָ�򾭹�ƽ��֮���ͼ������
*/
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag);

/*
*@ brief				��ֹ�ݶ�ͼ�зǾֲ���ֵ�������
*@ param pnMag			�ݶ�ͼ
*@ param pnGradX		x����ķ�����
*@ param pnGradY		y����ķ�����
*@ param nWidth	        ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param pUnchRst	    �洢�����Ľ������
*/
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
					int nHeight,	unsigned char *pUnchRst);

/*
*@ brief		������ʵ�����ơ��������󡱵�һ�����ܣ�Ҳ���ǣ��ȵ���EstimateThreshold
*�����Ծ���non-maximum����������pUnchSpr����һ������ֵ��Ȼ���ж�
*pUnchSpr�п��ܵı߽�����(=128)���ݶ��ǲ��Ǵ��ڸ���ֵnThdHigh�������
*����ֵ�󣬸õ㽫��Ϊһ���߽����㣬����TraceEdge�������Ѷ�Ӧ�ñ߽�
*�����������ҳ�������󣬵������������ʱ�������������û�б���־��
*�߽�㣬��ô��һ�����Ǳ߽�㡣
*@ param pnMag			�ݶȷ���ͼ
*@ param nWidth	        ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param dRatioLow	    �洢�����Ľ������
*@ param dRatioHigh	    ͼ�����ݿ��
*@ param pUnchEdge		ͼ�����ݸ߶�
*/
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
				double dRatioHigh, unsigned char *pUnchEdge);
/*
*@ brief		����non-maximum����������pUnchEdge��ͳ��pnMag��ֱ��ͼ��ȷ����ֵ��
*   ��������ֻ��ͳ��pUnchEdge�п���Ϊ�߽�����Щ���ء�Ȼ������ֱ��ͼ��
*   ����dRatioHigh���ø���ֵ���洢��pnThdHigh������dRationLow�͸���ֵ��
*   ���õ���ֵ���洢��*pnThdLow��dRatioHigh��һ�ֱ����������ݶ�С��
*   *pnThdHigh��������Ŀռ��������Ŀ�ı�����dRationLow����*pnThdHigh
*   ��*pnThdLow�ı��������������canny�㷨��ԭ������߸�����һ�����䡣
*@ param pnMag			�ݶȷ���ͼ
*@ param nWidth	        ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param pnThdHigh	    ����ֵ
*@ param pnThdLow	    ����ֵ
*@ param dRatioLow		����ֵ�͸���ֵ֮��ı���
*@ param dRatioHigh	    ����ֵռͼ�����������ı���
*@ param pUnchEdge		����non-maximum����������
*/
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
					   unsigned char * pUnchEdge, double dRatioHigh, double dRationLow);
/*
*@ brief  ��(x,y)������������б߽��ĸ��٣�����ֻ����pUnchEdge��û�д�����
*�����Ǳ߽�����Щ����(=128),����ֵΪ0�����õ㲻�����Ǳ߽�㣬����ֵΪ255�����õ��Ѿ�������Ϊ�߽�㣬�����ٿ���
*@ param y			��������y���� 
*@ param x	        ��������x����
*@ param nLowThd	�ж�һ�����Ƿ�Ϊ�߽��ĵ���ֵ
*@ param pUnchEdge	��¼�߽��Ļ�����
*@ param pnMag	    �ݶȷ���ͼ
*@ param nWidth		ͼ�����ݿ��
*/
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth);

/*
*@ brief				canny�ָ����ӣ�����Ľ��������pUnchEdge�У��߼�1(255)��ʾ�õ�Ϊ
* �߽�㣬�߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣�ú����Ĳ���sigma��dRatioLow
* dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
*@ param pUnchImage		ͼ������
*@ param nWidth			ͼ�����ݿ��
*@ param nHeight		ͼ�����ݸ߶�
*@ param sigma			��˹�˲��ı�׼����
*@ param dRatioLow		����ֵ�͸���ֵ֮��ı���
*@ param dRatioHigh	    ����ֵռͼ�����������ı���
*@ param pUnchEdge		canny���Ӽ����ķָ�ͼ
*/ 
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
		   double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge);
#endif