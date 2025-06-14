#include "HoshinoMin.h"

namespace Hoshino::Vfs
{

	// A blob is a package for untyped data, typically read from a file.
	class IBlob
	{
	public:
		virtual ~IBlob() = default;
		[[nodiscard]] virtual const void* data() const = 0;
		[[nodiscard]] virtual size_t size() const = 0;

		// Returns true if the provided blob contains no data.
		// Note: the previous version of this function was called IsEmpty (capital I)
		// and it returned the inverse of what it was supposed to.
		static bool isEmpty(IBlob const* blob)
		{
			return blob == nullptr || blob->data() == nullptr || blob->size() == 0;
		}
	};

	// Specific blob implementation that owns the data and frees it when deleted.
	class Blob : public IBlob
	{
	private:
		void* m_data;
		size_t m_size;

	public:
		Blob(void* data, size_t size);
		~Blob() override;
		[[nodiscard]] const void* data() const override;
		[[nodiscard]] size_t size() const override;
	};
} // namespace Hoshino::Vfs