#include "Hoshino/Vfs/Blob.h"

namespace Hoshino::Vfs
{

	Blob::Blob(void* data, size_t size) : m_data(data), m_size(size) {}

	const void* Blob::data() const
	{
		return m_data;
	}

	size_t Blob::size() const
	{
		return m_size;
	}

	Blob::~Blob()
	{
		if (m_data)
		{
			free(m_data);
			m_data = nullptr;
		}

		m_size = 0;
	}
} // namespace Hoshino::Vfs