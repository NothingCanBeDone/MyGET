#include "StdAfx.h"
#include "ToolbarRecord.h"
#include <shlobj.h>

ToolbarRecord::ToolbarRecord(const std::wstring &wsRecordFilePath):m_RecordFile(wsRecordFilePath)
{
}

ToolbarRecord::~ToolbarRecord(void)
{
}

bool ToolbarRecord::GetRecord(std::map<std::wstring, stDeleteToolbarForeverRecord> &mapToolbarRecord)
{
    if (!PathFileExists(m_RecordFile.c_str()))
    {
        return false;
    }
    std::string strRecordFile = boost::locale::conv::from_utf(m_RecordFile, "UTF-8");
    mapToolbarRecord.clear();

    boost::property_tree::ptree root;
    boost::property_tree::ptree node;
    try
    {
        boost::property_tree::read_xml(strRecordFile, root);
        node = root.get_child("ToolbarRecord");
    }
    catch (...)
    {
        return false;
    }
    boost::property_tree::ptree::const_iterator iteraEnd = node.end();
    for (boost::property_tree::ptree::const_iterator itera = node.begin(); itera != iteraEnd; ++itera)
    {
        try
        {
            std::string strBrowser      = itera->second.get<std::string>("Browser");
            std::string strType         = itera->second.get<std::string>("Type");
            std::string strGuid         = itera->second.get<std::string>("Guid");
            std::string strCaption      = itera->second.get<std::string>("Caption");
            std::string strCompnay      = itera->second.get<std::string>("Compnay");
            std::string strVersion      = itera->second.get<std::string>("Version");
            std::string strReg1KEY      = itera->second.get<std::string>("Reg1KEY");
            std::string strReg1SubKEY   = itera->second.get<std::string>("Reg1SubKEY");
            std::string strReg2KEY      = itera->second.get<std::string>("Reg2KEY");
            std::string strReg2SubKEY   = itera->second.get<std::string>("Reg2SubKEY");
            std::string strDllPath      = itera->second.get<std::string>("DllPath");
            std::string strDeleteDate   = itera->second.get<std::string>("DeleteDate");
            stDeleteToolbarForeverRecord stRecord;
            stRecord.wsBrowser      = boost::locale::conv::to_utf<wchar_t>(strBrowser, "UTF-8");
            stRecord.wsType         = boost::locale::conv::to_utf<wchar_t>(strType, "UTF-8");
            stRecord.wsGuid         = boost::locale::conv::to_utf<wchar_t>(strGuid, "UTF-8");
            stRecord.wsCaption      = boost::locale::conv::to_utf<wchar_t>(strCaption, "UTF-8");
            stRecord.wsCompnay      = boost::locale::conv::to_utf<wchar_t>(strCompnay, "UTF-8");
            stRecord.wsVersion      = boost::locale::conv::to_utf<wchar_t>(strVersion, "UTF-8");
            stRecord.wsReg1KEY      = boost::locale::conv::to_utf<wchar_t>(strReg1KEY, "UTF-8");
            stRecord.wsReg1SubKEY   = boost::locale::conv::to_utf<wchar_t>(strReg1SubKEY, "UTF-8");
            stRecord.wsReg2KEY      = boost::locale::conv::to_utf<wchar_t>(strReg2KEY, "UTF-8");
            stRecord.wsReg2SubKEY   = boost::locale::conv::to_utf<wchar_t>(strReg2SubKEY, "UTF-8");
            stRecord.wsDllPath      = boost::locale::conv::to_utf<wchar_t>(strDllPath, "UTF-8");
            stRecord.wsDeleteDate   = boost::locale::conv::to_utf<wchar_t>(strDeleteDate, "UTF-8");
            mapToolbarRecord.insert(std::map<std::wstring, stDeleteToolbarForeverRecord>::value_type(stRecord.wsGuid, stRecord));
        }
        catch (...)
        {
            return false;	
        }
    }
    return true;
}

