/*	Copyright (C) 2004 Garrett A. Kajmowicz

	This file is part of the uClibc++ Library.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <basic_definitions>
#include <char_traits>
#include <string.h>
#include <func_exception>
#include <memory>
#include <vector>


#ifdef __UCLIBCXX_HAS_WCHAR__
#include <cwchar>
#include <cwctype>
#endif

#ifndef __HEADER_STD_STRING
#define __HEADER_STD_STRING 1

#pragma GCC visibility push(default)

#include "klee.h"

namespace std{

	//Basic basic_string

	template<class Ch, class Tr = char_traits<Ch>, class A = allocator<Ch> > class basic_string;

	typedef basic_string<char> string;
	#ifdef __UCLIBCXX_HAS_WCHAR__
	typedef basic_string<wchar_t> wstring;
	#endif



//template<class Ch, class Tr = char_traits<Ch>, class A = allocator<Ch> > class _UCXXEXPORT basic_string
template<class Ch, class Tr, class A> class basic_string
	: public std::vector<Ch, A>
{
        // pointers to the symbolic char array
  	Ch* sym_chars;

public:
	typedef Tr traits_type;
	typedef typename Tr::char_type value_type;
	typedef A allocator_type;
	typedef typename A::size_type size_type;
	typedef typename A::difference_type difference_type;

	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;
	typedef typename A::pointer pointer;
	typedef typename A::const_pointer const_pointer;

	typedef typename vector<Ch, A>::iterator iterator;
	typedef typename vector<Ch, A>::const_iterator const_iterator;

	typedef typename vector<Ch, A>::reverse_iterator reverse_iterator;
	typedef typename vector<Ch, A>::const_reverse_iterator const_reverse_iterator;

	static const size_type npos = (size_type)-1;

	_UCXXEXPORT inline Ch* get_sym_chars() const { return sym_chars; }
	_UCXXEXPORT inline void set_sym_chars(Ch* chs) { sym_chars = chs; }
	_UCXXEXPORT inline const Ch* get_data() const {
	  return sym_chars ? sym_chars : vector<Ch, A>::data;
	}
	_UCXXEXPORT inline const size_type get_length() const{
	  return sym_chars ? klee_get_size(sym_chars) : length();
	}

	explicit _UCXXEXPORT basic_string(const A& al = A()) : vector<Ch, A>(al){ return; }

	_UCXXEXPORT basic_string(const basic_string& str, size_type pos = 0, size_type n = npos, const A& al = A());	//Below

	_UCXXEXPORT basic_string(const Ch* s, size_type n, const A& al = A())
		: vector<Ch, A>(al)
	{
		if(n == npos){
			__throw_out_of_range();
		}
		if(s > 0){
			resize(n);
			Tr::copy(vector<Ch, A>::data, s, vector<Ch, A>::elements);
		}
		sym_chars = NULL;
	}

	_UCXXEXPORT basic_string(const Ch* s, const A& al = A());		//Below
	
	_UCXXEXPORT basic_string(size_type n, Ch c, const A& al = A())
		: vector<Ch, A>(n, c, al)
	{
	}

	template<class InputIterator> _UCXXEXPORT basic_string(InputIterator begin, InputIterator end, const A& a = A())
		:vector<Ch, A>(begin, end)
	{
		
	}

	_UCXXEXPORT ~basic_string() {
		return;
	}

	_UCXXEXPORT basic_string& operator=(const basic_string& str);	//Below

	_UCXXEXPORT basic_string& operator=(const Ch* s){
		vector<Ch, A>::clear();
		if(s!=0){
			size_type len = Tr::length(s);
			resize(len);
			Tr::copy( vector<Ch, A>::data, s, len);
		}
		return *this;
	}

	_UCXXEXPORT basic_string& operator=(Ch c){
		vector<Ch, A>::clear();
		vector<Ch, A>::push_back(c);
		return *this;
	}

	inline _UCXXEXPORT size_type length() const { return vector<Ch, A>::size(); }

	void _UCXXEXPORT resize(size_type n, Ch c = Ch()){
		vector<Ch, A>::resize(n, c);
	}

	_UCXXEXPORT basic_string& operator+=(const basic_string& str){
		return append(str);
	}

	_UCXXEXPORT basic_string& operator+=(const Ch * s){
		return append(s);
	}

	_UCXXEXPORT basic_string& operator+=(Ch c){
		vector<Ch, A>::push_back(c);
		return *this;
	}

	_UCXXEXPORT basic_string& append(const basic_string& str){
		size_t temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + str.elements);
		Tr::copy( vector<Ch, A>::data + temp, str.vector<Ch, A>::data, str.elements);

		return *this;
	}

	_UCXXEXPORT basic_string& append(const basic_string& str, size_type pos, size_type n){
		if(pos > str.size()){
			__throw_out_of_range();
		}

		size_type rlen = str.elements - pos;
		if(rlen > n){
			rlen = n;
		}
		if(vector<Ch, A>::elements > npos - rlen){
			__throw_length_error();
		}
		size_t temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + rlen);
		Tr::copy( vector<Ch, A>::data + temp, str.vector<Ch, A>::data + pos, rlen);
		return *this;
	}
		
	_UCXXEXPORT basic_string& append(const Ch* s, size_type n){
		size_t temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + n);
		Tr::copy( vector<Ch, A>::data + temp, s, n);
		return *this;
	}

	_UCXXEXPORT basic_string& append(const Ch* s){
		size_type strLen = Tr::length(s);
		size_t temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + strLen);
		Tr::copy( vector<Ch, A>::data + temp, s, strLen);
		return *this;
	}

	_UCXXEXPORT basic_string& append(size_type n, Ch c){
		vector<Ch, A>::resize(vector<Ch, A>::elements + n, c);
		return *this;
	}

	_UCXXEXPORT basic_string& append(const Ch c){
		return append(1,c);
	}
	template<class InputIterator> _UCXXEXPORT basic_string& append(InputIterator first, InputIterator last){
		while (first != last){
			append(*first);
			++first;
		}
		return *this;
	}


	_UCXXEXPORT basic_string& assign(const basic_string& str){
		operator=(str);
		return *this;
	}

	_UCXXEXPORT basic_string& assign(const basic_string& str, size_type pos, size_type n){
		if(pos > str.elements){
			__throw_out_of_range();
		}
		size_type r = str.elements - pos;
		if(r > n){
			r = n;
		}
		resize(r);
		Tr::copy(vector<Ch, A>::data, str.vector<Ch, A>::data + pos, r);
		return *this;
	}

	_UCXXEXPORT basic_string& assign(const Ch* s, size_type n){
		resize(n);
		Tr::copy(vector<Ch, A>::data, s, n);
		return *this;
	}

	_UCXXEXPORT basic_string& assign(const Ch* s){
		size_type len = Tr::length(s);
		return assign(s, len);
	}

	_UCXXEXPORT basic_string& assign(size_type n, Ch c){
		vector<Ch, A>::clear();
		vector<Ch, A>::resize(n, Ch() );
		return *this;
	}

	template<class InputIterator> _UCXXEXPORT basic_string& assign(InputIterator first, InputIterator last){
		vector<Ch, A>::resize(0, Ch());
		while (first != last){
			append(*first);
			++first;
		}
		return *this;
	}

	_UCXXEXPORT basic_string& insert(size_type pos1, const basic_string& str, size_type pos2=0, size_type n=npos){
		if(pos1 > vector<Ch, A>::elements || pos2 > str.elements){
			__throw_out_of_range();
		}
		size_type r = str.elements - pos2;
		if( r > n){
			r = n;
		}
		if(vector<Ch, A>::elements > npos - r){
			__throw_length_error();
		}
		size_type temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + r);
		Tr::move(vector<Ch, A>::data + pos1 + r, vector<Ch, A>::data + pos1, temp - pos1);
		Tr::copy(vector<Ch, A>::data + pos1, str.vector<Ch, A>::data + pos2, r);
		return *this;
	}

	_UCXXEXPORT basic_string& insert(size_type pos, const Ch* s, size_type n){
		if(pos > vector<Ch, A>::elements){
			__throw_out_of_range();
		}
		if(vector<Ch, A>::elements > npos - n){
			__throw_length_error();
		}
		size_type temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + n);
		Tr::move(vector<Ch, A>::data + pos + n, vector<Ch, A>::data + pos, temp - pos);
		Tr::copy(vector<Ch, A>::data + pos, s, n);
		return *this;
	}

	inline _UCXXEXPORT basic_string& insert(size_type pos, const Ch* s){
		size_type len = Tr::length(s);
		return insert(pos, s, len);
	}

	inline _UCXXEXPORT basic_string& insert(size_type pos, const Ch c){
		return insert(pos, 1, c);
	}

	_UCXXEXPORT basic_string& insert(iterator p, const Ch c) {
		size_type pos = vector<Ch, A>::end() - p;
		return insert(pos, c);
	//	if(p != vector<Ch, A>::end()) {
	//	}
	}


	_UCXXEXPORT basic_string& insert(size_type pos, size_type n, Ch c){
		if(pos > vector<Ch, A>::elements){
			__throw_out_of_range();
		}
		if(vector<Ch, A>::elements > npos - n){
			__throw_length_error();
		}
		size_type temp = vector<Ch, A>::elements;
		resize(vector<Ch, A>::elements + n);
		Tr::move(vector<Ch, A>::data + pos + n, vector<Ch, A>::data + pos, temp - pos);
		Tr::assign(vector<Ch, A>::data + pos, n, c);
		return *this;
	}

//	iterator insert(iterator p, charT c = charT());
//	void insert(iterator p, size_type n, charT c);
//	template<class InputIterator> void insert(iterator p, InputIterator first, InputIterator last);

	_UCXXEXPORT basic_string& erase(size_type pos = 0, size_type n = npos){
	
		size_type xlen = vector<Ch, A>::elements - pos;

		if(xlen > n){
			xlen = n;
		}
		size_type temp = vector<Ch, A>::elements;

		Tr::move(vector<Ch, A>::data + pos, vector<Ch, A>::data + pos + xlen, temp - pos - xlen);
		resize(temp - xlen);
		return *this;
	}

	_UCXXEXPORT iterator erase(iterator position){
		if(position == vector<Ch, A>::end()){
			return position;
		}

		++position;

		iterator temp = position;

		while(position != vector<Ch, A>::end()){
			*(position-1) = *position;
			++position;
		}
		vector<Ch, A>::pop_back();
		return temp;
	}

	_UCXXEXPORT iterator erase(iterator first, iterator last){
		size_t count = last - first;

		iterator temp = last;

		while(last != vector<Ch, A>::end()){
			*(last - count) = *last;
			++last;
		}

		resize(	vector<Ch, A>::elements-count);

		return temp;
	}

	_UCXXEXPORT basic_string&
		replace(size_type pos1, size_type n1, const basic_string& str, size_type pos2=0, size_type n2=npos)
	{
		if(pos1 > vector<Ch, A>::elements){
			__throw_out_of_range();
		}
		size_type xlen = vector<Ch, A>::elements - pos1;
		if(xlen >  n1){
			xlen = n1;
		}
		size_type rlen = str.elements - pos2;
		if(rlen > n2){
			rlen = n2;
		}
		if((vector<Ch, A>::elements - xlen) >= (npos - rlen)){
			__throw_length_error();
		}

		size_t temp = vector<Ch, A>::elements;

		if(rlen > xlen){		//Only if making larger
			resize(temp - xlen + rlen);
		}

		//Final length = vector<Ch, A>::elements - xlen + rlen
		//Initial block is of size pos1
		//Block 2 is of size len

		Tr::move(vector<Ch, A>::data + pos1 + rlen, vector<Ch, A>::data + pos1 + xlen, temp - pos1 - xlen);
		Tr::copy(vector<Ch, A>::data + pos1, str.vector<Ch, A>::data + pos2, rlen);
		resize(temp - xlen + rlen);
		return *this;
	}

	_UCXXEXPORT basic_string& replace(size_type pos, size_type n1, const Ch* s, size_type n2){
		return replace(pos,n1,basic_string<Ch,Tr,A>(s,n2));
		
	}

	inline _UCXXEXPORT basic_string& replace(size_type pos, size_type n1, const Ch* s){
		return replace(pos,n1,basic_string<Ch,Tr,A>(s));
	}

	_UCXXEXPORT basic_string& replace(size_type pos, size_type n1, size_type n2, Ch c){
		return replace(pos,n1,basic_string<Ch, Tr, A>(n2,c));
	}
//	_UCXXEXPORT basic_string& replace(iterator i1, iterator i2, const basic_string& str);
//	_UCXXEXPORT basic_string& replace(iterator i1, iterator i2, const Ch* s, size_type n);
//	_UCXXEXPORT basic_string& replace(iterator i1, iterator i2, const Ch* s);
//	_UCXXEXPORT basic_string& replace(iterator i1, iterator i2, size_type n, Ch c);
/*	template<class InputIterator> _UCXXEXPORT basic_string& replace(iterator i1, iterator i2,
		InputIterator j1, InputIterator j2);*/

	size_type _UCXXEXPORT copy(Ch* s, size_type n, size_type pos = 0) const{
		if(pos > vector<Ch, A>::elements){
			__throw_out_of_range();
		}
		size_type r = vector<Ch, A>::elements - pos;
		if(r > n){
			r = n;
		}
		Tr::copy(s, vector<Ch, A>::data + pos, r);
		return r;
	}

	_UCXXEXPORT void swap(basic_string<Ch,Tr,A>& s){
		//Data pointers

		vector<Ch, A>::swap(s);
	}

	_UCXXEXPORT const Ch* c_str() const{
		const_cast<basic_string<Ch,Tr,A> *>(this)->reserve(vector<Ch, A>::elements+1);
		vector<Ch, A>::data[vector<Ch, A>::elements] = 0;	//Add 0 at the end
		return vector<Ch, A>::data;
	}

	_UCXXEXPORT const Ch* data() const{
		return vector<Ch, A>::data;
	}
	_UCXXEXPORT allocator_type get_allocator() const{
		return vector<Ch, A>::a;
	}

	_UCXXEXPORT size_type find (const basic_string& str, size_type pos = 0) const;	//Below

	_UCXXEXPORT size_type find (const Ch* s, size_type pos, size_type n) const{
		return find(basic_string<Ch, Tr, A>(s,n), pos);
	}
	_UCXXEXPORT size_type find (const Ch* s, size_type pos = 0) const{
		return find(basic_string<Ch, Tr, A>(s), pos);
	}

	_UCXXEXPORT size_type find (Ch c, size_type pos = 0) const {
	  for (size_type i = pos; i < length(); ++i){
	    if(operator[](i) == c){
	      return i;
	    }
	  }
	  return npos;
	}

	_UCXXEXPORT size_type rfind(const basic_string& str, size_type pos = npos) const{
	  if(pos >= length()){
	    pos = length();
	  }
	  for(size_type i = pos; i > 0; --i){
	    if(str == substr(i-1, str.length())){
	      return i-1;
	    }
	  }
	  return npos;
	}

	_UCXXEXPORT size_type rfind(const Ch* s, size_type pos, size_type n) const{
		return rfind(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type rfind(const Ch* s, size_type pos = npos) const{
		return rfind(basic_string<Ch, Tr, A>(s),pos);
	}
	_UCXXEXPORT size_type rfind(Ch c, size_type pos = npos) const{
		return rfind(basic_string<Ch, Tr, A>(1,c),pos);
	}

	_UCXXEXPORT size_type find_first_of(const basic_string& str, size_type pos = 0) const{
		for(size_type i = pos; i < length(); ++i){
			for(size_type j = 0; j < str.length() ; ++j){
				if( Tr::eq(str[j], operator[](i)) ){
					return i;
				}
			}
		}
		return npos;
	}

	_UCXXEXPORT size_type find_first_of(const Ch* s, size_type pos, size_type n) const{
		return find_first_of(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type find_first_of(const Ch* s, size_type pos = 0) const{
		return find_first_of(basic_string<Ch, Tr, A>(s),pos);
	}
	_UCXXEXPORT size_type find_first_of(Ch c, size_type pos = 0) const{
	  // the original processing (including the concrete case)
	  for(size_type i = pos; i< length(); ++i){
	    if( Tr::eq(operator[](i), c) ){
	      return i;
	    }
	  }
	  return npos;
	}


	_UCXXEXPORT size_type find_last_of (const basic_string& str, size_type pos = npos) const{
		if(pos > length()){
			pos = length();
		}
		for(size_type i = pos; i >0 ; --i){
			for(size_type j = 0 ; j < str.length(); ++j){
				if( Tr::eq(operator[](i-1), str[j]) ){
					return i-1;
				}
			}
		}
		return npos;
	}
	_UCXXEXPORT size_type find_last_of (const Ch* s, size_type pos, size_type n) const{
		return find_last_of(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type find_last_of (const Ch* s, size_type pos = npos) const{
		return find_last_of(basic_string<Ch, Tr, A>(s),pos);
	}

	_UCXXEXPORT size_type find_last_of (Ch c, size_type pos = npos) const{
	  if (pos > length()) {  // handle the exception case 
	    pos = length();
	  }

	  Ch* chs = get_sym_chars();
	  if (chs) {   // string with var-length
	    int s_len = klee_get_size(chs);
	    int index = klee_create_symbolic_variable(4,"index");
	    klee_assume(index >= -1 && index < s_len);
	    if (index == -1) {
	      klee_assume2("@x [0,%0] : %1[x] <> %2", pos, chs, c);
	    }
	    else {
	      klee_assume2("@x [%0,%1] : %2[x] <> %3", index+1, pos, chs, c);
	      klee_assume((sym_chars[(size_type)(index)]) == c);
	    }
	    return index;
	  } 

	  // the string is not symbolic, and it has a concrete length

	  // the case where c is symbolic
	  if (klee_is_symbolic(c)) {
	    bool found = false;
	    int v = pos - 1;
	    for (size_type i = pos; i > 0 ; i--) {
	      found = (operator[](i-1) == c) || found;
	      v -= !found;        // not equal, continue search
	    }
	    return v;
	  }

	  // the original processing (including the concrete case)
	  for(size_type i = pos; i >0 ; --i){
	    if( Tr::eq(operator[](i-1), c) ){
	      return i-1;
	    }
	  }
	  return npos;
	}


	_UCXXEXPORT size_type find_first_not_of(const basic_string& str, size_type pos = 0) const{
		bool foundCharacter;
		for(size_type i = pos; i < length(); ++i){
			foundCharacter = false;
                        for(size_type j = 0; j < str.length() ; ++j){
                                if( Tr::eq(str[j], operator[](i)) ){
					foundCharacter = true;
                                }
                        }
			if(foundCharacter == false){
				return i;
			}
                }
		return npos;
	}

	_UCXXEXPORT size_type find_first_not_of(const Ch* s, size_type pos, size_type n) const{
		return find_first_not_of(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type find_first_not_of(const Ch* s, size_type pos = 0) const{
		return find_first_not_of(basic_string<Ch, Tr, A>(s),pos);
	}
	_UCXXEXPORT size_type find_first_not_of(Ch c, size_type pos = 0) const{
		for(size_type i = pos; i < length() ; ++i){
			if(operator[](i) != c){
				return i;
			}
		}
		return npos;
	}
	_UCXXEXPORT size_type find_last_not_of (const basic_string& str, size_type pos = npos) const{
		size_type xpos(length() - 1);
		if(xpos > pos){
			xpos = pos;
		}
		
		while(xpos != npos && npos != str.find_first_of(at(xpos))){
			--xpos;
		}

		return xpos;
	}

	_UCXXEXPORT size_type find_last_not_of (const Ch* s, size_type pos, size_type n) const{
		return find_last_not_of(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type find_last_not_of (const Ch* s, size_type pos = npos) const{
		return find_last_not_of(basic_string<Ch, Tr, A>(s),pos);
	}
	_UCXXEXPORT size_type find_last_not_of (Ch c, size_type pos = npos) const{
		size_type xpos(length() - 1);
		if(xpos > pos){
			xpos = pos;
		}
		while(xpos != npos && Tr::eq(at(xpos), c)){
			--xpos;
		}
		return xpos;

	}

	_UCXXEXPORT basic_string substr(size_type pos = 0, size_type n = npos) const;

	_UCXXEXPORT int compare(const basic_string& str) const{
	  // the symbolic case
	  Ch* chs = get_sym_chars();
	  Ch* chs1 = str.get_sym_chars();
	  if (get_sym_chars() || chs1)
	    return compare(0, get_length(), str.get_data());

	  // the concrete case
	  size_type rlen = vector<Ch, A>::elements;
	  if(rlen >  str.elements){
	    rlen = str.elements;
	  }
	  int retval = Tr::compare(vector<Ch, A>::data, str.vector<Ch, A>::data, rlen);
	  if(retval == 0){
	    if(vector<Ch, A>::elements < str.elements){
	      retval = -1;
	    }
	    if(vector<Ch, A>::elements > str.elements){
	      retval = 1;
	    }
	  }
	  return retval;
	}

	_UCXXEXPORT int compare(size_type pos1, size_type n1, const basic_string& str,
				size_type pos2=0, size_type n2=npos) const{
	  // the symbolic case
	  if (get_sym_chars() || str.get_sym_chars())
	    return compare(pos1, n1, str.get_data(), n2);

	  // the concrete case
		size_type len1 = vector<Ch, A>::elements - pos1;
		if(len1 > n1){
			len1 = n1;
		}
		size_type len2 = str.vector<Ch, A>::elements - pos2;
		if(len2 > n2){
			len2 = n2;
		}
		size_type rlen = len1;
		if(rlen > len2){
			rlen = len2;
		}
		int retval = Tr::compare(vector<Ch, A>::data + pos1, str.vector<Ch, A>::data + pos2, rlen);
		if(retval == 0){
			if(len1 < len2){
				retval = -1;
			}
			if(len1 > len2){
				retval = 1;
			}
		}
		return retval;
	}

	_UCXXEXPORT int compare(const Ch* s) const{
	  Ch* chs = get_sym_chars();
	  if (chs || s)
	    return compare(0, get_length(), s);

		size_type slen = Tr::length(s);
		size_type rlen = slen;
		if(rlen > vector<Ch, A>::elements){
			rlen=vector<Ch, A>::elements;
		}
		int retval = Tr::compare(vector<Ch, A>::data, s, rlen);
		if(retval==0){
			if(vector<Ch, A>::elements < slen){
				retval = -1;
			}
			if(vector<Ch, A>::elements > slen){
				retval = 1;
			}
		}
		return retval;
	}


	_UCXXEXPORT int compare(size_type pos1, size_type n1, const Ch* s, size_type n2 = npos) const{
	  Ch* chs = get_sym_chars();
	  if (chs) {
	    unsigned s1_len = klee_get_size(chs);
	    unsigned s2_len = klee_is_symbolic_array(s) ? klee_get_size(s) : strlen(s);
	    int k = klee_create_symbolic_variable(4);
	    klee_assume(pos1 + n1 <= s1_len);
	    /* if (k > 0) { */
	    /*   klee_assume2("@x [0,%0-1] : %1[%2+x] > %3[x]", */
	    /*   		   (unsigned) n1, chs, (unsigned) pos1, s); */
	    /* } */
	    /* else if (k < 0) { */
	    /*   klee_assume2("@x [0,%0-1] : %1[%2+x] < %3[x]", */
	    /*   		   (unsigned) n1, chs, (unsigned) pos1, s); */
	    /* } */
	    if (k) {
	      klee_assume2("%0 == %1 => ?x [0,%0-1] : %2[%3+x] <> %4[x]", 
			   (unsigned) n1, s2_len, chs, (unsigned) pos1, s);
	    }
	    else {
	      printf("%d : %d\n", n1, s2_len);
	      klee_assume(n1 == s2_len);
	      klee_assume2("@x [0,%0-1] : %1[%2+x] == %3[x]",
	      		   (unsigned) n1, chs, (unsigned) pos1, s);
	    }
	    return k;
	  }

	  // the original case 
	  size_type len1 = vector<Ch, A>::elements - pos1;
	  if(len1 > n1){
	    len1 = n1;
	  }
	  size_type slen = Tr::length(s);
	  size_type len2 = slen;
	  if(len2 > n2){
	    len2 = n2;
	  }
	  size_type rlen = len1;
	  if(rlen > len2){
	    rlen = len2;
	  }
	  int retval  = Tr::compare(vector<Ch, A>::data + pos1, s, rlen);
	  if(retval == 0){
	    if(len1 < len2){
	      retval = -1;
	    }
	    if(len1 > len2){
	      retval = 1;
	    }
	  }
	  return retval;
	}
};
	

//Functions

template<class Ch,class Tr,class A> _UCXXEXPORT basic_string<Ch,Tr,A>::basic_string(const Ch* s, const A& al)
	: vector<Ch, A>(al)
{
	if(s!=0){
		size_type temp = Tr::length(s);
		append(s, temp);
	}
	sym_chars = NULL;
}

template<class Ch,class Tr,class A> _UCXXEXPORT basic_string<Ch,Tr,A>::
	basic_string(const basic_string& str, size_type pos, size_type n, const A& al) 
	: vector<Ch, A>(al)
{
  /* if (klee_is_symbolic_array(str.get_data())) { */
  /*   printf("Constructor %p!\n", str.get_data()); */
  /*   vector<Ch, A>::data = (Ch*) (str.get_data()); */
  /*   return; */
  /* } */

  // original processing
  if(pos>str.size()){
    __throw_out_of_range();
  }
  size_type rlen = str.size() - pos;
  if( rlen > n){
    rlen = n;
  }
  resize(rlen);
  Tr::copy(vector<Ch, A>::data, str.vector<Ch, A>::data + pos, vector<Ch, A>::elements);

  sym_chars = str.get_sym_chars();
}

template<class Ch,class Tr,class A> _UCXXEXPORT basic_string<Ch,Tr,A>&
	basic_string<Ch,Tr,A>::operator=(const basic_string<Ch,Tr,A> & str)
{
	if(&str == this){	//Check if we are doing a=a 
		return *this;
	}
	vector<Ch, A>::clear();
	resize(str.elements);
	Tr::copy( vector<Ch, A>::data, str.vector<Ch, A>::data, str.elements);
	return *this;
}


  template<class Ch,class Tr,class A> _UCXXEXPORT typename basic_string<Ch,Tr,A>::size_type
	  basic_string<Ch,Tr,A>::find (const basic_string<Ch,Tr,A>& str, size_type pos) const
  {
    Ch* chs = get_sym_chars();
    Ch* chs1 = str.get_sym_chars();
    if (chs || chs1) {
      unsigned s_len = get_length();
      unsigned s1_len = str.get_length();
      int k = klee_create_symbolic_variable(4);
      if (k == npos) {
	// an approximate solution
	unsigned len = klee_make_ite(s_len > s1_len, s_len, s1_len);
	klee_assume2("@x [0,%0-1] : %1[x] <> %2[x]", 
		     len, str.get_data(), get_data());
      }
      else {
	klee_assume(s1_len + k <= s_len && 0 <= k && k < s_len);
	klee_assume2("@x [0,%0] : %1[x] == %2[%3+x]",
		     s1_len - 1, str.get_data(), get_data(), k);
      }
      return k;
    }
    
    // the original processing (including the concrete case)
    if(str.length() > length()){
      return npos;
    }
    size_type max_string_start = 1 + length() - str.length();
    for(size_type i = pos; i < max_string_start; ++i){
      if(str == substr(i, str.length())){
	return i;
      }
    }
    return npos;
  }


template<class Ch,class Tr,class A>
	_UCXXEXPORT basic_string<Ch, Tr, A> basic_string<Ch,Tr,A>::substr(size_type pos, size_type n) const
{
  Ch* chs = get_sym_chars();
  if (chs) {
    unsigned s_len = klee_get_size(chs);  // this string's length
    basic_string<Ch, Tr, A> s1;
    char* chs1 = new char[vector<Ch, A>::elements];
    klee_make_symbolic_varr(chs1,"",0);
    s1.set_sym_chars(chs1);
    unsigned s1_len = klee_get_size(chs1);
    // the length
    if (n == npos)
      klee_assume(s1_len == s_len - pos && s1_len <= s_len);
    else
      klee_assume(s1_len == n && pos + n <= s_len);
    // the values
    klee_assume2("@x [0, %0] : %1[%2+x] == %3[x]", 
		 s1_len - 1, chs, (unsigned)(pos), chs1);
    return s1;
  }
  
  // the original processing (e.g. concrete case)
  if(pos > vector<Ch, A>::elements){
    __throw_out_of_range();
  }

  size_type rlen = vector<Ch, A>::elements - pos;
  if(rlen > n){
    rlen = n;
  }

  return basic_string<Ch,Tr,A>(vector<Ch, A>::data + pos,rlen);
}


#ifdef __UCLIBCXX_EXPAND_STRING_CHAR__
#ifndef __UCLIBCXX_COMPILE_STRING__

#ifdef __UCLIBCXX_EXPAND_CONSTRUCTORS_DESTRUCTORS__

	template <> _UCXXEXPORT string::basic_string(const allocator<char> &);
	template <> _UCXXEXPORT string::basic_string(size_type n, char c, const allocator<char> & );
	template <> _UCXXEXPORT string::basic_string(const char* s, const allocator<char>& al);
	template <> _UCXXEXPORT string::basic_string(const basic_string& str, size_type pos, size_type n, const allocator<char>& al);
	template <> _UCXXEXPORT string::~basic_string();

#endif

	template <> _UCXXEXPORT string & string::append(const char * s, size_type n);


	template <> _UCXXEXPORT string::size_type string::find(const string & str, size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::find(const char* s, size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::find (char c, size_type pos) const;

	template <> _UCXXEXPORT string::size_type string::rfind(const string & str, size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::rfind(char c, size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::rfind(const char* s, size_type pos) const;

	template <> _UCXXEXPORT string::size_type string::find_first_of(const string &, size_type) const;
	template <> _UCXXEXPORT string::size_type string::find_first_of(const char *, size_type pos, size_type n) const;
	template <> _UCXXEXPORT string::size_type string::find_first_of(const char*, size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::find_first_of(char c, size_type pos) const;

	template <> _UCXXEXPORT string::size_type string::find_last_of (const string & , size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::find_last_of (const char* s, size_type pos, size_type n) const;
	template <> _UCXXEXPORT string::size_type string::find_last_of (const char* s, size_type pos) const;
	template <> _UCXXEXPORT string::size_type string::find_last_of (char c, size_type pos) const;

	template <> _UCXXEXPORT string::size_type string::find_first_not_of(const string &, size_type) const;
	template <> _UCXXEXPORT string::size_type string::find_first_not_of(const char*, size_type, size_type) const;
	template <> _UCXXEXPORT string::size_type string::find_first_not_of(const char*, size_type) const;
	template <> _UCXXEXPORT string::size_type string::find_first_not_of(char c, size_type) const;

	template <> _UCXXEXPORT int string::compare(const string & str) const;
	template <> _UCXXEXPORT int string::compare(
		size_type pos1, size_type n1, const string & str, size_type pos2, size_type n2) const;

	template <> _UCXXEXPORT string string::substr(size_type pos, size_type n) const;

	template <> _UCXXEXPORT string & string::operator=(const string & str);
	template <> _UCXXEXPORT string & string::operator=(const char * s);

#endif
#endif




//typedef basic_string<char> string;

template<class charT, class traits, class Allocator> _UCXXEXPORT basic_string<charT,traits,Allocator> 
	operator+(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	temp.append(rhs);
	return temp;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT basic_string<charT,traits,Allocator>
	operator+(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	temp.append(rhs);
	return temp;
}


template<class charT, class traits, class Allocator> _UCXXEXPORT basic_string<charT,traits,Allocator>
	operator+(charT lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(1, lhs);
	temp.append(rhs);
	return temp;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT basic_string<charT,traits,Allocator>
	operator+(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	temp.append(rhs);
	return temp;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT basic_string<charT,traits,Allocator>
	operator+(const basic_string<charT,traits,Allocator>& lhs, charT rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	temp+=rhs;
	return temp;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator==(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(lhs.compare(rhs) == 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator==(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(rhs.compare(lhs) == 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator==(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	if(lhs.compare(rhs)==0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator!=(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(lhs.compare(rhs) !=0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator!=(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	return (temp != rhs);
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator!=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	return (lhs != temp);
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator< (const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(lhs.compare(rhs) < 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator< (const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	if(lhs.compare(rhs) < 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator< (const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	if(temp.compare(rhs) < 0){
		return true;
	}
	return false;
}


template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator> (const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(lhs.compare(rhs) > 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator> (const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	if(lhs.compare(rhs) > 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator> (const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	if(temp.compare(rhs) > 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator<=(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(lhs.compare(rhs) <=0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator<=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	if(lhs.compare(temp) <=0 ){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator<=(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	if(temp.compare(rhs) <= 0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator>=(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	if(lhs.compare(rhs) >=0){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator>=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	if(lhs.compare(temp) >=0 ){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator>=(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	if(temp.compare(rhs) >=0 ){
		return true;
	}
	return false;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT void 
	swap(basic_string<charT,traits,Allocator>& lhs, basic_string<charT,traits,Allocator>& rhs)
{
	lhs.swap(rhs);
}

/*template<class charT, class traits, class Allocator> _UCXXEXPORT basic_ostream<charT, traits>&
	operator<<(basic_ostream<charT, traits>& os, const basic_string<charT,traits,Allocator>& str)
{
	return os.write(str.data(), str.length());
}*/

#ifdef __UCLIBCXX_EXPAND_STRING_CHAR__
#ifndef __UCLIBCXX_COMPILE_STRING__

//Operators we can avoid duplication of

template <> _UCXXEXPORT bool operator==(const string & lhs, const string & rhs);
template <> _UCXXEXPORT bool operator==(const char * lhs, const string & rhs);
template <> _UCXXEXPORT bool operator==(const string & rhs, const char * rhs);

template <> _UCXXEXPORT bool operator!=(const string & lhs, const string & rhs);
template <> _UCXXEXPORT bool operator!=(const char * lhs, const string & rhs);
template <> _UCXXEXPORT bool operator!=(const string & rhs, const char * rhs);

template <> _UCXXEXPORT string operator+(const string & lhs, const char* rhs);
template <> _UCXXEXPORT string operator+(const char* lhs, const string & rhs);
template <> _UCXXEXPORT string operator+(const string & lhs, const string & rhs);

template <> _UCXXEXPORT bool operator> (const string & lhs, const string & rhs);
template <> _UCXXEXPORT bool operator< (const string & lhs, const string & rhs);


#endif
#endif


}

#pragma GCC visibility pop

#endif
