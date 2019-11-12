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


/*
       Modified by Fujitsu Lab of America to cater for the need of Klee.
       FLA, 2010.
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

// #define DEBUG

#pragma GCC visibility push(default)

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

	explicit _UCXXEXPORT basic_string(const A& al = A()) : vector<Ch, A>(al){ 
	  return; 
	}

	_UCXXEXPORT basic_string(const basic_string& str, size_type pos = 0, size_type n = npos, const A& al = A());	//Below

	_UCXXEXPORT basic_string(const Ch* s, size_type n, const A& al = A())
		: vector<Ch, A>(al)
	{
		if(n == npos){
			__throw_out_of_range();
		}

		if(s > 0) {
		  resize(n);                     // loop forever for symbolic length?

		  //////////////////////////////////////////////////
		  // replace "resize" with the following
// 		  reserve(n + __UCLIBCXX_STL_BUFFER_SIZE__);
// 		  Ch c;
// 		  for (size_type i = 0; i<20 ; i++) {
// 		    vector<Ch, A>::a.construct(vector<Ch, A>::data+i, c);
// 		  }

// 		  vector<Ch, A>::elements = n;
		  //////////////////////////////////////////////////

		  Tr::copy(vector<Ch, A>::data, s, vector<Ch, A>::elements);
		}
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
	// return *this;         // for testing whether this string.o is linked into klee
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


  //////////////////////////////////////////////////////////////////////////////////////////////////

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


  ////////////////////////////////////////////////////////////////////////////////////////////////////

	_UCXXEXPORT size_type find (const basic_string& str, size_type pos = 0) const;	//Below

	_UCXXEXPORT size_type find (const Ch* s, size_type pos, size_type n) const{
		return find(basic_string<Ch, Tr, A>(s,n), pos);
	}
	_UCXXEXPORT size_type find (const Ch* s, size_type pos = 0) const{
		return find(basic_string<Ch, Tr, A>(s), pos);
	}

	_UCXXEXPORT size_type find (Ch c, size_type pos = 0) const {
#ifdef DEBUG
	  printf("find(char,pos) \n");
#endif
	  bool found = false;
	  size_type v = pos;

	  for (size_type i = pos; i < length(); i++) {
	    found = (operator[](i) == c) || found;
	    v += !found;        // not equal, continue search
	  }

	  if (v >= length())       // not found
	    return npos;
	  else
	    return v;
	  
//              // Original implementation
// 		for(size_type i = pos; i < length(); ++i){
// 			if(operator[](i) == c){
// 				return i;
// 			}
// 		}
// 		return npos;
	}

	_UCXXEXPORT size_type rfind(const basic_string& str, size_type pos = npos) const {
#ifdef DEBUG
	  printf("rfind(str,pos) \n");
#endif
	  if (pos >= length()) {
	    pos = length();
	  }

	  bool found = false;
	  size_type v = pos - 1;          // if not found return -1

	  for(size_type i = pos; i != 0 ; i--) {
	    found = (substr(i-1, str.length()) == str) || found;
	    v -= !found;        // not equal, continue search
	  }
	  return v;

//              // Original implementation
// 		if(pos >= length()){
// 			pos = length();
// 		}
// 		for(size_type i = pos; i > 0; --i){
// 			if(str == substr(i-1, str.length())){
// 				return i-1;
// 			}
// 		}
// 		return npos;
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

  //////////////////////////////////////////////////////////////////////////////////////////////////

	_UCXXEXPORT size_type find_first_of(const basic_string& str, size_type pos = 0) const {
#ifdef DEBUG
	  printf("find_first_of(str,pos) \n");
#endif
	  bool found = false;
	  size_type v = pos;

	  for (size_type i = pos; i < length(); ++i) {
	    for (size_type j = 0; j < str.length() ; ++j) {
	      found = Tr::eq(str[j], operator[](i)) || found;
	    }
	      v += !found;        // not equal, continue search
	  }

	  if (v >= length())   // not found 
	    return npos;
	  else
	    return v;

//              // Original implementation
// 		for(size_type i = pos; i < length(); ++i){
// 			for(size_type j = 0; j < str.length() ; ++j){
// 				if( Tr::eq(str[j], operator[](i)) ){
// 					return i;
// 				}
// 			}
// 		}
// 		return npos;
	}

	_UCXXEXPORT size_type find_first_of(const Ch* s, size_type pos, size_type n) const{
		return find_first_of(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type find_first_of(const Ch* s, size_type pos = 0) const{
		return find_first_of(basic_string<Ch, Tr, A>(s),pos);
	}

	_UCXXEXPORT size_type find_first_of(Ch c, size_type pos = 0) const {
#ifdef DEBUG
	  printf("find_first_of(char,pos) ... \n");
#endif
	  
	  bool found = false;
	  size_type v = pos;

	  for (size_type i = pos; i < length(); i++) {
	    found = (operator[](i) == c) || found;
	    v += !found;        // not equal, continue search
	  }

	  if (v >= length())   // not found 
	    return npos;
	  else
	    return v;
	  
//              // Original implementation
// 		for(size_type i = pos; i< length(); ++i){
// 			if( Tr::eq(operator[](i), c) ){
// 				return i;
// 			}
// 		}
// 		return npos;
	}

	_UCXXEXPORT size_type find_last_of (const basic_string& str, size_type pos = npos) const{
#ifdef DEBUG
	  printf("find_last_of(str,pos) ... \n");
#endif

		if(pos > length()) {
		    pos = length();
		}
		
		bool found = false;
		int v = pos - 1;
		for (size_type i = pos; i != 0 ; --i) {
		  for (size_type j = 0 ; j < str.length(); ++j) {
		    found = Tr::eq(operator[](i-1), str[j]) || found;
		  }
		  v -= !found;        // not equal, continue search
		}

		return v;

//              // Original implementation
// 		for(size_type i = pos; i >0 ; --i){
// 			for(size_type j = 0 ; j < str.length(); ++j){
// 				if( Tr::eq(operator[](i-1), str[j]) ){
// 					return i-1;
// 				}
// 			}
// 		}
// 		return npos;
	}

	_UCXXEXPORT size_type find_last_of (const Ch* s, size_type pos, size_type n) const{
		return find_last_of(basic_string<Ch, Tr, A>(s,n),pos);
	}

	_UCXXEXPORT size_type find_last_of (const Ch* s, size_type pos = npos) const{
		return find_last_of(basic_string<Ch, Tr, A>(s),pos);
	}

	_UCXXEXPORT size_type find_last_of (Ch c, size_type pos = npos) const {
#ifdef DEBUG
	    printf("find_last_of(char,pos) ... \n");
#endif
printf("find_last_of(char,pos) ... \n");

	        if (pos > length()) {
		  pos = length();
		}

		/* for(size_type i = pos; i >0 ; --i){ */
		/*   if( Tr::eq(operator[](i-1), c) ){ */
		/*     return i-1; */
		/*   } */
		/* } */
		/* return npos; */

