#ifndef  _TRANSFORM_H
#define _TRANSFORM_H

#include "ImgCore.h"
#include "gdal_alg.h"
#include "gdal.h"
#include "ImagePoint.h"
#include "ImageOrthoModel.h"
/**
* @brief ���Ƶ㼸�ζ���ʽ�任����
*/
typedef struct
{
	double * m_dDstToSrcParam;
	double * m_dSrcToDstParam;
	int  iPolynopmialOrder;
}GcpPolynomialPamram;

/**
* @brief ���Ա任����
*/
typedef struct
{
	CPoint2D orgin;
	double m_dScaleX;
	double m_dScaleY;
}LinerTransformParam;

/**
* @brief ��άHelmert�任����
*/
typedef struct 
{
	/*! ԭ������ */
	CPoint2D origin;
	/*! ���ű� */
	double   scale;
	/*! ��ת�Ƕ� */
	double   angle;
}HelmertParameters;

/**
* @brief ����У���ṹ��
*/
typedef struct 
{
	GDALTransformerFunc hfnTransform;
	void *				pTransformArg;
	double 	adfGeoTransform[6];
	double  adfIvtGeoTransform[6];
}GeoTransStruct;

typedef struct
{
	CPushbroomCameraModel * mPushMode;
	SExteriorOrientation  m_exterior;  

}SExteriorOrientationTransformArg;


int   LinerTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess );
void  * CreateLinerTransformer(int iGcpCout,GDAL_GCP * pGcpList);
void  DestroyLinerTransformArg( void * TransFormArg);

int   HelmertTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess );
void  * CreateHelmertTransformer(int iGcpCout,GDAL_GCP * pGcpList);
void  DestroyHelmertTransformArg( void * TransFormArg);

int   GcpPolynomailTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess );
void  * CreateGcpPolynomailTransformArg(GDAL_GCP * pGcpList,int iGcpCount,int iPolynopmialOrder);
void   DestroyGcpPolynomailArg(void * pTransformArg);

int    GeoToPixelTransform( void *pTransformerArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess );
void  * CreateGeoToPixelTransform(GDALTransformerFunc hfnTransform,void *pTransformArg,double *adfGeoTranform);


int  ExteriorOtreationTransform(void *pTransformArg, int bDstToSrc, int nPointCount,double *x, double *y, double *z, int *panSuccess );
void *CreateExteriorOtreationTransformArg(int iGcpCout,GDAL_GCP * pGcpList);
void DestroyExteriorOtreationTransformArg(void * Arg);

int ReSampleImage(GDALDatasetH hSrcDs,GDALDatasetH hDstDs,void *pTransFormArg,GDALTransformerFunc &hTransform,double dMapMinx,double dMapMiny,ResampleMethod resampleMethod =  RM_Bilinear,CProcessBase *pro = NULL);

void  DestroyGeoToPixelTransformArg( void * TransFormArg);

#endif //_TRANSFORM_H