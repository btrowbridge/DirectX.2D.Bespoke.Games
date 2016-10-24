#include "pch.h"
#include "ContentTypeReaderManager.h"

using namespace std;

namespace Library
{
	map<uint64_t, shared_ptr<AbstractContentTypeReader>> ContentTypeReaderManager::sContentTypeReaders;
	bool ContentTypeReaderManager::sInitialized;

	const map<uint64_t, shared_ptr<AbstractContentTypeReader>>& ContentTypeReaderManager::ContentTypeReaders()
	{
		return sContentTypeReaders;
	}

	bool ContentTypeReaderManager::AddContentTypeReader(shared_ptr<AbstractContentTypeReader> reader)
	{
		return sContentTypeReaders.emplace(reader->TargetTypeId(), reader).second;
	}

	void ContentTypeReaderManager::Initialize(Game& game)
	{
		// Add known content type readers
		AddContentTypeReader(make_shared<Texture2DReader>(game));
		AddContentTypeReader(make_shared<VertexShaderReader>(game));
		AddContentTypeReader(make_shared<PixelShaderReader>(game));

		sInitialized = true;
	}

	void ContentTypeReaderManager::Shutdown()
	{
		sContentTypeReaders.clear();
		sInitialized = false;
	}
}