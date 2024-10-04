#pragma once
#include <stdint.h>
#include <string>

class CPath
{
    // Enums
public:
    enum DIR_CURRENT_DIRECTORY
    {
        CURRENT_DIRECTORY = 1
    };
#ifdef _WIN32
    enum DIR_MODULE_DIRECTORY
    {
        MODULE_DIRECTORY = 2
    };
    enum DIR_MODULE_FILE
    {
        MODULE_FILE = 3
    };
#endif

    enum
    {
        FIND_ATTRIBUTE_ALLFILES = 0xFFFF, // Search include all files
        FIND_ATTRIBUTE_FILES = 0x0000,    // File can be read or written to without restriction
        FIND_ATTRIBUTE_SUBDIR = 0x0010,   // Subdirectories
    };

    // Attributes
private:
    std::wstring m_strPath;
#ifdef _WIN32
    void * m_hFindFile;
    static void * m_hInst;
#else
    void * m_OpenedDir;
    std::wstring m_FindWildcard;
#endif
    uint32_t m_dwFindFileAttributes;

public:
    // Methods

    // Construction / destruction
    CPath();
    CPath(const CPath & rPath);
    CPath(const wchar_t * lpszPath);
    CPath(const wchar_t * lpszPath, const wchar_t * NameExten);
    CPath(const std::wstring & strPath);
    CPath(const std::wstring & strPath, const wchar_t * NameExten);
    CPath(const std::wstring & strPath, const std::wstring & NameExten);

    CPath(DIR_CURRENT_DIRECTORY sdt, const wchar_t * NameExten = nullptr);
#ifdef _WIN32
    CPath(DIR_MODULE_DIRECTORY sdt, const wchar_t * NameExten = nullptr);
    CPath(DIR_MODULE_FILE sdt);
#endif
    virtual ~CPath();

    // Operators
    CPath & operator=(const CPath & rPath);
    CPath & operator=(const wchar_t * lpszPath);
    CPath & operator=(const std::wstring & strPath);
    bool operator==(const CPath & rPath) const;
    bool operator!=(const CPath & rPath) const;
    operator const wchar_t *() const;
    operator const std::wstring &()
    {
        return m_strPath;
    }

    // Get path components
#ifdef _WIN32
    void GetDriveDirectory(std::wstring & rDriveDirectory) const;
    std::wstring GetDriveDirectory(void) const;
#endif
    void GetDirectory(std::wstring & rDirectory) const;
    std::wstring GetDirectory(void) const;
    void GetName(std::wstring & rName) const;
    std::wstring GetName(void) const;
    void GetNameExtension(std::wstring & rNameExtension) const;
    std::wstring GetNameExtension(void) const;
    void GetExtension(std::wstring & rExtension) const;
    std::wstring GetExtension(void) const;
    void GetLastDirectory(std::wstring & rDirectory) const;
    std::wstring GetLastDirectory(void) const;
    void GetFullyQualified(std::wstring & rFullyQualified) const;
#ifdef _WIN32
    void GetComponents(std::wstring * pDrive = nullptr, std::wstring * pDirectory = nullptr, std::wstring * pName = nullptr, std::wstring * pExtension = nullptr) const;
#else
    void GetComponents(std::wstring * pDirectory = nullptr, std::wstring * pName = nullptr, std::wstring * pExtension = nullptr) const;
#endif
    // Get other state
    bool IsEmpty() const
    {
        return m_strPath.empty();
    }
    bool IsRelative() const;

    // Set path components
#ifdef _WIN32
    void SetDrive(wchar_t chDrive);
    void SetDriveDirectory(const wchar_t * lpszDriveDirectory);
#endif
    void SetDirectory(const wchar_t * lpszDirectory, bool bEnsureAbsolute = false);
    void SetName(const wchar_t * lpszName);
    void SetName(int iName);
    void SetNameExtension(const wchar_t * lpszNameExtension);
    void SetExtension(const wchar_t * lpszExtension);
    void SetExtension(int iExtension);
    void AppendDirectory(const wchar_t * lpszSubDirectory);
    void UpDirectory(std::wstring * pLastDirectory = nullptr);
#ifdef _WIN32
    void SetComponents(const wchar_t * lpszDrive, const wchar_t * lpszDirectory, const wchar_t * lpszName, const wchar_t * lpszExtension);
#else
    void SetComponents(const wchar_t * lpszDirectory, const wchar_t * lpszName, const wchar_t * lpszExtension);
#endif
    // Set whole path
    void Empty()
    {
        m_strPath.erase();
    }
    void CurrentDirectory();
#ifdef _WIN32
    void Module();
    void Module(void * hInstance);
    void ModuleDirectory();
    void ModuleDirectory(void * hInstance);
#endif

    // Directory information
    bool IsDirectory() const;
    bool DirectoryExists() const;

    // File information
    bool IsFile() const
    {
        return !IsDirectory();
    }
    bool Exists() const;
#ifdef _WIN32
    bool SelectFile(void * hwndOwner, const wchar_t * InitialDir, const wchar_t * FileFilter, bool FileMustExist);
#endif

    // Directory operations
    bool DirectoryCreate(bool bCreateIntermediates = true);
    bool ChangeDirectory();
    CPath & NormalizePath(CPath BaseDir);

    // File operations
    bool Delete(bool bEvenIfReadOnly = true) const;
    bool CopyTo(const wchar_t * lpcszTargetFile, bool bOverwrite = true);
    bool MoveTo(const wchar_t * lpcszTargetFile, bool bOverwrite = true);

    // Finders
    bool FindFirst(uint32_t dwAttributes = 0);
    bool FindNext();

    // Helpers
#ifdef _WIN32
    static void SethInst(void * hInst);
    static void * GethInst();
#endif

private:
    // Setup and cleanup
    inline void Init();
    inline void Exit();

    bool AttributesMatch(uint32_t dwTargetAttributes, uint32_t dwFileAttributes);

    void cleanPathString(std::wstring & rDirectory) const;
    void StripLeadingChar(std::wstring & rText, wchar_t chLeading) const;
    void StripLeadingBackslash(std::wstring & Directory) const;
    void StripTrailingChar(std::wstring & rText, wchar_t chTrailing) const;
    void StripTrailingBackslash(std::wstring & rDirectory) const;
    void EnsureTrailingBackslash(std::wstring & Directory) const;
    void EnsureLeadingBackslash(std::wstring & Directory) const;
#ifndef _WIN32
    bool wildcmp(const wchar_t * wild, const wchar_t * string);
#endif
};
