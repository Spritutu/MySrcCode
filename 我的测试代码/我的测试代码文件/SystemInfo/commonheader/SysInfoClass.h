
//����һ���࣬���������Ϣ
class CClientInf	//��¼��Ϣ
{
public:
	CHAR			hostName[255];	//������
	ULONG			nClientIP;
	USHORT			nClientPort;
	ULONG			nClientDNS;
	WCHAR			wsSystemInf[250];
	ULONG			nCPUFrequency;
	ULONG			nMemorySize;
	ULONG			nDiskSize;
	ULONG			nFreeMemSize;
	ULONG			nUsedMemSize;
	ULONG           nTotalVirtualMemSize;
	ULONG           nVirtualFreeMemSize;
};