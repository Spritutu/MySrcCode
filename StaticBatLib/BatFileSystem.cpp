/**
*@ Author ����ΰ
*@ Brief  ���ļ�ʵ����BatFileSystem.h�ж���ĺ����������ļ�ϵͳ������������㷨
*@ Vesion 1.0
*@ Email cugxiangzhenwei@sina.cn
*@ Phone 13488751151
*@ Time  2011-10-21
*/
#include "BatFileSystem.h"

int FindSubDirectory(path &dir,vector<path> &ResultPath,bool bIsRecurrence)
{
	if (!exists(dir)||!is_directory(dir))
	{
		return RE_PARAMERROR;
	}
	if (bIsRecurrence)
	{
		rd_iterator end; // �ݹ������
		for (rd_iterator pos(dir);pos!=end;++pos)
		{
			if (is_directory(*pos))
				ResultPath.push_back(pos->path());
		}
	}

	else
	{
		urd_iterator end;
		for (urd_iterator pos(dir);pos!=end;++pos)
		{
			if (is_directory(*pos))
				ResultPath.push_back(pos->path());
		}
	}
	if (ResultPath.size()==0)
	{
		return RE_NOTFOUND;
	}
	return RE_SUCCESS;
}
int FindAllFile(path &dir,vector<path> &ResultFileList, bool bIsRecurrence)
{
	if (!exists(dir)||!is_directory(dir))
	{
		return RE_PARAMERROR;
	}
	if (bIsRecurrence)
	{
		rd_iterator end; // �ݹ������
		for (rd_iterator pos(dir);pos!=end;++pos)
		{
			if (!is_directory(*pos))
				ResultFileList.push_back(pos->path());
		}
	}

	else
	{
		urd_iterator end;
		for (urd_iterator pos(dir);pos!=end;++pos)
		{
			if (!is_directory(*pos))
				ResultFileList.push_back(pos->path());
		}
	}
	return RE_SUCCESS;
}
int FindAllFileAndDirectory(path &dir,vector<path> &ResultList,bool bIsRecurrence)
{
	if (!exists(dir)||!is_directory(dir))
	{
		return RE_PARAMERROR;
	}
	if (bIsRecurrence)
	{
		rd_iterator end; // �ݹ������
		for (rd_iterator pos(dir);pos!=end;++pos)
			ResultList.push_back(pos->path());

	}

	else
	{
		urd_iterator end; //���ݹ����
		for (urd_iterator pos(dir);pos!=end;++pos)
			ResultList.push_back(pos->path());
	}
	if (ResultList.size()==0)
	{
		return RE_NOTFOUND;
	}
	return RE_SUCCESS;
}
int FindExtenFile(path &dir,vector<path> &ResultList,string extension,bool bIsRecurrence)
{
	if (!exists(dir)||!is_directory(dir))
	{
		return RE_PARAMERROR;
	}
	if (bIsRecurrence)
	{
		rd_iterator end; // �ݹ������
		for (rd_iterator pos(dir);pos!=end;++pos)
		{
			string striex=pos->path().extension().string();
			to_upper(striex);
			to_upper(extension);
			if (striex==extension)
			{
				ResultList.push_back(pos->path());
			}
		}

	}
	else
	{
		urd_iterator end; //���ݹ����
		for (urd_iterator pos(dir);pos!=end;++pos)
		{
			string striex=pos->path().extension().string();
			to_upper(striex);
			to_upper(extension);
			if (striex==extension)
			{
				ResultList.push_back(pos->path());
			}

		}
	}
	if (ResultList.size()==0)
	{
		return RE_NOTFOUND;
	}
	return RE_SUCCESS;
}

int AppendStrToFile(const char* filename,string strAppend)
{
	setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
	ofstream  file(filename,ios::app);//����һ���ɶ�д���ļ�������,·����֧�ֿո�
	if (!file)
	{
		return RE_OPENFAILED;
	}
	file<<strAppend;
	file.close();
	setlocale(LC_ALL,"C");//��ԭ
	/*CFile file;
	CString str = filename;
	if(file.Open(str,CFile::modeReadWrite))
	{
		file.SeekToEnd();//��λ���ļ�β
		file.Write(strAppend.c_str(),strAppend.size());
		file.Close();
		return RE_SUCCESS;
	}*/
	return RE_OPENFAILED;
}
int CopyFileToDirectory(path &theFile,vector<path> &vpath,bool bOverWritten)
{
	int num=vpath.size();
	if (num==0)
	{
		return RE_PARAMERROR;
	}
	progress_display pd(num); //����������
	for (int i=0;i<num;i++)
	{
		string src=theFile.string();
		string des=vpath[i].string();
		if (is_directory(vpath[i]))
		{
			string fn=theFile.filename().string();
			des=des+"\\"+fn;
			path SrcPath(src);
			path DesPath(des);
			if (bOverWritten)
			{
				copy_file(SrcPath,DesPath,copy_option::overwrite_if_exists);
			}
		}
		++pd;
	}
	return RE_SUCCESS;
}
int GetFileInfo(path & pathFile,FileInfo &strInfo)
{
	if (is_directory(pathFile))
	{
		return RE_PARAMERROR;
	}
	strInfo.ParentPath=pathFile.parent_path().string();
	strInfo.ext=pathFile.extension().string();
	strInfo.filename=pathFile.filename().string();
	strInfo.filesize=file_size(pathFile);
	return RE_SUCCESS;
}
int DelFile(path &dir,string theName,bool IsFile,bool bIsRecurrence)
{
		if (!exists(dir)||!is_directory(dir))
		{
			return RE_PARAMERROR;
		}
	if (bIsRecurrence)
	{
		rd_iterator end; // �ݹ������
		for (rd_iterator pos(dir);pos!=end;++pos)
		{
			if (IsFile) //Ҫɾ�������ļ�
			{
				if (is_directory(*pos))
					continue;
				string str=pos->path().extension().string();
					if (str==theName)
						fs::remove(pos->path());
			}
		}

	}
	else
	{
		urd_iterator end; // ���ݹ������
		for (urd_iterator pos(dir);pos!=end;++pos)
		{
				if (IsFile) //Ҫɾ�������ļ�
				{
					if (is_directory(*pos))
						continue;
					string str=pos->path().extension().string();
					if (str==theName)
					{
						fs::remove(pos->path());
					}
				}
			}

		}
	return RE_SUCCESS;
}

void find_file(const path &dir,const string &filename,vector<path>&v)
{
	static sregex_compiler rc;  //������ʽ����
	if(!rc[filename].regex_id())
	{
		string str =replace_all_copy(replace_all_copy(filename,".","\\."),"*",".*");
		rc[filename]=rc.compile(str);
	}
	if(!exists(dir)||!is_directory(dir))
		return;
	rd_iterator end;
	for(rd_iterator pos(dir);pos!=end;++pos)
	{
		/*if(!is_directory(*pos)&&regex_match(pos->path().filename(),rc[filename]))
			v.push_back(pos->path());*/
		;
	}
}