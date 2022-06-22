/************************************************************************
*@ 公司名称：##############
*@ 文件名称：JsonLibconfig.h
*@ 简要描述：适配开源库rapidjson和libconfig数据的相互转换
*@
*@ 创建日期：2022年06月09日
*@ 当前版本：V1.0
*@ 作    者：Nobug.W
*@ 说    明：兼容Windows/Linux
*@
*@ 修改时间：
*@ 取代版本：
*@ 作    者：
*@ 说    明：
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
	* 函数名称：LibconfigToJson
	* 功能描述：支持从libconfig配置转成rapidjson配置
	* 输入参数：const std::string&
	* 输出参数：std::string&
	* 返 回 值：bool
	* 其他说明：公共函数
	************************************************************************/
	static bool LibconfigToJson(const std::string& cfg, std::string& json);


	/************************************************************************
	* 函数名称：JsonToLibconfig
	* 功能描述：支持从rapidjson配置转成libconfig配置
	* 输入参数：const std::string&
	* 输出参数：std::string&
	* 返 回 值：bool
	* 其他说明：公共函数
	************************************************************************/
	static bool JsonToLibconfig(const std::string& json, std::string& cfg);


	/************************************************************************
	* 函数名称：LibconfigFileToJson
	* 功能描述：支持从libconfig配置文件转成rapidjson配置内容
	* 输入参数：const std::string&
	* 输出参数：std::string&
	* 返 回 值：bool
	* 其他说明：公共函数
	************************************************************************/
	static bool LibconfigFileToJson(const std::string& cfgpath, std::string& json);


	/************************************************************************
	* 函数名称：JsonFileToLibconfig
	* 功能描述：支持从rapidjson配置文件转成libconfig配置内容
	* 输入参数：const std::string&
	* 输出参数：std::string&
	* 返 回 值：bool
	* 其他说明：公共函数
	************************************************************************/
	static bool JsonFileToLibconfig(const std::string& jsonpath, std::string& cfg);


	/************************************************************************
	* 函数名称：LibconfigFileToJsonFlie
	* 功能描述：支持从libconfig配置文件转成rapidjson配置文件
	* 输入参数：const std::string&, const std::string&
	* 输出参数：无
	* 返 回 值：bool
	* 其他说明：公共函数
	************************************************************************/
	static bool LibconfigFileToJsonFile(const std::string& cfgpath, const std::string& jsonpath);


	/************************************************************************
	* 函数名称：JsonFileToLibconfigFile
	* 功能描述：支持从rapidjson配置文件转成libconfig配置文件
	* 输入参数：const std::string&, const std::string&
	* 输出参数：无
	* 返 回 值：bool
	* 其他说明：公共函数
	************************************************************************/
	static bool JsonFileToLibconfigFile(const std::string& jsonpath, const std::string& cfgpath);
};


#endif//__JSONLIBCONFIG_H__