bool ToolbarRecord::SaveRecord(std::map<std::wstring, stDeleteToolbarForeverRecord> &mapToolbarRecord)
{
    if (mapToolbarRecord.size() == 0)
    {
        DeleteFile(m_RecordFile.c_str());
        return true;
    }
    boost::property_tree::ptree root;
    for (auto itera = mapToolbarRecord.begin(); itera != mapToolbarRecord.end(); ++itera)
    {
        stDeleteToolbarForeverRecord stRecord;
        boost::property_tree::ptree pt;
        std::string strBrowser      = boost::locale::conv::from_utf(itera->second.wsBrowser, "UTF-8");
        std::string strType         = boost::locale::conv::from_utf(itera->second.wsType, "UTF-8");
        std::string strGuid         = boost::locale::conv::from_utf(itera->second.wsGuid, "UTF-8");
        std::string strCaption      = boost::locale::conv::from_utf(itera->second.wsCaption, "UTF-8");
        std::string strCompnay      = boost::locale::conv::from_utf(itera->second.wsCompnay, "UTF-8");
        std::string strVersion      = boost::locale::conv::from_utf(itera->second.wsVersion, "UTF-8");
        std::string strReg1KEY      = boost::locale::conv::from_utf(itera->second.wsReg1KEY, "UTF-8");
        std::string strReg1SubKEY   = boost::locale::conv::from_utf(itera->second.wsReg1SubKEY, "UTF-8");
        std::string strReg2KEY      = boost::locale::conv::from_utf(itera->second.wsReg2KEY, "UTF-8");
        std::string strReg2SubKEY   = boost::locale::conv::from_utf(itera->second.wsReg2SubKEY, "UTF-8");
        std::string strDllPath      = boost::locale::conv::from_utf(itera->second.wsDllPath, "UTF-8");
        std::string strDeleteDate   = boost::locale::conv::from_utf(itera->second.wsDeleteDate, "UTF-8");
        pt.put("Browser", strBrowser);
        pt.put("Type", strType);
        pt.put("Guid", strGuid);
        pt.put("Caption", strCaption);
        pt.put("Compnay", strCompnay);
        pt.put("Version", strVersion);
        pt.put("Reg1KEY", strReg1KEY);
        pt.put("Reg1SubKEY", strReg1SubKEY);
        pt.put("Reg2KEY", strReg2KEY);
        pt.put("Reg2SubKEY", strReg2SubKEY);
        pt.put("DllPath", strDllPath);
        pt.put("DeleteDate", strDeleteDate);
        root.add_child("ToolbarRecord.tool", pt);
    }

    try
    {
        if (PathFileExists(m_RecordFile.c_str()))
        {
            std::wstring wstrBakFile(m_RecordFile);
            wstrBakFile.append(L".bak");
            CopyFile(m_RecordFile.c_str(), wstrBakFile.c_str(), FALSE);
            BOOL bResult = DeleteFile(m_RecordFile.c_str());
        }
        else
        {
            std::wofstream RecordFile;
            RecordFile.open(m_RecordFile, std::ios::app | std::ios::out | std::ios::binary);
            if (RecordFile.is_open())
            {
                RecordFile.close();
            }
            else
            {
                return false;
            }
        }
        std::string strRecordFile = boost::locale::conv::from_utf(m_RecordFile, "UTF-8");
        boost::property_tree::write_xml(strRecordFile, root);
    }
    catch (...)
    {
        std::wstring wstrBakFile(m_RecordFile);
        wstrBakFile.append(L".bak");
        CopyFile(wstrBakFile.c_str(), m_RecordFile.c_str(), FALSE);
        return false;
    }
    return true;
}

bool ToolbarRecord::ListRecordForDeleteToolbarForever(std::map<std::wstring, stDeleteToolbarForeverRecord> &mapToolbarRecord)
{
    return GetRecord(mapToolbarRecord);
}

bool ToolbarRecord::AddRecordForDeleteToolbarForever(const stDeleteToolbarForeverRecord &stRecord)
{
    try
    {
        if (PathFileExists(m_RecordFile.c_str()))
        {
            if (!GetRecord(m_ToolbarRecord))
            {
                return false;
            }
        }
        m_ToolbarRecord.insert(std::map<std::wstring, stDeleteToolbarForeverRecord>::value_type(stRecord.wsGuid, stRecord));
        if (!SaveRecord(m_ToolbarRecord))
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool ToolbarRecord::DelRecordForDeleteToolbarForever(const stDeleteToolbarForeverRecord &stRecord)
{
    try
    {
        if (!GetRecord(m_ToolbarRecord))
        {
            return false;
        }
        auto itera = m_ToolbarRecord.find(stRecord.wsGuid);
        if (itera == m_ToolbarRecord.end())
        {
            return false;
        }
        m_ToolbarRecord.erase(stRecord.wsGuid);
        if (!SaveRecord(m_ToolbarRecord))
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}