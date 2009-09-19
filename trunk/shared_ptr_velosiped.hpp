#ifndef SHARED_PTR_VELOSIPED_HPP
#define SHARED_PTR_VELOSIPED_HPP

#include <typeinfo>
#include <memory>
#include <functional>

namespace velosiped
{

template<typename x_Tp>
struct remove_pointer
{ typedef x_Tp type; };

template<typename x_Tp>
struct remove_pointer<x_Tp*>
{ typedef x_Tp type; };

template<typename x_Tp>
struct remove_pointer<x_Tp* const>
{ typedef x_Tp type; };

template<typename x_Tp>
struct remove_pointer<x_Tp* volatile>
{ typedef x_Tp type; };

template<typename x_Tp>
struct remove_pointer<x_Tp* const volatile>
{ typedef x_Tp type; };

class x_Sp_counted_base
{
  public:
    x_Sp_counted_base ():
    x_M_use_count (1), x_M_weak_count (1)
    {
    }

    virtual ~ x_Sp_counted_base ()
    {
    }

    virtual void x_M_dispose () = 0;

    virtual void x_M_destroy ()
    {
      delete this;
    }

    virtual void *x_M_get_deleter (const std::type_info &) = 0;

    void x_M_add_ref_copy ()
    {
      /*x__gnu_cxx::x__atomic_add_dispatch (&x_M_use_count, 1);*/ x_M_use_count++;
    }

    void x_M_add_ref_lock ();

    void x_M_release ()
    {
      if (/*x__gnu_cxx::x__exchange_and_add_dispatch (&x_M_use_count, -1)*/ x_M_use_count-- == 1)
      {
        x_M_dispose ();

        if ( /*x__gnu_cxx::x__exchange_and_add_dispatch (&x_M_weak_count, -1)*/ x_M_weak_count-- == 1)
          x_M_destroy ();
      }
    }

    void x_M_weak_add_ref ()
    {
      /*x__gnu_cxx::x__atomic_add_dispatch (&x_M_weak_count, 1);*/ x_M_weak_count++;
    }

    void x_M_weak_release ()
    {
      if ( /*x__gnu_cxx::x__exchange_and_add_dispatch (&x_M_weak_count, -1)*/ x_M_weak_count-- == 1)
      {
        x_M_destroy ();
      }
    }

    long x_M_get_use_count () const
    {
      return x_M_use_count;
    }

  private:
    x_Sp_counted_base (x_Sp_counted_base const &);
    x_Sp_counted_base & operator= (x_Sp_counted_base const &);

    /*x_Atomic_word*/ long x_M_use_count;
    /*x_Atomic_word*/ long x_M_weak_count;
};

template < typename x_Ptr, typename x_Deleter> class x_Sp_counted_base_impl:
public x_Sp_counted_base
{
  public:

    x_Sp_counted_base_impl (x_Ptr x__p, x_Deleter x__d):
    x_M_ptr (x__p), x_M_del (x__d)
    {
    }

    virtual void x_M_dispose ()
    {
      x_M_del (x_M_ptr);
    }

    virtual void *x_M_get_deleter (const std::type_info & x__ti)
    {
      return x__ti == typeid (x_Deleter) ? &x_M_del : 0;
    }

  private:
    x_Sp_counted_base_impl (const x_Sp_counted_base_impl &);
    x_Sp_counted_base_impl & operator= (const x_Sp_counted_base_impl &);

    x_Ptr x_M_ptr;
    x_Deleter x_M_del;
};

template < typename x_Tp > struct x_Sp_deleter
{
  typedef void result_type;
  typedef x_Tp *argument_type;
  void operator  () (x_Tp * x__p) const
  {
    delete x__p;
  }
};

class x__shared_count
{
  public:
    x__shared_count ():
    x_M_pi (0)
    {
    }

    template < typename x_Ptr > x__shared_count (x_Ptr x__p):
    x_M_pi (0)
    {
      try
      {
        typedef typename remove_pointer < x_Ptr >::type x_Tp;
        x_M_pi =
          new x_Sp_counted_base_impl < x_Ptr, x_Sp_deleter < x_Tp > >
          (x__p, x_Sp_deleter < x_Tp > ());
      }
      catch (...)
      {
        delete x__p;
        throw;
      }
    }

