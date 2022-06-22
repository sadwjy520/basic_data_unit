/************************************************************************
*@ ��˾���ƣ�##############
*@ �ļ����ƣ�JsonLibconfig.h
*@ ��Ҫ���������俪Դ��rapidjson��libconfig���ݵ��໥ת��
*@
*@ �������ڣ�2022��06��09��
*@ ��ǰ�汾��V1.0
*@ ��    �ߣ�Nobug.W
*@ ˵    ��������Windows/Linux
*@
*@ �޸�ʱ�䣺
*@ ȡ���汾��
*@ ��    �ߣ�
*@ ˵    ����
/************************************************************************/


#ifndef __JSONLIBCONFIG_H__
#define __JSONLIBCONFIG_H__

#include <string>


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#if defined(JSON_LIBCONFIG_STATIC)
#define LIBJSON_LIBCONFIG_API
#elif defined(JSON_LIBCONFIG_EXPORTS)
#define LIBJSON_LIBCONFIG_API __declspec(dllexport)
#else /* ! JSON_LIBCONFIG_EXPORTS */
#define LIBJSON_LIBCONFIG_API __declspec(dllimport)
#endif /* JSON_LIBCONFIG_STATIC */
#else /* ! WIN32 */
#define LIBJSON_LIBCONFIG_API
#endif /* WIN32 */



class LIBJSON_LIBCONFIG_API JsonLibconfig
{
public:
	/************************************************************************
	* �������ƣ�LibconfigToJson
	* ����������֧�ִ�libconfig����ת��rapidjson����
	* ���������const std::string&
	* ���������std::string&
	* �� �� ֵ��bool
	* ����˵������������
	************************************************************************/
	static bool LibconfigToJson(const std::string& cfg, std::string& json);


	/************************************************************************
	* �������ƣ�JsonToLibconfig
	* ����������֧�ִ�rapidjson����ת��libconfig����
	* ���������const std::string&
	* ���������std::string&
	* �� �� ֵ��bool
	* ����˵������������
	************************************************************************/
	static bool JsonToLibconfig(const std::string& json, std::string& cfg);


	/************************************************************************
	* �������ƣ�LibconfigFileToJson
	* ����������֧�ִ�libconfig�����ļ�ת��rapidjson��������
	* ���������const std::string&
	* ���������std::string&
	* �� �� ֵ��bool
	* ����˵������������
	************************************************************************/
	static bool LibconfigFileToJson(const std::string& cfgpath, std::string& json);


	/************************************************************************
	* �������ƣ�JsonFileToLibconfig
	* ����������֧�ִ�rapidjson�����ļ�ת��libconfig��������
	* ���������const std::string&
	* ���������std::string&
	* �� �� ֵ��bool
	* ����˵������������
	************************************************************************/
	static bool JsonFileToLibconfig(const std::string& jsonpath, std::string& cfg);


	/************************************************************************
	* �������ƣ�LibconfigFileToJsonFlie
	* ����������֧�ִ�libconfig�����ļ�ת��rapidjson�����ļ�
	* ���������const std::string&, const std::string&
	* �����������
	* �� �� ֵ��bool
	* ����˵������������
	************************************************************************/
	static bool LibconfigFileToJsonFile(const std::string& cfgpath, const std::string& jsonpath);


	/************************************************************************
	* �������ƣ�JsonFileToLibconfigFile
	* ����������֧�ִ�rapidjson�����ļ�ת��libconfig�����ļ�
	* ���������const std::string&, const std::string&
	* �����������
	* �� �� ֵ��bool
	* ����˵������������
	************************************************************************/
	static bool JsonFileToLibconfigFile(const std::string& jsonpath, const std::string& cfgpath);
};


#endif//__JSONLIBCONFIG_H__
