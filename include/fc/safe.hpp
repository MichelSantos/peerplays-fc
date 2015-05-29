#pragma once
#include <fc/exception/exception.hpp>
#include <fc/reflect/reflect.hpp>

#include <limits>

namespace fc {

   /**
    *  This type is designed to provide automatic checks for
    *  integer overflow and default initialization. It will
    *  throw an exception on overflow conditions.
    *
    *  It can only be used on built-in types.  In particular,
    *  safe<uint128_t> is buggy and should not be used.
    */
   template<typename T>
   struct safe
   {
      template<typename O>
      safe( O o ):value(o){}
      safe(){}
      safe( const safe& o ):value(o.value){}

      static safe max()
      { return std::numeric_limits<T>::max(); }
      static safe min()
      { return std::numeric_limits<T>::min(); }

      safe& operator += (  const safe& b )
      {
         if( b.value > 0 && value > (std::numeric_limits<T>::max() - b.value) ) FC_CAPTURE_AND_THROW( overflow_exception, (*this)(b) );
         if( b.value < 0 && value < (std::numeric_limits<T>::min() - b.value) ) FC_CAPTURE_AND_THROW( underflow_exception, (*this)(b) );
         value += b.value;
         return *this;
      }
      friend safe operator + ( const safe& a, const safe& b )
      {
         if( b.value > 0 && a.value > std::numeric_limits<T>::max() - b.value ) FC_CAPTURE_AND_THROW( overflow_exception, (a)(b) );
         if( b.value < 0 && a.value < std::numeric_limits<T>::min() - b.value ) FC_CAPTURE_AND_THROW( underflow_exception, (a)(b) );
         return safe(a.value+b.value);
      }
      safe& operator *= ( safe v ) { value *= v.value; return *this; }
      safe& operator /= ( safe v ) { FC_ASSERT(v.value != 0); value /= v.value; return *this; }
      safe& operator -= (  const safe& b ) { return *this += safe(-b.value); }
      safe operator -()const
      {
         if( value == std::numeric_limits<T>::min() )
            FC_CAPTURE_AND_THROW( overflow_exception, (value) );
         return safe(-value);
      }

      safe operator++(int) { safe bak = *this; *this += 1; return bak; }
      safe& operator++() { return *this += 1; }
      safe operator--(int) { safe bak = *this; *this -= 1; return bak; }
      safe& operator--() { return *this -= 1; }

      friend safe operator - ( const safe& a, const safe& b )
      {
         safe tmp(a); tmp -= b; return tmp;
      }

      friend bool operator == ( const safe& a, const safe& b )
      {
         return a.value == b.value;
      }
      friend bool operator == ( const safe& a, const T& b )
      {
         return a.value == b;
      }
      friend bool operator == ( const T& a, const safe& b )
      {
         return a == b.value;
      }
      friend bool operator != ( const safe& a, const safe& b )
      {
         return a.value != b.value;
      }
      friend bool operator != ( const safe& a, const T& b )
      {
         return a.value != b;
      }
      friend bool operator != ( const T& a, const safe& b )
      {
         return a != b.value;
      }
      friend bool operator < ( const safe& a, const safe& b )
      {
         return a.value < b.value;
      }
      friend bool operator < ( const safe& a, const T& b )
      {
         return a.value < b;
      }
      friend bool operator < ( const T& a, const safe& b )
      {
         return a < b.value;
      }
      friend bool operator > ( const safe& a, const safe& b )
      {
         return a.value > b.value;
      }
      friend bool operator > ( const safe& a, const T& b )
      {
         return a.value > b;
      }
      friend bool operator > ( const T& a, const safe& b )
      {
         return a > b.value;
      }
      friend bool operator >= ( const safe& a, const safe& b )
      {
         return a.value >= b.value;
      }
      friend bool operator >= ( const safe& a, const T& b )
      {
         return a.value >= b;
      }
      friend bool operator >= ( const T& a, const safe& b )
      {
         return a >= b.value;
      }
      friend bool operator <= ( const safe& a, const safe& b )
      {
         return a.value <= b.value;
      }
      friend bool operator <= ( const safe& a, const T& b )
      {
         return a.value <= b;
      }
      friend bool operator <= ( const T& a, const safe& b )
      {
         return a <= b.value;
      }
      T value = 0;
   };

}

FC_REFLECT_TEMPLATE( (typename T), safe<T>, (value) )
