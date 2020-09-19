#include "head.h"
using namespace std;

vector<string> split(string s, string c)
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="s"> s 是要进行分隔的字符串</param>
	/// <param name="c"> c 是用来指定分隔符的，如a，b为分隔符，那么c应为"a|b"</param>
	/// <returns></returns>
	vector<string> ans;
	set<char> marks;
	string t = "";
	bool flag = TRUE;

	for (int i = 0; i != c.size(); i++)
	{
		if (i % 2)//如果是奇数位，那么此处应该是|，用来间隔实际的分隔字符
		{
			if (c[i] != '|')
			{
				flag = FALSE;
				break;
			}
		}
		else//如果是偶数位，那么此处应该是实际需要的分隔符，存下来，但这个分隔符不应该是|
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
	if (!c.size())//如果分隔符字符串长度为0，那么就没有可用的分隔符，也是错误
	{
		flag = FALSE;
	}
	if (flag)//如果flag为真，那么分隔符都存放在marks里面了
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
			if (!(fileinfo.attrib & _A_SUBDIR))//如果不是文件夹而是文件，那么计数一次
			{
				ans++;
			}

			if (fileinfo.attrib & _A_SUBDIR && (strcmp(fileinfo.name, ".") && strcmp(fileinfo.name, "..")))
			{
				//对子文件夹中的文件进行计数
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
	/// 此函数用来处理绝对路径中出现的..和.，一个是返回上一层，一个在当前层
	/// 如函数会将D:\\game\\..\\c++转换为 D:\\c++
	/// 函数保证具有较强的鲁棒性,切分字符可以是\\也可以是/,如D:/game/lol
	/// </summary>
	/// <param name="path">要处理的一个绝对路径</param>
	/// <returns>出错的话返回原字符串，否则返回处理过的字符串</returns>
	string ans = path;
	//首先判断path是否是一个绝对路径
	//绝对路径必定是从盘符开始的，我们只需要判断前两个字母是不是一个英文字母加：就行
	if ((path.size() >= 2 &&
			((path[0] > 'a' && path[0] < 'z') || (path[0] > 'A' && path[0] < 'Z')) &&
			path[1] == ':')
	)
	{
		//若是绝对路径，那么使用\\和/将路径切分
		vector<string> split_path = split(path, "\\|/");
		//保留盘符，然后使用栈来处理..和.
		stack<string> s;
		for (int i = 1; i < split_path.size(); i++)
		{
			if (split_path[i] == ".")
			{
				;//什么也不做
			}
			else if (split_path[i] == "..")
			{
				//从栈中弹出当前文件夹，返回到上层文件夹
				if (s.size())s.pop();
			}
			else
			{
				//除上述两种情况，就把这个文件夹名称放入栈中
				s.push(split_path[i]);
			}
		}
		//根据栈中存储的文件夹，组成路径
		ans = "";
		while (s.size())
		{
			ans = "\\" + s.top() + ans;
			s.pop();
		}
		//最后把盘符装上
		ans = split_path[0] + ans;
	}
	return ans;
}

void cp_func_for_thread(int* flag, string origin_path, string target_path)
{
	error_code ec;
	flag[0] = 0;
	fs::copy(origin_path.c_str(), target_path.c_str(), fs::copy_options::recursive, ec); // 复制目录（递归）
	//ec.value为0 是成功，80是目标文件存在，2是源目标不存在
	if (ec.value() == 0)
	{
		flag[0] == 1;
	}
	else if (ec.value() == 2)
	{
		//源文件不存在
		flag[0] = -1;
	}
	else if (ec.value() == 80)
	{
		//目标文件存在
		flag[0] = -2;
	}
	else
	{
		//其他错误
		flag[0] = -3;
	}
	return;
}
