/*
*@brief ��γ������תֱ������
*@param lon		�����ؾ���
*@param lat		�����γ��
*@param height  �����ظ߳�
*@param x		������ֱ������x
*@param y		������ֱ������y
*@param z		������ֱ������z
*/
static void LatLonHeightToXYZ( const double lon, const double lat, const double height,double &x, double &y, double &z)
{
	double a = 6378249.2000 /*6378249.200*/;	//�����峤����
	double e = 0.08248325679 /*0.08248325679*/;
	double N = a/sqrt(1-M_SQUARE(e*sin(lat)) );
	x = (N+ height)*cos(lat)*cos(lon);
	y = (N+ height)*cos(lat)*sin(lon);
	z = (N*(1-e*e) +height)*sin(lat);
	printf("���ȣ�%f, γ�ȣ�%f, �̣߳�%f, ���򳤰���%f, �������%f ,x = %.4f ,y =%.4f ,z = %.4f\n",lon,lat,height,a,e,x,y,z);
}