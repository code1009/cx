/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void EditSeed::notifyModified(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"EditSeed::notifyModified";
		modifiedEventListener.notify(
			EditSeed_ModifiedEvent,
			nullptr
		);
	}

	//=======================================================================
	bool EditSeed::isModified() const
	{
		return _CommitSeed != _CurrentSeed;
	}

	void EditSeed::commit()
	{
		if (_CommitSeed == _CurrentSeed)
		{
			return;
		}
		_CommitSeed = _CurrentSeed;
		//CX_RUNTIME_LOG(cxLTrace) << L"EditSeed::commit=" << _CurrentSeed;
		notifyModified();
	}

	void EditSeed::touch()
	{
		++_CurrentSeed;
		//CX_RUNTIME_LOG(cxLTrace) << L"EditSeed::touch=" << _CurrentSeed;
		notifyModified();
	}

	void EditSeed::restore(Seed value)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"EditSeed::restore=" << value;
		if (_CurrentSeed == value)
		{
			return;
		}
		_CurrentSeed = value;
		notifyModified();
	}

	EditSeed::Seed EditSeed::commitSeed() const
	{
		return _CommitSeed;
	}

	EditSeed::Seed EditSeed::currentSeed() const
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"EditSeed::currentSeed=" << _CurrentSeed;
		return _CurrentSeed;
	}
}
