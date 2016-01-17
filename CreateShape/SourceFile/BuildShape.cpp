#include "BuildShape.h"
#include <QMessageBox>
bool build(const char* pszFileName ,const char *outdataName,int linenum,int rows)
{

	//��ȡӰ����Ϣ
	GDALDataset *poDataSet;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDataSet=(GDALDataset*)GDALOpen(pszFileName,GA_ReadOnly); //�����ݼ�
	if (poDataSet==NULL)
	{
		QMessageBox::critical(NULL,"������ʾ","��դ�����ݼ�ʧ��!",QMessageBox::Ok);
		return false;
	}
	double Trans[6];//����ת����������,��ΪGetGeoTransform()�����Ĳ���
	int width=poDataSet->GetRasterXSize(); //��ȡӰ��������Ϊ��������������׼��
	int height=poDataSet->GetRasterYSize();//��ȡӰ��������Ϊ��������������׼��
	poDataSet->GetGeoTransform(Trans);
	//Xp = padfTransform[0] + P*padfTransform[1] + L*padfTransform[2];
	//Yp = padfTransform[3] + P*padfTransform[4] + L*padfTransform[5];
	//In a north up image, padfTransform[1] is the pixel width, and padfTransform[5] is the pixel height.
	//The upper left corner of the upper left pixel is at position (padfTransform[0],padfTransform[3]).
	//ע��shape�ļ�����
	const char* pszDriverName="ESRI Shapefile";
	OGRSFDriver *poDriver;
	OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDriver=OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);
	if (poDriver==NULL)
	{
		QMessageBox::critical(NULL,"������ʾ","ע���ļ�����ʧ��!",QMessageBox::Ok);
		return false;

	}
	//����shape�ļ�
	OGRDataSource *poDS;
	//����һ����myshapefile��Ŀ¼��������ɵ��ļ�
	//���������.shp��׺����ֱ���ڵ�ǰĿ¼�������ļ�
	poDS=poDriver->CreateDataSource(outdataName,NULL); 
	if (poDS==NULL)
	{
		QMessageBox::critical(NULL,"������ʾ","����ʸ�������ļ�ʧ��!",QMessageBox::Ok);
		return false;
	}
	//�������ͼ��
	OGRLayer *poLayer;
	const char *prj=poDataSet->GetProjectionRef(); //��ȡդ��Ӱ��Ŀռ�ο���Ϣ
	//cout<<"դ�����ݿռ�ο���ϢΪ:\n"<<prj<<endl<<endl;
	OGRSpatialReference oSRS;  
	oSRS.SetFromUserInput(prj);  //����õĿռ�ο���Ϣ�ַ�����Ϊ�ı�һ���Ը���ʸ�����ݵ�OGRSpatialReference����
	poLayer=poDS->CreateLayer(outdataName,&oSRS);
	if (poLayer==NULL)
	{
		QMessageBox::critical(NULL,"������ʾ","����ʸ����ʧ��!",QMessageBox::Ok);
		return false;
	}
	//���������ݵ�����fields
	OGRFieldDefn oField("Point",OFTString);
	oField.SetWidth(10);
	if (poLayer->CreateField(&oField)!=OGRERR_NONE)
	{
		QMessageBox::critical(NULL,"������ʾ","������������fieldʧ��!",QMessageBox::Ok);
		return false;
	}
	//����features��д��feature������
	OGRFeature *poFeature;
	poFeature=OGRFeature::CreateFeature(poLayer->GetLayerDefn());
	//������߿�
	OGRLineString Line;
	OGRPoint Point1(Trans[0],Trans[3]); 
	OGRPoint Point2(Trans[0]+width*Trans[1],Trans[3]);
	OGRPoint Point3(Trans[0]+width*Trans[1],Trans[3]+width*Trans[4]+height*Trans[5]);
	OGRPoint Point4(Trans[0],Trans[3]+width*Trans[4]+height*Trans[5]);
	Line.addPoint(&Point1);
	Line.addPoint(&Point2);
	Line.addPoint(&Point3);
	Line.addPoint(&Point4);
	Line.addPoint(&Point1);
	//ˮƽ�������
	OGRLineString SubHline[50];
	OGRPoint PointLeft[50],PointRight[50];
	for (int i=1;i<linenum;i++)
	{
		PointLeft[i].setX(Trans[0]); //������߿���Ҫ���ߵ����X����
		PointLeft[i].setY((Point4.getY()-Trans[3])/linenum*i+Trans[3]);//������߿���Ҫ���ߵ����Y����
		PointRight[i].setX(Point2.getX());//�����ұ߿���Ҫ���ߵ����X����
		PointRight[i].setY((Point4.getY()-Trans[3])/linenum*i+Trans[3]);//�����ұ߿���Ҫ���ߵ����Y����
	}
	for (int i=1;i<linenum;i++)
	{  
		SubHline[i].addPoint(&PointLeft[i]); //��߿��ϼӵ�
		SubHline[i].addPoint(&PointRight[i]);//�ұ߿��ϼӵ�
		if (i<linenum-1)
		{
			SubHline[i].addPoint(&PointRight[i+1]); //���ұ߿����һ��ת����һ�㣬���⽻��б�ߵĳ���
		}
		Line.addSubLineString(&SubHline[i]);//��SubHline�����е�ÿһ������Ϊ���߶���ӵ�Line������
	}
	Line.addPoint(&Point2);
	//��ֱ�������
	OGRLineString SubVline[50];
	OGRPoint PointUp[50],PointDown[50];
	for (int j=1;j<rows;j++)
	{
		//����ϱ߿��ϵĵ�
		PointUp[j].setX((Point2.getX()-Trans[0])/rows*j+Trans[0]);
		PointUp[j].setY(Trans[3]);
		//����±߿��ϵĵ�
		PointDown[j].setX((Point2.getX()-Trans[0])/rows*j+Trans[0]);
		PointDown[j].setY(Point4.getY());
	}
	for (int j=1;j<rows;j++)
	{
		SubVline[j].addPoint(&PointUp[j]);
		SubVline[j].addPoint(&PointDown[j]);
		if (j<rows-1)
		{
			SubVline[j].addPoint(&PointDown[j+1]); //���±߿��ǰһ��ת����һ�㣬���⽻��б�ߵĳ���
		}
		Line.addSubLineString(&SubVline[j]);
	}
	poFeature->SetGeometryDirectly(&Line);
	if (poLayer->CreateFeature(poFeature)!=OGRERR_NONE)
	{
		QMessageBox::critical(NULL,"������ʾ","��featureд��ʸ���ļ�ʧ��!",QMessageBox::Ok);
		return false;
	}
	OGRFeature::DestroyFeature(poFeature);
	OGRDataSource::DestroyDataSource(poDS);
	//printf("����ʸ�����ݳɹ���\n");
	return true;
}