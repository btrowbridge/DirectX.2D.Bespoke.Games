#pragma once

#include "RTTI.h"
#include <memory>

namespace Library
{
	class Game;

	class AbstractContentTypeReader abstract : public RTTI
	{
		RTTI_DECLARATIONS(AbstractContentTypeReader, RTTI)

	public:
		virtual ~AbstractContentTypeReader() = default;
		AbstractContentTypeReader(const AbstractContentTypeReader&) = default;
		AbstractContentTypeReader& operator=(const AbstractContentTypeReader&) = default;
		AbstractContentTypeReader(AbstractContentTypeReader&&) = default;
		AbstractContentTypeReader& operator=(AbstractContentTypeReader&&) = default;

		std::uint64_t TargetTypeId() const;
		virtual std::shared_ptr<RTTI> Read(const std::wstring& assetName) = 0;

	protected:
		AbstractContentTypeReader(Game& game, const std::uint64_t targetTypeId);

		Game* mGame;
		const std::uint64_t mTargetTypeId;
	};

	template <typename T>
	class ContentTypeReader abstract : public AbstractContentTypeReader
	{
	public:		
		ContentTypeReader(const ContentTypeReader&) = default;
		ContentTypeReader& operator=(const ContentTypeReader&) = default;
		ContentTypeReader(ContentTypeReader&&) = default;
		ContentTypeReader& operator=(ContentTypeReader&&) = default;
		virtual ~ContentTypeReader() = default;

		virtual std::shared_ptr<RTTI> Read(const std::wstring& assetName) override;

	protected:
		ContentTypeReader(Game& game, const std::uint64_t targetTypeId);

		virtual std::shared_ptr<T> _Read(const std::wstring& assetName) = 0;
	};
}

#include "ContentTypeReader.inl"