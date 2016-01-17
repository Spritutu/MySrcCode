#include <Ice/Ice.h>  
#include "IceCore.h"  
using namespace std;  
using namespace CoreSpace;  
Ice::CommunicatorPtr ic;  
int main(int argc,char *argv[])
{
	//����ͨ����  
	ic = Ice::initialize(argc, argv);  
	//���Ice�������SimplePrinter-�����ʶ����default -p 10000-Э����˿�  
	Ice::ObjectPrx base = ic->stringToProxy("MyIceCore:default -p 10000");  
	//����ת��  
	CoreBasePrx co = CoreBasePrx::checkedCast(base);  
	if (!co) throw "Invalid proxy";  
	//���ò���  
	co->PrintMsg("Hello World!");  
	int iSum = co->add(98,2);
	cout << iSum <<endl;
	int iSub = co->sub(87,7);
	cout<<iSub<<endl;
	ic->destroy(); // ������Դ

	return 0;
}
