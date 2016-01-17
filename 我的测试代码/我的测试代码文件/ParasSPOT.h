#include "gdal_priv.h"
#include<vector>
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;

/*
*@breif ����λ�ýṹ��
*/
typedef struct  
{
	double dX;
	double dY;
	double dZ;

	double dVx;
	double dVy;
	double dVz;

	ptime ptm;

}SatellitePosition;
/*
*@breif ������̬�ǽṹ��
*/
typedef struct  
{
	double dYAW;
	double dPITCH;
	double dROLL;

	ptime ptm;
}SatelliteAngle;
/*
*@breif ������͵�λ�ýṹ��
*/
typedef struct
{
	double dLat;
	double dLon;
	double dAltitude;
}NADIRPOS;

typedef struct
{
	int row;
	int col;
	ptime ptm;
}SensorCenter;

typedef GDAL_GCP FrameCenter;
typedef SatelliteAngle SatelliteAngleSpeed;
typedef double LookAngle; 

typedef struct
{
	double lsp;
	LookAngle AlongTrackAngle;
	LookAngle AcrossTrackAngle;
	vector<GDAL_GCP> vGcp;
	vector<SatellitePosition> vPos;
	vector<SatelliteAngle> vAngle;
	vector<SatelliteAngleSpeed> vAngleSpeed;
	NADIRPOS  nadirpos;
	SensorCenter sensorCenter;
	FrameCenter  frameCenter;
}SpotDimParam;

/*
*@brief ��SPOTԪ�����л�ȡ����ʱ���ǹ������
*/
bool GetSpotDimParam(const char *pszMetaData,SpotDimParam & spotDimParam);
/*
*@brief ���ǳ���ʱ�̲�ֵ
*/
ptime InterpolationT(SensorCenter sCenter,int iLine,double dLsp);
/*
*@brief ������̬�ǲ�ֵ
*/
void  InterpolationSensorAngle(SatelliteAngle ang1,SatelliteAngle ang2,SatelliteAngle & outAng);
/*
*@brief ����λ�ü����ٶȲ�ֵ
*/
void  InterpolationSensorPosition(SpotDimParam * DimPram,SatellitePosition  &SatellitePos);
/*
*@brief ������̬�ǲ�ֵ
*/
void  InterpolationSensorAngle(SpotDimParam * DimPram,SatelliteAngle & outAng);
/*
*@brief ������̬���ٶȲ�ֵ
*/
void  InterpolationAngleSpeed(SpotDimParam * DimPram,SatelliteAngleSpeed  &AngSpeed);
