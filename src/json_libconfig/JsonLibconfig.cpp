#include "JsonLibconfig.h"
#include "libconfig/libconfig.h++"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include <fstream>


/************************************************************************
* �������ƣ�__Libconfig2Json
* ����������ʵ�ֶ�libconfig���ö���ĵݹ�����͵ݹ����rapidjson���ö���
* ���������const libconfig::Setting&
* ���������rapidjson::PrettyWriter<rapidjson::StringBuffer>&
* �� �� ֵ��void
* ����˵������̬����
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
* �������ƣ�__Json2Libconfig
* ����������ʵ�ֶ�rapidjson���ö���ĵݹ�����͵ݹ����libconfig���ö���
* ���������const rapidjson::GenericObject<flag, rapidjson::Value>&
* ���������libconfig::Setting&
* �� �� ֵ��void
* ����˵������̬ģ�庯��
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

			/// �ݹ�ִ��
			__Json2Libconfig(sub, set);
		}break;
		case rapidjson::Type::kArrayType:
		{
			cfg_obj.add(item.name.GetString(), libconfig::Setting::TypeList);
			libconfig::Setting& set = cfg_obj[item.name.GetString()];

			for (const auto& sub_item : item.value.GetArray())
			{
				/// ����Ϊ�Ӷ�������
				if (sub_item.IsObject())
				{
					rapidjson::GenericObject<true, rapidjson::Value> sub = sub_item.GetObject();
					libconfig::Setting& set_sub = set.add(libconfig::Setting::TypeGroup);

					/// �ݹ�ִ��
					__Json2Libconfig(sub, set_sub);

					continue;
				}

				/// ����Ϊ��ֵ�����
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
* �������ƣ�LibconfigToJson
* ����������֧�ִ�libconfig����ת��rapidjson����
* ���������const std::string&
* ���������std::string&
* �� �� ֵ��bool
* ����˵�������Ա��������
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
* �������ƣ�JsonToLibconfig
* ����������֧�ִ�rapidjson����ת��libconfig����
* ���������const std::string&
* ���������std::string&
* �� �� ֵ��bool
* ����˵�������Ա��������
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

	/// Ŀǰlibconfigû�����������������ֻ��ͨ����д���ļ����ٴ��ļ���ȡ���
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
* �������ƣ�LibconfigFileToJson
* ����������֧�ִ�libconfig�����ļ�ת��rapidjson��������
* ���������const std::string&
* ���������std::string&
* �� �� ֵ��bool
* ����˵������������
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
* �������ƣ�JsonFileToLibconfig
* ����������֧�ִ�rapidjson�����ļ�ת��libconfig��������
* ���������const std::string&
* ���������std::string&
* �� �� ֵ��bool
* ����˵������������
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
* �������ƣ�LibconfigFileToJsonFlie
* ����������֧�ִ�libconfig�����ļ�ת��rapidjson�����ļ�
* ���������const std::string&
* ���������std::string&
* �� �� ֵ��bool
* ����˵������������
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
* �������ƣ�JsonFileToLibconfigFile
* ����������֧�ִ�rapidjson�����ļ�ת��libconfig�����ļ�
* ���������const std::string&
* ���������std::string&
* �� �� ֵ��bool
* ����˵������������
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

