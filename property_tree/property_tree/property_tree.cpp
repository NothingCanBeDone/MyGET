// property_tree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/locale.hpp>
#include <shlwapi.h>
#include <iosfwd>

#include "ToolbarRecord.h"

#pragma comment(lib, "Shlwapi.lib")

struct FFToolbarRecord
{
    std::wstring wsCaption;
    std::wstring wsDescription;
    std::wstring wsCompnay;
    std::wstring wsVersion;
    std::wstring wsIcon;
    std::wstring wsClsid;
    std::wstring wsDllorFileCaption;
    std::wstring wsDllorFilepath;
    std::wstring wsToolType;
    std::wstring wsDeleteTime;
    FFToolbarRecord():wsCaption(L""),wsDescription(L""),wsCompnay(L""),wsVersion(L""),wsIcon(L""),
        wsClsid(L""),wsDllorFileCaption(L""),wsDllorFilepath(L""),wsToolType(L""),wsDeleteTime(L"")
    {        
    }
};

struct IEToolbarRecord
{
    std::wstring wsCaption;
    std::wstring wsDescription;
    std::wstring wsCompnay;
    std::wstring wsVersion;
    std::wstring wsClsid;
    std::wstring wsDllCaption;
    std::wstring wsDllpath;
    std::wstring wsToolType;
    std::wstring wsDeleteTime;
    IEToolbarRecord():wsCaption(L""),wsDescription(L""),wsCompnay(L""),wsVersion(L""),
        wsClsid(L""),wsDllCaption(L""),wsDllpath(L""),wsToolType(L""),wsDeleteTime(L"")
    {        
    }
};

bool readIEXMLFile(const std::wstring &file_path, std::map<std::wstring, IEToolbarRecord> &mapIEToolbarRecord)
{
    if (!PathFileExists(file_path.c_str())) 
    {
        return false;
    }
    std::string strRecordFile = boost::locale::conv::from_utf(file_path, "UTF-8");
    mapIEToolbarRecord.clear();
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;

    try 
    {
	    boost::property_tree::read_xml(strRecordFile, root);
	    node = root.get_child("IEToolBar");
	    boost::property_tree::ptree::const_iterator iteraEnd = node.end();
	    for (boost::property_tree::ptree::const_iterator itera = node.begin(); itera != iteraEnd; ++itera)
	    {
	        std::string strVersion          = itera->second.get<std::string>("Version");
	        std::string strCaption          = itera->second.get<std::string>("Caption");
	        std::string strClsid            = itera->second.get<std::string>("Clsid");
	        std::string strCompnay          = itera->second.get<std::string>("Compnay");
	        std::string strDescription      = itera->second.get<std::string>("Description");
	        std::string strDllCaption       = itera->second.get<std::string>("DllCaption");
	        std::string strDllpath          = itera->second.get<std::string>("Dllpath");
	        std::string strToolType         = itera->second.get<std::string>("ToolType");
	        std::string strDeleteTime       = itera->second.get<std::string>("DeleteTime");
	        IEToolbarRecord stRecord;
	        stRecord.wsVersion          = boost::locale::conv::to_utf<wchar_t>(strVersion, "UTF-8");
	        stRecord.wsCaption          = boost::locale::conv::to_utf<wchar_t>(strCaption, "UTF-8");
	        stRecord.wsClsid            = boost::locale::conv::to_utf<wchar_t>(strClsid, "UTF-8");
	        stRecord.wsCompnay          = boost::locale::conv::to_utf<wchar_t>(strCompnay, "UTF-8");
	        stRecord.wsDescription      = boost::locale::conv::to_utf<wchar_t>(strDescription, "UTF-8");
	        stRecord.wsDllCaption       = boost::locale::conv::to_utf<wchar_t>(strDllCaption, "UTF-8");
	        stRecord.wsDllpath          = boost::locale::conv::to_utf<wchar_t>(strDllpath, "UTF-8");
	        stRecord.wsToolType         = boost::locale::conv::to_utf<wchar_t>(strToolType, "UTF-8");
	        stRecord.wsDeleteTime       = boost::locale::conv::to_utf<wchar_t>(strDeleteTime, "UTF-8");
	        mapIEToolbarRecord.insert(std::map<std::wstring, IEToolbarRecord>::value_type(stRecord.wsClsid, stRecord));
	    }
    }
    catch (std::exception &ex)
    {
        OutputDebugStringA(ex.what());
        return false;
    }
    return true;
}

