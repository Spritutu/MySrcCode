#include <stdio.h>
#include "MyClass.h"
#include "ice/Application.h"

class  MyApp:virtual public Ice::Application
{
	virtual int run(int ,char *[]);
};
int MyApp::run(int argc,char *argv[])
{
	Ice::CommunicatorPtr ic;  
	//����ͨ����  
	ic=Ice::initialize(argc, argv);  
	//�����������������󶨶˿�  
	Ice::ObjectAdapterPtr adapter=ic->createObjectAdapterWithEndpoints(  
		"MyIceCoreAdapter","default -p 10000");  
	Ice::ObjectPtr object = new MyCore();  
	//����Ice����ʵ����ָ����ʶ��  
	adapter->add(object,ic->stringToIdentity("MyIceCore"));  
	//����������  
	adapter->activate();  
	//����ý���
	cout<< "�ȴ��ͻ�����Ӧ..."<<endl;
	ic->waitForShutdown();  
	return 0;
}
int main(int argc,char *argv[])
{
	MyApp app;
	return app.main(argc,argv);
}