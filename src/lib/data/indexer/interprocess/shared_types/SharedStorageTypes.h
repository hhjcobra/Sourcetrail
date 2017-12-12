#ifndef SHARED_STORAGE_TYPES_H
#define SHARED_STORAGE_TYPES_H

#include "data/storage/type/StorageCommentLocation.h"
#include "data/storage/type/StorageComponentAccess.h"
#include "data/storage/type/StorageEdge.h"
#include "data/storage/type/StorageError.h"
#include "data/storage/type/StorageFile.h"
#include "data/storage/type/StorageLocalSymbol.h"
#include "data/storage/type/StorageNode.h"
#include "data/storage/type/StorageOccurrence.h"
#include "data/storage/type/StorageSourceLocation.h"
#include "data/storage/type/StorageSymbol.h"
#include "utility/types.h"
#include "utility/interprocess/SharedMemory.h"

// macro creating SharedStorageType from StorageType
// - arguments: StorageType & SharedStorageType
// - defines: conversion functions toShared() & fromShared()

#define CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(__type__, __shared_type__) \
	typedef __type__ __shared_type__; \
\
	inline const __shared_type__& toShared(const __type__& instance, SharedMemory::Allocator* allocator) \
	{ \
		return instance; \
	} \
\
	inline const __type__& fromShared(const __shared_type__& instance) \
	{ \
		return instance; \
	}

CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(	StorageEdge,				SharedStorageEdge )
CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(	StorageSymbol,				SharedStorageSymbol )
CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(	StorageSourceLocation,		SharedStorageSourceLocation )
CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(	StorageOccurrence,			SharedStorageOccurrence )
CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(	StorageComponentAccessData,		SharedStorageComponentAccessData)
CONVERT_STORAGE_TYPE_TO_SHARED_TYPE(	StorageCommentLocationData,		SharedStorageCommentLocationData)


struct SharedStorageNode
{
	SharedStorageNode(Id id, int type, const std::string& serializedName, SharedMemory::Allocator* allocator)
		: id(id)
		, type(type)
		, serializedName(serializedName.c_str(), allocator)
	{}

	Id id;
	int type;
	SharedMemory::String serializedName;
};

inline SharedStorageNode toShared(const StorageNode& node, SharedMemory::Allocator* allocator)
{
	return SharedStorageNode(node.id, node.type, node.serializedName, allocator);
}

inline StorageNode fromShared(const SharedStorageNode& node)
{
	return StorageNode(node.id, node.type, node.serializedName.c_str());
}


struct SharedStorageFile
{
	SharedStorageFile(
		Id id, const std::string& filePath, const std::string& modificationTime, bool complete, SharedMemory::Allocator* allocator
	)
		: id(id)
		, filePath(filePath.c_str(), allocator)
		, modificationTime(modificationTime.c_str(), allocator)
		, complete(complete)
	{}

	Id id;
	SharedMemory::String filePath;
	SharedMemory::String modificationTime;
	bool complete;
};

inline SharedStorageFile toShared(const StorageFile& file, SharedMemory::Allocator* allocator)
{
	return SharedStorageFile(file.id, file.filePath, file.modificationTime, file.complete, allocator);
}

inline StorageFile fromShared(const SharedStorageFile& file)
{
	return StorageFile(file.id, file.filePath.c_str(), file.modificationTime.c_str(), file.complete);
}


struct SharedStorageLocalSymbol
{
	SharedStorageLocalSymbol(Id id, const std::string& name, SharedMemory::Allocator* allocator)
		: id(id)
		, name(name.c_str(), allocator)
	{}

	Id id;
	SharedMemory::String name;
};

inline SharedStorageLocalSymbol toShared(const StorageLocalSymbol& symbol, SharedMemory::Allocator* allocator)
{
	return SharedStorageLocalSymbol(symbol.id, symbol.name, allocator);
}

inline StorageLocalSymbol fromShared(const SharedStorageLocalSymbol& symbol)
{
	return StorageLocalSymbol(symbol.id, symbol.name.c_str());
}


struct SharedStorageErrorData
{
	SharedStorageErrorData(
		const std::string& message,
		const std::string& filePath,
		uint lineNumber,
		uint columnNumber,
		bool fatal,
		bool indexed,
		SharedMemory::Allocator* allocator
	)
		: message(message.c_str(), allocator)
		, filePath(filePath.c_str(), allocator)
		, lineNumber(lineNumber)
		, columnNumber(columnNumber)
		, fatal(fatal)
		, indexed(indexed)
	{}

	SharedMemory::String message;

	SharedMemory::String filePath;
	uint lineNumber;
	uint columnNumber;

	bool fatal;
	bool indexed;
};

inline SharedStorageErrorData toShared(const StorageErrorData& error, SharedMemory::Allocator* allocator)
{
	return SharedStorageErrorData(
		error.message, error.filePath.str(),
		error.lineNumber, error.columnNumber, error.fatal, error.indexed, allocator);
}

inline StorageErrorData fromShared(const SharedStorageErrorData& error)
{
	return StorageErrorData(
		error.message.c_str(), FilePath(error.filePath.c_str()),
		error.lineNumber, error.columnNumber, error.fatal, error.indexed);
}

#endif // SHARED_STORAGE_TYPES_H