bool writeIEXMLFile(const std::wstring &file_path, std::map<std::wstring, IEToolbarRecord> &mapIEToolbarRecord)
{
    try
    {
        boost::property_tree::ptree root;
        for (auto itera = mapIEToolbarRecord.begin(); itera != mapIEToolbarRecord.end(); ++itera)
        {
            boost::property_tree::ptree pt;
            std::string strVersion      = boost::locale::conv::from_utf(itera->second.wsVersion, "UTF-8");
            std::string strCaption      = boost::locale::conv::from_utf(itera->second.wsCaption, "UTF-8");
            std::string strClsid        = boost::locale::conv::from_utf(itera->second.wsClsid, "UTF-8");
            std::string strCompnay      = boost::locale::conv::from_utf(itera->second.wsCompnay, "UTF-8");
            std::string strDescription  = boost::locale::conv::from_utf(itera->second.wsDescription, "UTF-8");
            std::string strDllCaption   = boost::locale::conv::from_utf(itera->second.wsDllCaption, "UTF-8");
            std::string strDllpath      = boost::locale::conv::from_utf(itera->second.wsDllpath, "UTF-8");
            std::string strToolType     = boost::locale::conv::from_utf(itera->second.wsToolType, "UTF-8");
            std::string strDeleteTime   = boost::locale::conv::from_utf(itera->second.wsDeleteTime, "UTF-8");
            pt.put("Version", strVersion);
            pt.put("Caption", strCaption);
            pt.put("Clsid", strClsid);
            pt.put("Compnay", strCompnay);
            pt.put("Description", strDescription);
            pt.put("DllCaption", strDllCaption);
            pt.put("Dllpath", strDllpath);
            pt.put("ToolType", strToolType);
            pt.put("DeleteTime", strDeleteTime);
            root.add_child("IEToolBar.tool", pt);
        }

        if (PathFileExists(file_path.c_str()))
        {
            DeleteFile(file_path.c_str());
        }
        std::wofstream RecordFile;
        RecordFile.open(file_path, std::ios::app | std::ios::out | std::ios::binary);
        if (RecordFile.is_open())
        {
            RecordFile.close();
        }
        else
        {
            return false;
        }
        std::string strRecordFile = boost::locale::conv::from_utf(file_path, "UTF-8");
        boost::property_tree::write_xml(strRecordFile, root);
    }
    catch (std::exception &ex)
    {
        OutputDebugStringA(ex.what());
        return false;
    }
    return true;
}

bool readFFXMLFile(const std::wstring &file_path, std::map<std::wstring, FFToolbarRecord> &mapFFToolbarRecord)
{
    if (!PathFileExists(file_path.c_str())) 
    {
        return false;
    }
    std::string strRecordFile = boost::locale::conv::from_utf(file_path, "UTF-8");
    mapFFToolbarRecord.clear();
    boost::property_tree::ptree root;
    boost::property_tree::ptree node;

    try 
    {
        boost::property_tree::read_xml(strRecordFile, root);
        node = root.get_child("FFToolBar");
        boost::property_tree::ptree::const_iterator iteraEnd = node.end();
        for (boost::property_tree::ptree::const_iterator itera = node.begin(); itera != iteraEnd; ++itera)
        {
            std::string strVersion          = itera->second.get<std::string>("Version");
            std::string strIcon             = itera->second.get<std::string>("Icon");
            std::string strCaption          = itera->second.get<std::string>("Caption");
            std::string strClsid            = itera->second.get<std::string>("Clsid");
            std::string strCompnay          = itera->second.get<std::string>("Compnay");
            std::string strDescription      = itera->second.get<std::string>("Description");
            std::string strDllorFileCaption = itera->second.get<std::string>("DllorFileCaption");
            std::string strDllorFilepath    = itera->second.get<std::string>("DllorFilepath");
            std::string strToolType         = itera->second.get<std::string>("ToolType");
            std::string strDeleteTime       = itera->second.get<std::string>("DeleteTime");
            FFToolbarRecord stRecord;
            stRecord.wsVersion          = boost::locale::conv::to_utf<wchar_t>(strVersion, "UTF-8");
            stRecord.wsIcon             = boost::locale::conv::to_utf<wchar_t>(strIcon, "UTF-8");
            stRecord.wsCaption          = boost::locale::conv::to_utf<wchar_t>(strCaption, "UTF-8");
            stRecord.wsClsid            = boost::locale::conv::to_utf<wchar_t>(strClsid, "UTF-8");
            stRecord.wsCompnay          = boost::locale::conv::to_utf<wchar_t>(strCompnay, "UTF-8");
            stRecord.wsDescription      = boost::locale::conv::to_utf<wchar_t>(strDescription, "UTF-8");
            stRecord.wsDllorFileCaption = boost::locale::conv::to_utf<wchar_t>(strDllorFileCaption, "UTF-8");
            stRecord.wsDllorFilepath    = boost::locale::conv::to_utf<wchar_t>(strDllorFilepath, "UTF-8");
            stRecord.wsToolType         = boost::locale::conv::to_utf<wchar_t>(strToolType, "UTF-8");
            stRecord.wsDeleteTime       = boost::locale::conv::to_utf<wchar_t>(strDeleteTime, "UTF-8");
            mapFFToolbarRecord.insert(std::map<std::wstring, FFToolbarRecord>::value_type(stRecord.wsClsid, stRecord));
        }
    }
    catch (std::exception &ex)
    {
        OutputDebugStringA(ex.what());
        return false;
    }
    return true;
}

