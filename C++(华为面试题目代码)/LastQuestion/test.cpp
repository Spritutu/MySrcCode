#include<iostream>
#include<string>
#include<stack>
#include<stdio.h>
using namespace std;

typedef struct SCharacter
{
char m_ch;
int  m_iPosIndex;
}SCharacter;

void TestErrMsg(const SCharacter & sc)
{
const char * tmsg  = "";
const char * tmsg2 = "";
if(sc.m_ch == '(')
{
	tmsg = "��ں�";
	tmsg2 = "�ҿں�";
}
else if(sc.m_ch ==')')
{
	tmsg  = "�ҿں�";
	tmsg2 = "��ں�";
}
	
	printf("\033[31mλ��%d��%s��ƥ���%s\n\033[0m",sc.m_iPosIndex+1,tmsg,tmsg2);
}

void Test2()
{
	cout << "�������ַ���������֤���ŵ�ƥ�������\n";
	string str;
	cin >> str;
	int iLen = str.length();
	stack<SCharacter> tStackCirCle; // Բ����
	char ch = 0;
	SCharacter chStackTop;
	for(int i=0;i< iLen;i++)
	{
		ch = str[i];
		if(ch=='('|| ch ==')')
		{
			if(tStackCirCle.empty())
			{
				SCharacter sc;
				sc.m_ch = ch;
				sc.m_iPosIndex = i;
				tStackCirCle.push(sc);
			}
			else
			{
				chStackTop = tStackCirCle.top();
				if(chStackTop.m_ch == '(' && ch == ')') // mathced
				{
					printf("λ��%d:%cƥ��λ��%d:%c\n",chStackTop.m_iPosIndex+1,chStackTop.m_ch,i+1,ch);
					tStackCirCle.pop(); // ɾ��һ����ƥ�䵽�ַ�
				}
				else
				{
					SCharacter sc;
					sc.m_ch = ch;
					sc.m_iPosIndex = i;
					tStackCirCle.push(sc);
				}

			}
		}
	}

	if(tStackCirCle.empty())
		cout << "�ַ�����Բ������ȫƥ�䣡\n";
	else
	{
		cout << "\033[31mԲ���Ų�ƥ�䣡\n\033[0m";
		while(!tStackCirCle.empty())
		{
			chStackTop = tStackCirCle.top();
			TestErrMsg(chStackTop);
			tStackCirCle.pop();
		}
	}
}
void Test()
{
	cout << "�������ַ���������֤���ŵ�ƥ�������\n";
	string str;
	cin >> str;
	int iLen = str.length();
	stack<char> tStackCirCle; // Բ����
	stack<char> tStackBrace; // ������
	stack<char> tStackSquareBrackets; // ������
	char ch = 0;
	char chStackTop = 0;
	for(int i=0;i< iLen;i++)
	{
		ch = str[i];
		if(ch=='('|| ch ==')')
		{
			if(tStackCirCle.empty())
				tStackCirCle.push(ch);
			else
			{
				chStackTop = tStackCirCle.top();
				if(chStackTop == '(' && ch == ')') // mathced
					tStackCirCle.pop(); // ɾ��һ����ƥ�䵽�ַ�
				else
					tStackCirCle.push(ch);

			}
		}
		else if(ch=='{'|| ch =='}')
		{
			if(tStackBrace.empty())
				tStackBrace.push(ch);
			else
			{
				chStackTop = tStackBrace.top();
				if(chStackTop == '{' && ch == '}') // mathced
					tStackBrace.pop(); // ɾ��һ����ƥ�䵽�ַ�
				else
					tStackBrace.push(ch);

			}
		}
		else if(ch=='['|| ch ==']')
		{
			if(tStackSquareBrackets.empty())
				tStackSquareBrackets.push(ch);
			else
			{
				chStackTop = tStackSquareBrackets.top();
				if(chStackTop == '[' && ch == ']') // mathced
					tStackSquareBrackets.pop(); // ɾ��һ����ƥ�䵽�ַ�
				else
					tStackSquareBrackets.push(ch);

			}
		}
	}

	if(tStackCirCle.empty())
		cout << "�ַ�����Բ������ȫƥ�䣡\n";
	else
		cout << "\033[31mԲ���Ų�ƥ�䣡\n\033[0m";

	if(tStackBrace.empty())
		cout << "�ַ����л�������ȫƥ�䣡\n";
	else
		cout << "\033[31m�����Ų�ƥ�䣡\n\033[0m";

	if(tStackSquareBrackets.empty())
		cout << "�ַ����з�������ȫƥ�䣡\n";
	else
		cout << "\033[31m�����Ų�ƥ�䣡\n\033[0m";

}
int main(int argc,char *argv[])
{
 
while(1)
{
Test2();
}
 return 0;
}
