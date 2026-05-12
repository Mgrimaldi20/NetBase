#include "Entry.h"

std::string_view Entry::GetLevelStr(Entry::Level entrylevel) const
{
	switch (entrylevel)
	{
		case Entry::Level::Debug: return "DEBUG";
		case Entry::Level::Info: return "INFO";
		case Entry::Level::Warn: return "WARN";
		case Entry::Level::Error: return "ERROR";
		default: return "UNKNOWN";
	}
}
