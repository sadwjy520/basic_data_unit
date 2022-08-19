# basic_data_unit
基础数据单元库构建。当前所实现的部分如下：

##### **模块功能**

​		基于C/C++语言使用开源库 **libconfig** 和 **rapidjson** 的二次开发，实现 .json 数据格式与 .cfg 数据格式的相互转换。

##### **模块接口**

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

##### 平台支持

​		目前仅支持Window平台，Linux平台下尚未编译第三方库，但如果需要可自行编译依赖库放入3rdparty并创建相应的路径，然后在项目中引入路径，并且自行实现CMakeList.txt。
