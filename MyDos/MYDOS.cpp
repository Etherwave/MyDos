// MYDOS.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
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
	//cd����ֻ����һ�������ͺã���������ʹ�ÿո�ָȻ��ȡ��һ��
	if (args.size())
	{
		path = args[0];
		//��·����ʽͳһת��Ϊ����·��������ע�����⴦��.��..������·��
		//����·���ض��Ǵ��̷���ʼ�ģ�����ֻ��Ҫ�ж�ǰ������ĸ�ǲ���һ��Ӣ����ĸ�ӣ�����
		if (
			!(path.size() >= 2 &&
			((path[0] > 'a' && path[0] < 'z') || (path[0] > 'A' && path[0] < 'Z')) &&
			path[1] == ':'
			)
		)
		{
			//���·��ת��Ϊ����·��
			path = this->path + "\\" + path;
		}
		//����·���е�.��..�����й�һ��
		path = normalize_windows_path(path);
		DWORD ftype = GetFileAttributesA(path.c_str());
		if (ftype == INVALID_FILE_ATTRIBUTES)
		{
			cout << "ϵͳ�Ҳ���ָ����·����" << endl;
		}
		else if (ftype == FILE_ATTRIBUTE_DIRECTORY)
		{
			this->path = path;
		}
		else
		{
			cout << "Ŀ¼������Ч��" << endl;
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
	long  hFile = 0;//�ļ����  64λ��long ��Ϊ intptr_t
	struct _finddata_t fileinfo;//�ļ���Ϣ 
	string p;
	if ((hFile = _findfirst((path + "\\*").c_str(), &fileinfo)) != -1L) //�ļ�����
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
		"cd ��ʾ��ǰĿ¼�����ƻ�����ġ�",
		"ls ��ʾ��ǰĿ¼�е��ļ�����Ŀ¼��",
		"cls �����Ļ��",
		"help ��ʾ������",
		"exit �˳���",
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
	if (!fs::remove_all(path))//���û��ɾ���ɹ�
	{
		cout << "�ļ����ļ��в����ڣ�" << endl;
	}
	return;
}

void MYDOS::cp(vector<string> args)
{
	/// <summary>
	/// cp������Ҫ����Ϊ���Ʊ��ص�һ���ļ������ص���һ���ط�
	/// ����filesystem��copy����ʵ�֣�����1�����ŵ����ٶȿ죬���ٶȺ�MS-DOS�е�copy�����ٶ���ȣ�
	/// ȱ�����޷���֪���ƽ���
	/// MS-DOS��copy�����ǲ��û�����Ա�д�ģ�����2�������c���Ե�fopen,fread,fwirte�ٶȽϿ죨����3����
	/// c++��fstream�����c�������Ӽ�㣬�ٶ���c����һ��(����4)
	/// ����1��2�ٶ���죬���޷���֪���ȣ���Ϊ�Ҳ�̫����࣬2333
	/// ����3��4�ٶ������ǰ�����ٶ����˽�һ���������ױ�д�����Ե�֪���ƽ���
	/// </summary>
	/// <param name="args">ԭ�ļ�/�ļ��е�·����Ŀ���ļ�/�ļ��е�·��</param>
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
		//�ȸ��ƣ����Ƴɹ�����ɾ��Դ�ļ�
		fs::copy(from_path.c_str(), to_path.c_str(), fs::copy_options::recursive, ec); // ����Ŀ¼���ݹ飩
		if (ec.value() == 0)//�ɹ���ɾ��Դ�ļ�
		{
			bool flag = fs::remove_all(from_path);
			if (!flag)
			{
				cout<<"ɾ��Դ�ļ�ʧ�ܣ�"<<endl;
			}
		}
		else
		{
			cout<<"�ļ�ת��ʧ�ܣ�"<<endl;
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
