#pragma once

// P0607R0 Inline Variables For The STL
#if _HAS_CXX17
#define _INLINE_VAR inline
#else // _HAS_CXX17
#define _INLINE_VAR
#endif // _HAS_CXX17

namespace SPC
{
    template <typename _Ty>
    class CppDeleter { // default deleter for CAutoPtr
    public:
        constexpr CppDeleter() noexcept = default;

        CppDeleter(const CppDeleter<_Ty>&) noexcept {}
        void operator()(_Ty* _Ptr) const noexcept {
            static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
            delete _Ptr;
        }
    };

    template <typename _Ty, ULONG _PoolTag>
    class KernelDeleter { // default deleter for CAutoPtr in kernel
    public:
        static constexpr ULONG pool_tag = _PoolTag;
        constexpr KernelDeleter() noexcept = default;

        KernelDeleter(const KernelDeleter<_Ty, _PoolTag>&) noexcept
        {}
        void operator()(_Ty* _Ptr) const noexcept {
            static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
            ExFreePoolWithTag(_Ptr, _PoolTag);
        }
    };

    template<typename _Ty, ULONG _PoolTag,
        class _Dx = KernelDeleter<_Ty, _PoolTag>>
        class CKernalAutoPtr {
        public:
            using DataType = _Ty;
            using DeleterType = _Dx;

            CKernalAutoPtr(DataType* ptr = nullptr) noexcept {
                this->Ptr = ptr;
            }

            ~CKernalAutoPtr() noexcept {
                if (nullptr != this->Ptr)
                    this->Deleter(this->Ptr);
            }

            CKernalAutoPtr& operator=(CKernalAutoPtr<_Ty, _PoolTag, _Dx>&& _Right) noexcept {
                reset(_Right.release());
                return *this;
            }

            operator _Ty* () const
            {
                return this->Ptr;
            }

            _Ty* operator->() const noexcept
            {
                return this->Ptr;
            }

            void set(_Ty* ptr)  noexcept {
                this->Ptr = ptr;
            }

            _Ty* get() const noexcept {
                return this->Ptr;
            }

            void reset(_Ty* new_ptr = nullptr) noexcept {
                _Ty* old_ptr = nullptr;
                old_ptr = this->Ptr;
                this->Ptr = new_ptr;

                if (old_ptr)
                {
                    this->Deleter(old_ptr);
                }
            }

            _Ty* release() noexcept {
                _Ty* old_ptr = this->Ptr;
                this->Ptr = nullptr;
                return old_ptr;
            }

            bool is_empty() noexcept {
                return (nullptr == this->Ptr);
            }

        protected:
            DataType* Ptr = nullptr;
            DeleterType Deleter;

            friend class CKernalAutoPtr;
    };
}

using SPC::CKernalAutoPtr;