    template < typename x_Ptr, typename x_Deleter > x__shared_count (x_Ptr x__p, x_Deleter x__d):
    x_M_pi (0)
    {
      try
      {
        x_M_pi = new x_Sp_counted_base_impl < x_Ptr, x_Deleter > (x__p, x__d);
      }
      catch (...)
      {
        x__d (x__p);
        throw;
      }
    }

    template < typename x_Tp > explicit x__shared_count (std::auto_ptr < x_Tp > &x__r):
    x_M_pi (new x_Sp_counted_base_impl < x_Tp *, x_Sp_deleter < x_Tp > > (x__r.get (), x_Sp_deleter < x_Tp > ()))
    {
      x__r.release ();
    }

    ~x__shared_count ()
    {
      if (x_M_pi != 0)
        x_M_pi->x_M_release ();
    }

    x__shared_count (const x__shared_count & x__r):
    x_M_pi (x__r.x_M_pi)
    {
      if (x_M_pi != 0)
        x_M_pi->x_M_add_ref_copy ();
    }

    x__shared_count & operator= (const x__shared_count & x__r)
    {
      x_Sp_counted_base *x__tmp = x__r.x_M_pi;
      if (x__tmp != x_M_pi)
      {
        if (x__tmp != 0)
          x__tmp->x_M_add_ref_copy ();
        if (x_M_pi != 0)
          x_M_pi->x_M_release ();
        x_M_pi = x__tmp;
      }
      return *this;
    }

    void x_M_swap (x__shared_count & x__r)
    {
      x_Sp_counted_base *x__tmp = x__r.x_M_pi;
      x__r.x_M_pi = x_M_pi;
      x_M_pi = x__tmp;
    }

    long x_M_get_use_count () const
    {
      return x_M_pi != 0 ? x_M_pi->x_M_get_use_count () : 0;
    }

    bool x_M_unique () const
    {
      return this->x_M_get_use_count () == 1;
    }

    friend inline bool
      operator== (const x__shared_count & x__a, const x__shared_count & x__b)
    {
      return x__a.x_M_pi == x__b.x_M_pi;
    }

    friend inline bool
      operator< (const x__shared_count & x__a, const x__shared_count & x__b)
    {
      return std::less < x_Sp_counted_base *>()(x__a.x_M_pi, x__b.x_M_pi);
    }

    void *x_M_get_deleter (const std::type_info & x__ti) const
    {
      return x_M_pi ? x_M_pi->x_M_get_deleter (x__ti) : 0;
    }

