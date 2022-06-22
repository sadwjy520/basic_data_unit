#include <stdio.h>
#include <fstream>
#include "json_libconfig/JsonLibconfig.h"


int main(int argc, char **argv)
{
	/** ---------------------------------------------------------------------------------------
	*  开源库：libconfig、rapidjson
	*  功  能：.cfg配置和.json配置互转
	*  ---------------------------------------------------------------------------------------*/
	{
		std::string path = "./updated.cfg";
		std::ifstream in(path, std::ios_base::in);
		if (!in.is_open())
		{
			return -1;
		}
		std::string str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();

		std::string cfg;
		std::string json;

		if (!JsonLibconfig::LibconfigToJson(str, json))
		{
			printf("LibconfigToJson is failed!\n");
		}

		if (!JsonLibconfig::JsonToLibconfig(json, cfg))
		{
			printf("JsonToLibconfig is failed!\n");
		}

		if (!JsonLibconfig::LibconfigFileToJson(path, json))
		{
			printf("LibconfigFileToJson is failed!\n");
		}

		std::string jsonpath = "./jsonfile.json";
		if (!JsonLibconfig::LibconfigFileToJsonFile(path, jsonpath))
		{
			printf("LibconfigFileToJsonFile is failed!\n");
		}

		std::string cfgpath = "./cfgfile.cfg";
		if (!JsonLibconfig::JsonFileToLibconfigFile(jsonpath, cfgpath))
		{
			printf("JsonFileToLibconfigFile is failed!\n");
		}

	}

	getchar();

	return 0;
}