#ifndef STRING_NOHACK
		int retval; // = rand() % length();
		retval = 19;     // Klee cannot handle strings of symbolic lengths

		if (operator[]((size_type)retval) != c) {      // force a branch to return npos
		  return npos;
		}
		
		return retval;        // get the middle one
#else
		int v1 = -1;
		for (size_type i = pos; i > 0; i--) {
		  v1 = klee_make_ite(operator[](i-1) == c, i-1, v1);
		}
		return v1;

		// the following may lead to a string with symbolic length
		bool found = false;
		int v = pos - 1;
		for (size_type i = pos; i > 0 ; i--) {
		  found = (operator[](i-1) == c) || found;
		  v -= !found;        // not equal, continue search
		}

		return v;
#endif
	}


        // Searches for the first character in the object which is not part of str and returns its position.
	_UCXXEXPORT size_type find_first_not_of(const basic_string& str, size_type pos = 0) const {
	  bool found = false;
	  size_type v = pos;

	  for (size_type i = pos; i < length(); ++i) {
	     bool found_char = false;
	     for(size_type j = 0; j < str.length() ; ++j) {
	       found_char = Tr::eq(str[j], operator[](i)) || found_char;
	     }
	     found = !found_char || found;
	    v += !found;        // not equal, continue search
	  }
	  
	  if (v >= length())      // found  
	    return npos;
	  else
	    return v;

//              // Original implementation
// 		bool foundCharacter;
// 		for(size_type i = pos; i < length(); ++i){
// 			foundCharacter = false;
//                         for(size_type j = 0; j < str.length() ; ++j){
//                                 if( Tr::eq(str[j], operator[](i)) ){
// 					foundCharacter = true;
//                                 }
//                         }
// 			if(foundCharacter == false){
// 				return i;
// 			}
//                 }
// 		return npos;
	}

	_UCXXEXPORT size_type find_first_not_of(const Ch* s, size_type pos, size_type n) const{
		return find_first_not_of(basic_string<Ch, Tr, A>(s,n),pos);
	}
	_UCXXEXPORT size_type find_first_not_of(const Ch* s, size_type pos = 0) const{
		return find_first_not_of(basic_string<Ch, Tr, A>(s),pos);
	}

	_UCXXEXPORT size_type find_first_not_of(Ch c, size_type pos = 0) const{
	  bool found = false;
	  size_type v = pos;

	  for (size_type i = pos; i < length(); i++) {
	    found = (operator[](i) != c) || found;
	    v += !found;        // not equal, continue search
	  }
	  
	  if (v >= length()) 
	    return npos;
	  else
	    return v;

//              // Original implementation
// 		for(size_type i = pos; i < length() ; ++i){
// 			if(operator[](i) != c){
// 				return i;
// 			}
// 		}
// 		return npos;
	}

	_UCXXEXPORT size_type find_last_not_of (const basic_string& str, size_type pos = npos) const {
		size_type xpos(length());
		if (xpos > pos) {
		   xpos = pos;
		}
		
		bool found = false;
		size_type v = xpos - 1;
		
		for (size_type i = xpos; i != 0; --i) {
		  bool found_char = false;
		  for(size_type j = 0; j < str.length(); ++j) {
		    found_char = Tr::eq(str[j], operator[](i-1)) || found_char;
		  }
		  found = !found_char || found;
		  v -= !found;        // not found, continue search
		}
	  
		return v;         // return -1 when not found

//              // Original implementation
// 		while(xpos != npos && npos != str.find_first_of(at(xpos))){
// 			--xpos;
// 		}

// 		return xpos;
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

		bool found = false;
		int v = xpos;
		for (size_type i = xpos; i != 0 ; i--) {
		  found = (operator[](i-1) != c) || found;
		  v -= !found;        // not equal, continue search
		}

		return v;

//              // Original implementation
// 		while(xpos != npos && Tr::eq(at(xpos), c)){
// 			--xpos;
// 		}
// 		return xpos;
	}

	_UCXXEXPORT basic_string substr(size_type pos = 0, size_type n = npos) const;


  ///////////////////////////////////////////////////////////////////////////////////////////////
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////


	_UCXXEXPORT int compare(const basic_string& str) const {
	        // 	  return compare(str.c_str());
#ifdef DEBUG
	  printf("compare(string) ... \n");
#endif

	        size_type rlen = vector<Ch, A>::elements;
		if (rlen > str.elements)
		  rlen =  str.elements;
	  
		int v = 0;       // 1, 0 and -1 stand for gt, eq and lt respectively

		for (size_type i = 0; i < rlen; i++) {
 		  v += (!v) * ((operator[](i) > str[i]) - (operator[](i) < str[i]));
		  // the following removes the multiplication operator, however leads to no improvement in solving time
		  // v += (~(!v) + 1) & ((operator[](i) > str[i]) - (operator[](i) < str[i]));
		}

		v += (!v) * ((vector<Ch, A>::elements > str.elements) - (vector<Ch, A>::elements < str.elements));
		return v;
	}


	_UCXXEXPORT int compare(size_type pos1, size_type n1, const basic_string& str,
  		size_type pos2=0, size_type n2=npos) const {

#ifdef DEBUG
	  printf("compare(int, int, string, ,) ... \n");
#endif
		size_type len1 = vector<Ch, A>::elements - pos1;
		if (len1 > n1)
		  len1 = n1;

		size_type len2 = str.vector<Ch, A>::elements - pos2;
		if (len2 > n2)
		  len2 = n2;

		size_type rlen = len1;
		if (rlen > len2)
		  rlen = len2;

		int v = 0;       // 1, 0 and -1 stand for gt, eq and lt respectively

		for (size_type i = 0; i < rlen; i++) {
		  v += (!v) * ((operator[](pos1 + i) > str[pos2 + i]) - (operator[](pos1 + i) < str[pos2 + i]));
		  // v += (~(!v) + 1) & ((operator[](pos1 + i) > str[pos2 + i]) - (operator[](pos1 + i) < str[pos2 + i]));
		}

		v += (!v) * ((len1 > len2) - (len1 < len2));
		return v;
	}


	_UCXXEXPORT int compare(const Ch* s) const {
#ifdef DEBUG
	  printf("compare(char*) ... \n");
#endif
	        size_type rlen = vector<Ch, A>::elements;
		size_type slen = Tr::length(s);
		if (rlen > slen)
		  rlen = slen;
	  
		int v = 0;       // 1, 0 and -1 stand for gt, eq and lt respectively

		for (size_type i = 0; i < rlen; i++) {
		  v += (!v) * ((operator[](i) > s[i]) - (operator[](i) < s[i]));
		  // v += (~(!v) + 1) & ((operator[](i) > s[i]) - (operator[](i) < s[i]));
		}

		v += (!v) * ((vector<Ch, A>::elements > slen) - (vector<Ch, A>::elements < slen));
		// v +=  (~(!v) + 1) & ((vector<Ch, A>::elements > slen) - (vector<Ch, A>::elements < slen));
		return v;
	}


	_UCXXEXPORT int compare(size_type pos1, size_type n1, const Ch* s, size_type n2 = npos) const {
#ifdef DEBUG
	  printf("compare(int, int, char*, ) ... \n");
#endif
	        size_type len1 = vector<Ch, A>::elements - pos1;
		if(len1 > n1) {
			len1 = n1;
		}
		size_type slen = Tr::length(s);
		size_type len2 = slen;
		if(len2 > n2) {
			len2 = n2;
		}
		size_type rlen = len1;
		if(rlen > len2) {
			rlen = len2;
		}

		int v = 0;       // 1, 0 and -1 stand for gt, eq and lt respectively

		for (size_type i = 0; i < rlen; i++) {
		  v += (!v) * ((operator[](pos1 + i) > s[i]) - (operator[](pos1 + i) < s[i]));
		  // v += (~(!v) + 1) & ((operator[](pos1 + i) > s[i]) - (operator[](pos1 + i) < s[i]));
		}

		v += (!v) * ((len1 > len2) - (len1 < len2));
		// v +=  (~(!v) + 1) & ((len1 > len2) - (len1 < len2));

		return v;
	}

  ////////////// end class declaration
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////


  template<class Ch,class Tr,class A> _UCXXEXPORT basic_string<Ch,Tr,A>::basic_string(const Ch* s, const A& al)
    : vector<Ch, A>(al)
  {
    if(s!=0){
      size_type temp = Tr::length(s);
      append(s, temp);
    }
  }

  template<class Ch,class Tr,class A> _UCXXEXPORT basic_string<Ch,Tr,A>::
  basic_string(const basic_string& str, size_type pos, size_type n, const A& al) 
    : vector<Ch, A>(al)
  {
    if(pos>str.size()){
      __throw_out_of_range();
    }
    size_type rlen = str.size() - pos;
    if( rlen > n){
      rlen = n;
    }
    resize(rlen);
	Tr::copy(vector<Ch, A>::data, str.vector<Ch, A>::data + pos, vector<Ch, A>::elements);
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
#ifdef DEBUG
        printf("find(str,pos) ... \n");
#endif

	if(str.length() > length()){
	  printf("find(str,pos): %d : %d \n", str.length(), length());
		return npos;
	}

	printf("find(str,pos): 2 \n");
	size_type max_string_start = 1 + length() - str.length();

	bool found = false;
	size_type v = max_string_start;

	for(size_type i = max_string_start + 1; i != pos ; i--) {
	   found = (substr(i - 1, str.length()) == str) || found;
	   v -= !found;        // not equal, continue search
	}
	return v;
}