  private:
    x_Sp_counted_base *x_M_pi;
};

struct x__static_cast_tag
{
};
struct x__const_cast_tag
{
};
struct x__dynamic_cast_tag
{
};
template < typename x_Tp > class shared_ptr
{
  public:
    typedef x_Tp element_type;

    shared_ptr ():
    x_M_ptr (0), x_M_refcount ()
    {
    }

    template < typename x_Tp1 > explicit shared_ptr (x_Tp1 * x__p):
    x_M_ptr (x__p), x_M_refcount (x__p)
    {
//      x__enable_shared_from_this_helper (x_M_refcount, x__p, x__p);
    }
    template < typename x_Tp1, typename x_Deleter > shared_ptr (x_Tp1 * x__p, x_Deleter x__d):
    x_M_ptr (x__p), x_M_refcount (x__p,
      x__d)
    {
//      x__enable_shared_from_this_helper (x_M_refcount, x__p, x__p);
    }
    template < typename x_Tp1 >
      shared_ptr (const shared_ptr < x_Tp1 > &x__r):
    x_M_ptr (x__r.x_M_ptr), x_M_refcount (x__r.x_M_refcount)
    {
    }

    template < typename x_Tp1 > explicit shared_ptr (std::auto_ptr < x_Tp1 > &x__r):
    x_M_ptr (x__r.get ()),
      x_M_refcount ()
    {
      x_Tp1 *x__tmp = x__r.get ();
      x_M_refcount = x__shared_count (x__r);
//      x__enable_shared_from_this_helper (x_M_refcount, x__tmp, x__tmp);
    }

    template < typename x_Tp1 >
      shared_ptr (const shared_ptr < x_Tp1 > &x__r, x__static_cast_tag):
    x_M_ptr (static_cast <element_type * >(x__r.x_M_ptr)),x_M_refcount (x__r.x_M_refcount)
    {
    }

    template < typename x_Tp1 >
      shared_ptr (const shared_ptr < x_Tp1 > &x__r, x__const_cast_tag):
    x_M_ptr (const_cast <element_type * >(x__r.x_M_ptr)), x_M_refcount (x__r.x_M_refcount)
    {
    }

    template < typename x_Tp1 >
      shared_ptr (const shared_ptr < x_Tp1 > &x__r, x__dynamic_cast_tag):
    x_M_ptr (dynamic_cast <element_type * >(x__r.x_M_ptr)), x_M_refcount (x__r.x_M_refcount)
    {
      if (x_M_ptr == 0)
        x_M_refcount = x__shared_count ();
    }

    template < typename x_Tp1 >
      shared_ptr & operator= (const shared_ptr < x_Tp1 > &x__r)
    {
      x_M_ptr = x__r.x_M_ptr;
      x_M_refcount = x__r.x_M_refcount;
      return *this;
    }

    template < typename x_Tp1 >
      shared_ptr & operator= (std::auto_ptr < x_Tp1 > &x__r)
    {
      shared_ptr (x__r).swap (*this);
      return *this;
    }
    void reset ()
    {
      shared_ptr ().swap (*this);
    }

    template < typename x_Tp1 > void reset (x_Tp1 * x__p)
    {
      shared_ptr (x__p).swap (*this);
    }

    template < typename x_Tp1, typename x_Deleter >
      void reset (x_Tp1 * x__p, x_Deleter x__d)
    {
      shared_ptr (x__p, x__d).swap (*this);
    }
/*
    typename std::tr1::add_reference < x_Tp >::type operator* () const
    {
      return *x_M_ptr;
    }
*/
    x_Tp *operator-> () const
    {
      return x_M_ptr;
    }

    x_Tp *get () const
    {
      return x_M_ptr;
    }

  private:
    typedef x_Tp *shared_ptr::*x__unspecified_bool_type;

  public:
    operator   x__unspecified_bool_type ()const
    {
      return x_M_ptr == 0 ? 0 : &shared_ptr::x_M_ptr;
    }

    bool unique () const
    {
      return x_M_refcount.x_M_unique ();
    }

    long use_count () const
    {
      return x_M_refcount.x_M_get_use_count ();
    }

    void swap (shared_ptr < x_Tp > &x__other)
    {
      std::swap (x_M_ptr, x__other.x_M_ptr);
      x_M_refcount.x_M_swap (x__other.x_M_refcount);
    }
  private:
    void *x_M_get_deleter (const std::type_info & x__ti) const
    {
      return x_M_refcount.x_M_get_deleter (x__ti);
    }

    template < typename x_Tp1 >
      bool x_M_less (const shared_ptr < x_Tp1 > &x__rhs) const
    {
      return x_M_refcount < x__rhs.x_M_refcount;
    }

    template < typename x_Tp1 > friend class shared_ptr;

    template < typename x_Del, typename x_Tp1 >
      friend x_Del * get_deleter (const shared_ptr < x_Tp1 > &);

    template < typename x_Tp1 >
      friend inline bool
      operator== (const shared_ptr & x__a, const shared_ptr < x_Tp1 > &x__b)
    {
      return x__a.get () == x__b.get ();
    }

    template < typename x_Tp1 >
      friend inline bool
      operator!= (const shared_ptr & x__a, const shared_ptr < x_Tp1 > &x__b)
    {
      return x__a.get () != x__b.get ();
    }

    template < typename x_Tp1 >
      friend inline bool
      operator< (const shared_ptr & x__a, const shared_ptr < x_Tp1 > &x__b)
    {
      return x__a.x_M_less (x__b);
    }

    x_Tp *x_M_ptr;
    x__shared_count x_M_refcount;
};

}

#endif
