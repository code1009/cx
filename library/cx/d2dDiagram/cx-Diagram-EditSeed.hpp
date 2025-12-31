#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditSeed
	{
	public:
		cx::ev::key::EventListener modifiedEventListener;

	public:
		using Seed = std::size_t;

	private:
		Seed _CurrentSeed = 0;
		Seed _CommitSeed = 0;

	public:
		EditSeed() = default;

	public:
		EditSeed(const EditSeed& other) = delete;
		EditSeed& operator=(const EditSeed& other) = delete;
		EditSeed(EditSeed&& other) noexcept = delete;
		EditSeed& operator=(EditSeed&& other) noexcept = delete;

	public:
		void notifyModified(void);

	public:
		bool isModified() const;
		void commit();
		void touch();
		void restore(Seed value);

		Seed commitSeed() const;
		Seed currentSeed() const;
	};
}
