#pragma once
#include <Windows.h>
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <iosfwd>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost\locale.hpp>

#pragma comment(lib, "Shlwapi.lib")

struct stDeleteToolbarForeverRecord
{
    std::wstring wsBrowser;
    std::wstring wsType;
    std::wstring wsGuid;
    std::wstring wsCaption;
    std::wstring wsCompnay;
    std::wstring wsVersion;
    std::wstring wsReg1KEY;
    std::wstring wsReg1SubKEY;
    std::wstring wsReg2KEY;
    std::wstring wsReg2SubKEY;
    std::wstring wsDllPath;
    std::wstring wsDeleteDate;
};

class ToolbarRecord
{
public:
    ToolbarRecord(const std::wstring &wsRecordFilePath);
    ~ToolbarRecord(void);
    bool ListRecordForDeleteToolbarForever(std::map<std::wstring, stDeleteToolbarForeverRecord> &mapToolbarRecord);
    bool AddRecordForDeleteToolbarForever(const stDeleteToolbarForeverRecord &stRecord);
    bool DelRecordForDeleteToolbarForever(const stDeleteToolbarForeverRecord &stRecord);
private:
    std::wstring m_RecordFile;
    std::map<std::wstring, stDeleteToolbarForeverRecord> m_ToolbarRecord;
    bool GetRecord(std::map<std::wstring, stDeleteToolbarForeverRecord> &mapToolbarRecord);
    bool SaveRecord(std::map<std::wstring, stDeleteToolbarForeverRecord> &mapToolbarRecord);
};