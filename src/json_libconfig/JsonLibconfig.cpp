#include "JsonLibconfig.h"
#include "libconfig/libconfig.h++"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <fstream>


/************************************************************************
* 函数名称：__Libconfig2Json
* 功能描述：实现对libconfig配置对象的递归解析和递归组合rapidjson配置对象
* 输入参数：const libconfig::Setting&
* 输出参数：rapidjson::PrettyWriter<rapidjson::StringBuffer>&
* 返 回 值：void
* 其他说明：静态函数
************************************************************************/
static void __Libconfig2Json(const libconfig::Setting& cfg_obj, rapidjson::PrettyWriter<rapidjson::StringBuffer>& js_obj)
{
	if (cfg_obj.isAggregate())
	{
		if (cfg_obj.isList() || cfg_obj.isArray())
		{
			js_obj.StartArray();
			for (auto &item : cfg_obj)
			{
				if (!item.isAggregate())
				{
					js_obj.Key(item.getName());
				}
				__Libconfig2Json(item, js_obj);
			}
			js_obj.EndArray();
		}
		else if (cfg_obj.isGroup())
		{
			js_obj.StartObject();
			for (auto &item : cfg_obj)
			{
				js_obj.Key(item.getName());
				__Libconfig2Json(item, js_obj);
			}
			js_obj.EndObject();
		}
	}
	else if (cfg_obj.isScalar())
	{
		switch (cfg_obj.getType())
		{
		case libconfig::Setting::Type::TypeInt: js_obj.Int(cfg_obj); break;
		case libconfig::Setting::Type::TypeInt64: js_obj.Int64(cfg_obj); break;
		case libconfig::Setting::Type::TypeFloat: js_obj.Double(cfg_obj); break;
		case libconfig::Setting::Type::TypeBoolean: js_obj.Bool(cfg_obj); break;
		case libconfig::Setting::Type::TypeString: js_obj.String(cfg_obj); break;
		default: break;
		}
	}
}


/************************************************************************
* 函数名称：__Json2Libconfig
* 功能描述：实现对rapidjson配置对象的递归解析和递归组合libconfig配置对象
* 输入参数：const rapidjson::GenericObject<flag, rapidjson::Value>&
* 输出参数：libconfig::Setting&
* 返 回 值：void
* 其他说明：静态模板函数
************************************************************************/
template<bool flag>
static void __Json2Libconfig(const rapidjson::GenericObject<flag, rapidjson::Value>& js_obj, libconfig::Setting& cfg_obj)
{
	for (const auto& item : js_obj)
	{
		switch (item.value.GetType())
		{
		case rapidjson::Type::kNullType:
		{
			cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeNone);
		}break;
		case rapidjson::Type::kFalseType:
		case rapidjson::Type::kTrueType:
		{
			cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeBoolean) = item.value.GetBool();
		}break;
		case rapidjson::Type::kObjectType:
		{
			rapidjson::GenericObject<true, rapidjson::Value> sub = item.value.GetObject();
			libconfig::Setting& set = cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeGroup);

			/// 递归执行
			__Json2Libconfig(sub, set);
		}break;
		case rapidjson::Type::kArrayType:
		{
			cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeList);
			libconfig::Setting& set = cfg_obj[item.name.GetString()];

			for (const auto& sub_item : item.value.GetArray())
			{
				/// 对象为子对象的情况
				if (sub_item.IsObject())
				{
					rapidjson::GenericObject<true, rapidjson::Value> sub = sub_item.GetObject();
					libconfig::Setting& set_sub = set.add(libconfig::Setting::TypeGroup);

					/// 递归执行
					__Json2Libconfig(sub, set_sub);

					continue;
				}

				/// 对象为数值的情况
				libconfig::Setting& set_sub = set.add(libconfig::Setting::TypeGroup);
				if (sub_item.IsInt())
				{
					set_sub.add(libconfig::Setting::TypeInt) = item.value.GetInt();
				}
				else if (sub_item.IsInt64())
				{
					set_sub.add(libconfig::Setting::TypeInt64) = item.value.GetInt64();
				}
				else if (sub_item.IsFloat())
				{
					set_sub.add(libconfig::Setting::TypeFloat) = item.value.GetDouble();
				}
				else if (sub_item.IsDouble())
				{
					set_sub.add(libconfig::Setting::TypeFloat) = item.value.GetDouble();
				}
			}
		}break;
		case rapidjson::Type::kStringType:
		{
			cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeString) = item.value.GetString();
		}break;
		case rapidjson::Type::kNumberType:
		{
			if (item.value.IsInt())
			{
				cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeInt) = item.value.GetInt();
			}
			else if (item.value.IsInt64())
			{
				cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeInt64) = item.value.GetInt64();
			}
			else if (item.value.IsFloat())
			{
				cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeFloat) = item.value.GetDouble();
			}
			else if (item.value.IsDouble())
			{
				cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeFloat) = item.value.GetDouble();
			}
		}break;
		default: break;
		}
	}
}



