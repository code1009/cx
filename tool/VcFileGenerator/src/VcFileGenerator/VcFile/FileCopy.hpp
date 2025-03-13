#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Generator;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class SourceFileList
{
public:
	bool shouldIgnoreFile(const std::wstring& filePath);
	std::vector<std::wstring> getFileList(const std::wstring& directoryPath);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class FileCopy
{
public:
	class Item
	{
	public:
		std::wstring _Source;
		std::wstring _Target;

	public:
		Item(std::wstring source, std::wstring target) :
			_Source(source),
			_Target(target)
		{
		}
	};

public:
	Generator* _Generator {nullptr};

public:
	std::vector<std::shared_ptr<Item>> _Items;

public:
	explicit FileCopy(Generator* generator);

public:
	bool copy(void);

public:
	void makeItems(void);
	bool copyItems(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
