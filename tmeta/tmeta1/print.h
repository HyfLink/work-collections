#include <iostream>
#include "types.h"

#pragma once
#ifndef TMETA_PRINT_H
#define TMETA_PRINT_H

namespace tmeta {
      template<typename T> std::ostream& print( const std::string& msg = {}, std::ostream & os = std::cout, bool bracket = true );
      template<typename T> std::ostream& println( const std::string& msg = {}, std::ostream& os = std::cout, bool bracket = true );

      namespace impl {

            template<typename T>
            struct _print_impl {

                  static void print_impl( std::ostream& os, bool bracket )
                  {
                        print_impl( os, bracket, typename T::__tags__( ) );
                  }

                  static void print_impl( std::ostream& os, bool bracket, __null_tags )
                  {
                        os << "null";
                  }

                  static void print_impl( std::ostream& os, bool bracket, __boolean_tags )
                  {
                        os << (T::instance ? "true" : "false");
                  }

                  static void print_impl( std::ostream& os, bool bracket, __number_tags )
                  {
                        if constexpr ( T::denom == 1 ) {
                              os << T::numer;
                        }
                        else if constexpr ( T::denom == 0 ) {
                              os << "NaN";
                        }
                        else {
                              os << T::instance << '(' << T::numer << '/' << T::denom << ')';
                        }
                  }

                  static void print_impl( std::ostream& os, bool bracket, __string_tags )
                  {
                        os << '\"' << T::c_str << '\"';
                  }

                  static void print_impl( std::ostream& os, bool bracket, __pair_tags )
                  {
                        if ( bracket ) { os << '{'; }

                        print<car<T>>( {}, os );
                        print<cdr<T>>( ", ", os );

                        if ( bracket ) { os << '}'; }
                  }

                  static void print_impl( std::ostream& os, bool bracket, __list_tags )
                  {
                        if ( bracket ) { os << '['; }

                        print<car<T>>( {}, os );
                        if constexpr ( !isNull<cdr<T>>::instance ) {
                              print<cdr<T>>( ", ", os, false );
                        }

                        if ( bracket ) { os << ']'; }
                  }

            };
      } // namespace impl

      template<typename T> std::ostream& print( const std::string& msg, std::ostream& os, bool bracket )
      {
            os << msg;
            impl::_print_impl<T>::print_impl( os, bracket );
            return os;
      }

      template<typename T> std::ostream& println( const std::string& msg, std::ostream& os, bool bracket )
      {
            os << msg;
            impl::_print_impl<T>::print_impl( os, bracket );
            return os << std::endl;
      }

} // namespace tmeta

#endif // TMETA_PRINT_H