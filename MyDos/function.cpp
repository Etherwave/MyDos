#include "head.h"
using namespace std;

vector<string> split(string s, string c)
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="s"> s ��Ҫ���зָ����ַ���</param>
	/// <param name="c"> c ������ָ���ָ����ģ���a��bΪ�ָ�������ôcӦΪ"a|b"</param>
	/// <returns></returns>
	vector<string> ans;
	set<char> marks;
	string t = "";
	bool flag = TRUE;

	for (int i = 0; i != c.size(); i++)
	{
		if (i % 2)//���������λ����ô�˴�Ӧ����|���������ʵ�ʵķָ��ַ�
		{
			if (c[i] != '|')
			{
				flag = FALSE;
				break;
			}
		}
		else//�����ż��λ����ô�˴�Ӧ����ʵ����Ҫ�ķָ�������������������ָ�����Ӧ����|
		{
			if (c[i] == '|')
			{
				flag = FALSE;
				break;
			}
			else
			{
				marks.insert(c[i]);
			}
		}
	}
	if (!c.size())//����ָ����ַ�������Ϊ0����ô��û�п��õķָ�����Ҳ�Ǵ���
	{
		flag = FALSE;
	}
	if (flag)//���flagΪ�棬��ô�ָ����������marks������
	{
		for (int i = 0; i != s.size(); i++)
		{
			if (marks.find(s[i]) != marks.end())
			{
				if (t.size())
				{
					ans.push_back(t);
				}
				t = "";
			}
			else
			{
				t += s[i];
			}
		}
		if (t.size())
		{
			ans.push_back(t);
			t = "";
		}
	}
	else
	{
		ans.push_back(s);
	}
	return ans;
}

long long count_files(string path)
{
	long long ans = 0;
	long  hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst((path + "\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib & _A_SUBDIR))//��������ļ��ж����ļ�����ô����һ��
			{
				ans++;
			}

			if (fileinfo.attrib & _A_SUBDIR && (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")))
			{
				//�����ļ����е��ļ����м���
				string subdir = path + "\\" + fileinfo.name;
				ans += count_files(subdir);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return ans;
}

string normalize_windows_path(string path)
{
	/// <summary>
	/// �˺��������������·���г��ֵ�..��.��һ���Ƿ�����һ�㣬һ���ڵ�ǰ��
	/// �纯���ὫD:\\game\\..\\c++ת��Ϊ D:\\c++
	/// ������֤���н�ǿ��³����,�з��ַ�������\\Ҳ������/,��D:/game/lol
	/// </summary>
	/// <param name="path">Ҫ�����һ������·��</param>
	/// <returns>����Ļ�����ԭ�ַ��������򷵻ش�������ַ���</returns>
	string ans = path;
	//�����ж�path�Ƿ���һ������·��
	//����·���ض��Ǵ��̷���ʼ�ģ�����ֻ��Ҫ�ж�ǰ������ĸ�ǲ���һ��Ӣ����ĸ�ӣ�����
	if ((path.size() >= 2 &&
			((path[0] > 'a' && path[0] < 'z') || (path[0] > 'A' && path[0] < 'Z')) &&
			path[1] == ':')
	)
	{
		//���Ǿ���·������ôʹ��\\��/��·���з�
		vector<string> split_path = split(path, "\\|/");
		//�����̷���Ȼ��ʹ��ջ������..��.
		stack<string> s;
		for (int i = 1; i < split_path.size(); i++)
		{
			if (split_path[i] == ".")
			{
				;//ʲôҲ����
			}
			else if (split_path[i] == "..")
			{
				//��ջ�е�����ǰ�ļ��У����ص��ϲ��ļ���
				if (s.size())s.pop();
			}
			else
			{
				//����������������Ͱ�����ļ������Ʒ���ջ��
				s.push(split_path[i]);
			}
		}
		//����ջ�д洢���ļ��У����·��
		ans = "";
		while (s.size())
		{
			ans = "\\" + s.top() + ans;
			s.pop();
		}
		//�����̷�װ��
		ans = split_path[0] + ans;
	}
	return ans;
}

void cp_func_for_thread(int* flag, string origin_path, string target_path)
{
	error_code ec;
	flag[0] = 0;
	fs::copy(origin_path.c_str(), target_path.c_str(), fs::copy_options::recursive, ec); // ����Ŀ¼���ݹ飩
	//ec.valueΪ0 �ǳɹ���80��Ŀ���ļ����ڣ�2��ԴĿ�겻����
	if (ec.value() == 0)
	{
		flag[0] == 1;
	}
	else if (ec.value() == 2)
	{
		//Դ�ļ�������
		flag[0] = -1;
	}
	else if (ec.value() == 80)
	{
		//Ŀ���ļ�����
		flag[0] = -2;
	}
	else
	{
		//��������
		flag[0] = -3;
	}
	return;
}
