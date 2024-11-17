[TOC]
new 和 delete 的本质就是调了 malloc 和 free

# new
`stdc++v3/libsupc++/new_op.cc`
```c++
_GLIBCXX_WEAK_DEFINITION void *
operator new (std::size_t sz) _GLIBCXX_THROW (std::bad_alloc)
{
  void *p;

  /* malloc (0) is unpredictable; avoid it.  */
  if (sz == 0)
    sz = 1;

  while (__builtin_expect ((p = malloc (sz)) == 0, false))
    {
      new_handler handler = std::get_new_handler ();
      if (! handler)
	_GLIBCXX_THROW_OR_ABORT(bad_alloc());
      handler ();
    }

  return p;
}
```
1. 注意 `if (sz == 0)` 逻辑, 对应对象模型提到, 无任何非静态成员的类, size 是 1

# delete
`stdc++v3/libsupc++/del_op.cc`
```c++
_GLIBCXX_WEAK_DEFINITION void
operator delete(void* ptr) _GLIBCXX_USE_NOEXCEPT
{
  std::free(ptr);
}
```