// template<class Ch,class Tr,class A> _UCXXEXPORT typename basic_string<Ch,Tr,A>::size_type
// 	basic_string<Ch,Tr,A>::find (const basic_string<Ch,Tr,A>& str, size_type pos) const
// {
// 	if(str.length() > length()){
// 		return npos;
// 	}
// 	size_type max_string_start = 1 + length() - str.length();
// 	for(size_type i = pos; i < max_string_start; ++i){
// 		if(str == substr(i, str.length())){
// 			return i;
// 		}
// 	}
// 	return npos;
// }


template<class Ch,class Tr,class A>
	_UCXXEXPORT basic_string<Ch, Tr, A> basic_string<Ch,Tr,A>::substr(size_type pos, size_type n) const
{
#ifdef DEBUG
        printf("substr ... \n");
#endif

	if (klee_is_symbolic(pos))  {
	  pos = klee_get_min_value(pos);
	  printf("pos = %d\n", pos);
	}

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
	return lhs.compare(rhs) == 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator==(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	return rhs.compare(lhs) == 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator==(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	return lhs.compare(rhs) == 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator!=(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	return lhs.compare(rhs) !=0;
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
	return lhs.compare(rhs) < 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator< (const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	return lhs.compare(rhs) < 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator< (const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	return temp.compare(rhs) < 0;
}


template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator> (const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	return lhs.compare(rhs) > 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator> (const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	return lhs.compare(rhs) > 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator> (const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	return temp.compare(rhs) > 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator<=(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	return lhs.compare(rhs) <=0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator<=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	return lhs.compare(temp) <=0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator<=(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	return temp.compare(rhs) <= 0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator>=(const basic_string<charT,traits,Allocator>& lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	return lhs.compare(rhs) >=0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator>=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs)
{
	basic_string<charT,traits,Allocator> temp(rhs);
	return lhs.compare(temp) >=0;
}

template<class charT, class traits, class Allocator> _UCXXEXPORT bool 
	operator>=(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs)
{
	basic_string<charT,traits,Allocator> temp(lhs);
	return temp.compare(rhs) >=0;
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
