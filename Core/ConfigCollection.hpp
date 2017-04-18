#ifndef CONFIGCOLLECTION_H
#define CONFIGCOLLECTION_H

#include <string>
#include <map>

class ConfigValueBase {};
template<typename T>
class ConfigValue : public ConfigValueBase {
	public:
		ConfigValue(T v) : val(v){}
		T getValue() const {
			return val;
		}
	private:
		const T val;
};
class ConfigCollection {
	public:
		template<typename T>
		T GetConfigValue(const std::string& configName) const {
			return static_cast<ConfigValue<T>*>(configMap.at(configName))->getValue();
		}
		template<typename T>
		void AddConfig(const std::string& configName, const T& configValue) {
			configMap[configName] = new ConfigValue<T>(configValue);
		}
		~ConfigCollection() {
			typedef std::map<std::string, ConfigValueBase*>::iterator iter_type;
			for(iter_type it = configMap.begin(); it != configMap.end(); ++it) {
				delete it->second;
			}
		}
	private:
		std::map<std::string, ConfigValueBase*> configMap;
};

#endif	// CONFIGCOLLECTION_H
