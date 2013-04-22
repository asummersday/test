static void TrimLogConfig(std::string &str, char ignore = '\0')
{
  if (str.empty())
	{
		return;
	}
	size_t endPos = str.size();
	int posBegin = endPos;
	int posEnd = -1;
	
	for (size_t i = 0; i<str.npos; i++)
	{
		char ch = str[i];
		if (ch != '\r' && ch != '\n' && ch != ' ' && ch != '\t' && ch != ignore)
		{
			posBegin = (int)i;
			break;
		}
	}
	for (size_t i = endPos; i> 0; i--)
	{
		char ch = str[i-1];
		if (ch != '\r' && ch != '\n' && ch != ' ' && ch != '\t' && ch != ignore)
		{
			posEnd = (int)i-1;
			break;
		}
	}
	if (posBegin <= posEnd)
	{
		str = str.substr(posBegin, posEnd-posBegin+1);
	}
	else
	{
		str.clear();
	}
}

static void ParseConfig(std::string file, std::map<std::string, std::map<std::string, std::string> > & outConfig)
{
	//! read file content
	{
		std::ifstream f(file);
		if (f.is_open())
		{
			char buf[500];
			std::string line;
			std::string section;
			std::string key;
			std::string value;
			do 
			{
				if (!f.getline(buf, 500-1, '\n'))
				{
					break;
				}
				line = buf;

				TrimLogConfig(line);

				if (line.empty())
				{
					break;
				}
				if (*(line.begin()) == '#')
				{
					continue;
				}
				if (*(line.begin()) == '[')
				{
					section = line;
					TrimLogConfig(section, '[');
					TrimLogConfig(section, ']');
					continue;
				}
				size_t pos = line.find_first_of('=');
				if (pos == std::string::npos)
				{
					continue;
				}
				key = line.substr(0, pos);
				value = line.substr(pos+1);
				TrimLogConfig(key);
				TrimLogConfig(value);
				outConfig[section][key] = value;				
			} while (1);
		}
	}
}
