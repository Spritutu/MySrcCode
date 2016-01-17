string  IFormat(const char * fmt ,...)
{
	string message;
	va_list args;
	va_start(args, fmt);

	size_t size = 512;
	char * buffer = new char[size];

	while (1)
	{
		int n = _vsnprintf(buffer, size, fmt, args);

		// If that worked, return a string.
		if ((n > -1) && (static_cast<size_t>(n) < size))
		{
			message.append(buffer);
			delete [] buffer;
			return message;
		}

		// Else try again with more space.
		size = (n > -1) ?
			n + 1 : // ISO/IEC 9899:1999

		size * 2; // twice the old size

		delete [] buffer;
		buffer = new char[size];
	}

	va_end(args);
	return message;
}

/*
* IFormat���Դ��� 
*/
void TestFormatI(){
	int iRev = RE_FAILED; 
	string str1 = IFormat("����ķ����Ƿ���%d",43);
	string str2 = IFormat("����ķ����Ƿ���%f",9.654);
	string str3 = IFormat("����ķ����Ƿ���%p",&iRev);
	string str4 = IFormat("����ķ����Ƿ���%s���","����һ��IFormat���Ժ���");
	string str5 = IFormat("%s,����%d,С��%f,�ַ�%c,��ַ%p","����һ��IFormat���Ժ���",98,87.9878,'M',&iRev);
}