/************************************************************************
* 函数名称：LibconfigToJson
* 功能描述：支持从libconfig配置转成rapidjson配置
* 输入参数：const std::string&
* 输出参数：std::string&
* 返 回 值：bool
* 其他说明：类成员公共函数
************************************************************************/
bool JsonLibconfig::LibconfigToJson(const std::string& cfg, std::string& json)
{
	libconfig::Config conf;
	try
	{
		conf.readString(cfg);
	}
	catch (const libconfig::ParseException &ex)
	{
		return false;
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	try
	{
		const libconfig::Setting& root = conf.getRoot();
		__Libconfig2Json(root, writer);
	}
	catch (const libconfig::ParseException &ex)
	{
		return false;
	}

	json = buffer.GetString();
	buffer.Clear();

	return !json.empty();
}


/************************************************************************
* 函数名称：JsonToLibconfig
* 功能描述：支持从rapidjson配置转成libconfig配置
* 输入参数：const std::string&
* 输出参数：std::string&
* 返 回 值：bool
* 其他说明：类成员公共函数
************************************************************************/
bool JsonLibconfig::JsonToLibconfig(const std::string& json, std::string& cfg)
{
	rapidjson::Document dom;
	if (dom.Parse(json.c_str()).HasParseError())
	{
		return false;
	}
	libconfig::Config conf;
	libconfig::Setting &root = conf.getRoot();

	rapidjson::GenericObject<false, rapidjson::Value> object = dom.GetObject();
	__Json2Libconfig(object, root);

	std::string cache_file = "./cache";
	conf.writeFile(cache_file);
	conf.clear();

	/// 目前libconfig没有输出数据流，所以只能通过先写入文件，再从文件读取输出
	std::fstream in(cache_file);
	if (!in.is_open())
	{
		return false;
	}
	cfg = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();
	std::remove(cache_file.c_str());

	return !cfg.empty();
}


/************************************************************************
* 函数名称：LibconfigFileToJson
* 功能描述：支持从libconfig配置文件转成rapidjson配置内容
* 输入参数：const std::string&
* 输出参数：std::string&
* 返 回 值：bool
* 其他说明：公共函数
************************************************************************/
bool JsonLibconfig::LibconfigFileToJson(const std::string& cfgpath, std::string& json)
{
	std::fstream in(cfgpath);
	if (!in.is_open())
	{
		return false;
	}
	std::string cfg = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();

	return LibconfigToJson(cfg, json);
}


/************************************************************************
* 函数名称：JsonFileToLibconfig
* 功能描述：支持从rapidjson配置文件转成libconfig配置内容
* 输入参数：const std::string&
* 输出参数：std::string&
* 返 回 值：bool
* 其他说明：公共函数
************************************************************************/
bool JsonLibconfig::JsonFileToLibconfig(const std::string& jsonpath, std::string& cfg)
{
	std::fstream in(jsonpath);
	if (!in.is_open())
	{
		return false;
	}
	std::string json = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();

	return JsonToLibconfig(json, cfg);
}


/************************************************************************
* 函数名称：LibconfigFileToJsonFlie
* 功能描述：支持从libconfig配置文件转成rapidjson配置文件
* 输入参数：const std::string&
* 输出参数：std::string&
* 返 回 值：bool
* 其他说明：公共函数
************************************************************************/
bool JsonLibconfig::LibconfigFileToJsonFile(const std::string& cfgpath, const std::string& jsonpath)
{
	std::string json;
	if (!LibconfigFileToJson(cfgpath, json))
	{
		return false;
	}
	std::ofstream out(jsonpath);
	if (!out.is_open())
	{
		return false;
	}
	out << json;
	out.close();

	return true;
}


/************************************************************************
* 函数名称：JsonFileToLibconfigFile
* 功能描述：支持从rapidjson配置文件转成libconfig配置文件
* 输入参数：const std::string&
* 输出参数：std::string&
* 返 回 值：bool
* 其他说明：公共函数
************************************************************************/
bool JsonLibconfig::JsonFileToLibconfigFile(const std::string& jsonpath, const std::string& cfgpath)
{
	std::string cfg;
	if (!JsonFileToLibconfig(jsonpath, cfg))
	{
		return false;
	}
	std::ofstream out(cfgpath);
	if (!out.is_open())
	{
		return false;
	}
	out << cfg;
	out.close();

	return true;
}

