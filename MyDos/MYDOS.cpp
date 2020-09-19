// MYDOS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "head.h"
#include "MYDOS.h"

MYDOS::MYDOS(string path)
{
	this->path = path;
}

void MYDOS::cd(vector<string> args)
{
	string path = "";
	//cd命令只接受一个参数就好，所以我们使用空格分割，然后取第一个
	if (args.size())
	{
		path = args[0];
		//将路径格式统一转换为绝对路径，但是注意特殊处理.和..这两个路径
		//绝对路径必定是从盘符开始的，我们只需要判断前两个字母是不是一个英文字母加：就行
		if (
			!(path.size() >= 2 &&
			((path[0] > 'a' && path[0] < 'z') || (path[0] > 'A' && path[0] < 'Z')) &&
			path[1] == ':'
			)
		)
		{
			//相对路径转化为绝对路径
			path = this->path + "\\" + path;
		}
		//处理路径中的.和..，进行归一化
		path = normalize_windows_path(path);
		DWORD ftype = GetFileAttributesA(path.c_str());
		if (ftype == INVALID_FILE_ATTRIBUTES)
		{
			cout << "系统找不到指定的路径。" << endl;
		}
		else if (ftype == FILE_ATTRIBUTE_DIRECTORY)
		{
			this->path = path;
		}
		else
		{
			cout << "目录名称无效。" << endl;
		}
	}
	else
	{
		cout << "useage: cd path" << endl;
	}
	cout << endl;
	return;
}

void MYDOS::ls()
{
	long  hFile = 0;//文件句柄  64位下long 改为 intptr_t
	struct _finddata_t fileinfo;//文件信息 
	string p;
	if ((hFile = _findfirst((path + "\\*").c_str(), &fileinfo)) != -1L) //文件存在
	{
		do
		{
			cout << fileinfo.name << endl;
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	cout << endl;
	return;
}

void MYDOS::cls()
{
	system("cls");
}

void MYDOS::help()
{
	const int len = 5;
	string s[len] = {
		"cd 显示当前目录的名称或将其更改。",
		"ls 显示当前目录中的文件和子目录。",
		"cls 清除屏幕。",
		"help 显示帮助。",
		"exit 退出。",
	};
	
	for (int i = 0; i != len; i++)
	{
		cout << s[i] << endl;
	}
	cout << endl;
	return;
}

void MYDOS::rm(vector<string> args)
{
	if (!fs::remove_all(path))//如果没有删除成功
	{
		cout << "文件或文件夹不存在！" << endl;
	}
	return;
}

void MYDOS::cp(vector<string> args)
{
	/// <summary>
	/// cp命令主要功能为复制本地的一个文件到本地的另一个地方
	/// 调用filesystem的copy函数实现（方法1），优点是速度快，其速度和MS-DOS中的copy命令速度相等，
	/// 缺点是无法得知复制进度
	/// MS-DOS的copy命令是采用汇编语言编写的（方法2），相比c语言的fopen,fread,fwirte速度较快（方法3），
	/// c++的fstream相对于c语音更加简便，速度与c语言一致(方法4)
	/// 方法1，2速度最快，但无法得知进度，因为我不太懂汇编，2333
	/// 方法3，4速度相对与前两种速度慢了近一倍，但容易编写，可以得知复制进度
	/// </summary>
	/// <param name="args">原文件/文件夹的路径，目标文件/文件夹的路径</param>
	/// <returns></returns>
	if (args.size() != 2)
	{
		cout << "useage: cp origin_file target_file" << endl;
	}
	else
	{
		
	}
	
	return;
}

void MYDOS::mv(vector<string> args)
{
	if (args.size() != 2)
	{
		cout << "useage: mv from_path to_path" << endl;
	}
	else
	{
		string from_path = args[0];
		string to_path = args[1];
		vector<string> cut_from_path = split(from_path, "");
		string file_name = "";

		error_code ec;
		//先复制，复制成功后再删除源文件
		fs::copy(from_path.c_str(), to_path.c_str(), fs::copy_options::recursive, ec); // 复制目录（递归）
		if (ec.value() == 0)//成功后删除源文件
		{
			bool flag = fs::remove_all(from_path);
			if (!flag)
			{
				cout<<"删除源文件失败！"<<endl;
			}
		}
		else
		{
			cout<<"文件转移失败！"<<endl;
		}
	}
	return;
}

void MYDOS::mkdir(vector<string> args)
{
	vector<string> ans;
	return;
}

void MYDOS::run()
{
	string cmd, order;
	cout << this->path << ">";
	while (1)
	{
		cmd = "", order = "";
		getline(cin, cmd);   
		vector<string> cutcmd = split(cmd, " ");
		vector<string> args;
		order = cutcmd[0];
		for (int i = 1; i != cutcmd.size(); i++)
		{
			args.push_back(cutcmd[i]);
		}

		if (order == "ls")this->ls();
		else if (order == "cd")this->cd(args);
		else if (order == "cls")this->cls();
		else if (order == "help")this->help();
		else if (order == "cp")this->cp(args);
		else if (order == "exit")break;

		cout << this->path << ">";
	}
}