bool writeFFXMLFile(const std::wstring &file_path, std::map<std::wstring, FFToolbarRecord> &mapFFToolbarRecord)
{
    try
    {
        boost::property_tree::ptree root;
        for (auto itera = mapFFToolbarRecord.begin(); itera != mapFFToolbarRecord.end(); ++itera)
        {
            std::wstring wsCaption;
            std::wstring wsDescription;
            std::wstring wsCompnay;
            std::wstring wsVersion;
            std::wstring wsIcon;
            std::wstring wsClsid;
            std::wstring wsDllorFileCaption;
            std::wstring wsDllorFilepath;
            std::wstring wsToolType;
            std::wstring wsDeleteTime;

            boost::property_tree::ptree pt;
            std::string strVersion          = boost::locale::conv::from_utf(itera->second.wsVersion, "UTF-8");
            std::string strIcon             = boost::locale::conv::from_utf(itera->second.wsIcon, "UTF-8");
            std::string strCaption          = boost::locale::conv::from_utf(itera->second.wsCaption, "UTF-8");
            std::string strClsid            = boost::locale::conv::from_utf(itera->second.wsClsid, "UTF-8");
            std::string strCompnay          = boost::locale::conv::from_utf(itera->second.wsCompnay, "UTF-8");
            std::string strDescription      = boost::locale::conv::from_utf(itera->second.wsDescription, "UTF-8");
            std::string strDllorFileCaption = boost::locale::conv::from_utf(itera->second.wsDllorFileCaption, "UTF-8");
            std::string strDllorFilepath    = boost::locale::conv::from_utf(itera->second.wsDllorFilepath, "UTF-8");
            std::string strToolType         = boost::locale::conv::from_utf(itera->second.wsToolType, "UTF-8");
            std::string strDeleteTime       = boost::locale::conv::from_utf(itera->second.wsDeleteTime, "UTF-8");

            pt.put("Version", strVersion);
            pt.put("Icon", strIcon);
            pt.put("Caption", strCaption);
            pt.put("Clsid", strClsid);
            pt.put("Compnay", strCompnay);
            pt.put("Description", strDescription);
            pt.put("DllorFileCaption", strDllorFileCaption);
            pt.put("DllorFilepath", strDllorFilepath);
            pt.put("ToolType", strToolType);
            pt.put("DeleteTime", strDeleteTime);

            root.add_child("FFToolBar.tool", pt);
        }

        if (PathFileExists(file_path.c_str()))
        {
            DeleteFile(file_path.c_str());
        }
        std::wofstream RecordFile;
        RecordFile.open(file_path, std::ios::app | std::ios::out | std::ios::binary);
        if (RecordFile.is_open())
        {
            RecordFile.close();
        }
        else
        {
            return false;
        }
        std::string strRecordFile = boost::locale::conv::from_utf(file_path, "UTF-8");
        boost::property_tree::write_xml(strRecordFile, root);
    }
    catch (std::exception &ex)
    {
        OutputDebugStringA(ex.what());
        return false;
    }
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::wstring file_path = L"C:\\Users\\Silence\\AppData\\Local\\Anvisoft\\Anvi Slim Toolbar\\FFToobar\\toolBarRemoveInfo.xml";
    std::map<std::wstring, FFToolbarRecord> mapFFToolbarRecord;
    readFFXMLFile(file_path, mapFFToolbarRecord);
    writeFFXMLFile(L"C:\\1.XML", mapFFToolbarRecord);

    file_path = L"C:\\Users\\Silence\\AppData\\Local\\Anvisoft\\Anvi Slim Toolbar\\IEToobar\\toolBarRemoveInfo.xml";
    std::map<std::wstring, IEToolbarRecord> mapIEToolbarRecord;
    readIEXMLFile(file_path, mapIEToolbarRecord);
    writeIEXMLFile(L"C:\\2.XML", mapIEToolbarRecord);
    return 0;

    ToolbarRecord testToolbarRecord(L"");
    stDeleteToolbarForeverRecord stRecord;
    stRecord.wsBrowser = L"IE";
    stRecord.wsType = L"BHO";
    stRecord.wsGuid = L"312312312-312312-312312-23123";
    stRecord.wsCaption = L"TEST";
    stRecord.wsReg1KEY = L"REG111";
    stRecord.wsReg1SubKEY = L"11111111111";
    stRecord.wsReg2KEY = L"REG222";
    stRecord.wsReg2SubKEY = L"222222222222";
    stRecord.wsDllPath = L"C://1.TXT";
    stRecord.wsDeleteDate = L"2014-11-11";
    testToolbarRecord.AddRecordForDeleteToolbarForever(stRecord);
    stRecord.wsGuid = L"312312312-312312-312312-23124";
    testToolbarRecord.AddRecordForDeleteToolbarForever(stRecord);
    stRecord.wsGuid = L"312312312-312312-312312-23125";
    testToolbarRecord.AddRecordForDeleteToolbarForever(stRecord);

    std::map<std::wstring, stDeleteToolbarForeverRecord> mapToolbarRecord;
    testToolbarRecord.ListRecordForDeleteToolbarForever(mapToolbarRecord);
	return 0;
}

