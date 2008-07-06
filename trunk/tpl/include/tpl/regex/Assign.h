/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: tpl/regex/Assign.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-13 9:41:58
// 
// $Id$
// -----------------------------------------------------------------------*/
#ifndef TPL_REGEX_ASSIGN_H
#define TPL_REGEX_ASSIGN_H

#ifndef TPL_REGEX_ACTION_H
#include "Action.h"
#endif

#ifndef TPL_REGEX_REF_H
#include "Ref.h"
#endif

NS_TPL_BEGIN

// =========================================================================
// class AssigStr

// Usage: Rule/assign(a_string_var)
// Usage: Rule/assign(a_leaf_node_var)

class AssigStr
{
public:
	template <class StringT, class Iterator>
	static StringT TPL_CALL get(Iterator pos, Iterator pos2, const void* = NULL) {
		return StringT(pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssigCh

// Usage: Rule/assign(a_char_var)

class AssigCh
{
public:
	template <class CharT, class Iterator>
	static CharT TPL_CALL get(Iterator pos, Iterator pos2, const void* = NULL) {
		TPL_ASSERT(std::distance(pos, pos2) == 1);
		return *pos;
	}
};

// -------------------------------------------------------------------------
// class AssigHexInt

// Usage: Rule/assign(u_integer_var)

class AssigHexInt
{
public:
	template <class UIntT, class Iterator>
	static UIntT TPL_CALL get(Iterator pos, Iterator pos2, const void* = NULL) {
		return std::CastToUInt<UIntT>::get(pos, pos2, 16);
	}
};

// -------------------------------------------------------------------------
// class AssigUInt/AssigOctInt

// Usage: Rule/assign(a_uint_var)

template <int radix = 10>
class AssigUI
{
public:
	template <class UIntT, class Iterator>
	static UIntT TPL_CALL get(Iterator first, Iterator last, const void* = NULL)
	{
		UIntT val = 0;
		for (; first != last; ++first) {
			TPL_ASSERT(*first >= '0' && *first <= radix+'0');
			val = val * radix + (*first - '0');
		}
		return val;
	}
};

typedef AssigUI<10> AssigUInt;
typedef AssigUI<8> AssigOctInt;
typedef AssigUI<2> AssigBinInt;

// -------------------------------------------------------------------------
// class AssigInt

// Usage: Rule/assign(a_int_var)

class AssigInt
{
public:
	template <class IntT, class Iterator>
	static IntT TPL_CALL get(Iterator first, Iterator last, const void* = NULL)
	{
		if (*first == '-') {
			return -AssigUInt::get<IntT>(first+1, last);
		}
		else if (*first == '+') {
			++first;
		}
		return AssigUInt::get<IntT>(first, last);
	}
};

// -------------------------------------------------------------------------
// class AssigReal

// Usage: Rule/assign(a_real_var)

class AssigReal
{
public:
	template <class RealT, class Iterator>
	static RealT TPL_CALL get(Iterator pos, Iterator pos2, const void* = NULL) {
		return std::CastToReal<RealT>::get(pos, pos2);
	}
};

// -------------------------------------------------------------------------
// class AssigLst

class AssigLst
{
public:
	template <class IntT, class Iterator, class LstOpT>
	static IntT TPL_CALL get(Iterator pos, Iterator pos2, const LstOpT* lst) {
		return lst->size();
	}
};

// -------------------------------------------------------------------------
// SelectAssig

template <class AssigTag, class CharT, class Tr, class Ax>
struct SelectAssig<AssigTag, std::basic_string<CharT, Tr, Ax> > {
	typedef AssigStr assig_type;
};

template <class AssigTag, class Iterator>
struct SelectAssig<AssigTag, std::Range<Iterator> > {
	typedef AssigStr assig_type;
};

#define TPL_TEXT_ASSIG_(AssigTag, AssigType)	\
												\
template <class CharT, class Tr, class Ax>		\
struct SelectAssig<AssigTag, std::basic_string<CharT, Tr, Ax> > {	\
	typedef AssigType assig_type;									\
};

#define TPL_TEXT_ASSIG(AssigTag, AssigType)		\
	namespace tpl {								\
		TPL_TEXT_ASSIG_(AssigTag, AssigType)	\
	}

NS_TPL_END

// =========================================================================

TPL_ASSIG(TagAssigNone, AssigStr)
TPL_ASSIG(TagAssigLst, AssigLst)
TPL_ASSIG(TagAssigChar, AssigCh)
TPL_ASSIG(TagAssigUInteger, AssigUInt)
TPL_ASSIG(TagAssigHexInteger, AssigHexInt)
TPL_ASSIG(TagAssigOctInteger, AssigOctInt)
TPL_ASSIG(TagAssigBinInteger, AssigBinInt)
TPL_ASSIG(TagAssigInteger, AssigInt)
TPL_ASSIG(TagAssigUReal, AssigReal)
TPL_ASSIG(TagAssigReal, AssigReal)
TPL_ASSIG(TagAssigUFraction, AssigReal)
TPL_ASSIG(TagAssigFraction, AssigReal)

// =========================================================================
// class Assign

NS_TPL_BEGIN

template <class ValueT>
class Assign
{
private:
	ValueT& m_val;

public:
	Assign(ValueT& val) : m_val(val) {}

	typedef ValueT value_type;

	void TPL_CALL operator()(const value_type& val) const {
		m_val = val;
	}
};

// Usage: Rule/assign(var)

template <class ValueT> __forceinline
Action<Assign<ValueT> > TPL_CALL assign(ValueT& result) {
	return Action<Assign<ValueT> >(result);
}

template <class ValueT> __forceinline
Action<Assign<ValueT> > TPL_CALL assign(Var<ValueT>& result) {
	return Action<Assign<ValueT> >(result.val);
}

NS_TPL_END

// =========================================================================

#define TPL_SIMPLE_ACTION_UNARY_OP_CLASS_(Op, op)				\
template <class ValueT>											\
class Op														\
{																\
private:														\
	ValueT& m_val;												\
																\
public:															\
	Op(ValueT& val) : m_val(val) {}								\
																\
	void TPL_CALL operator()() const {							\
		op m_val;												\
	}															\
};

#define TPL_ACTION_OP_FN1_EX_(Action, Op, fn) 					\
																\
template <class ValueT> __forceinline							\
Action<Op<ValueT> > TPL_CALL fn(ValueT& result) {				\
	return Action<Op<ValueT> >(result);							\
}																\
																\
template <class ValueT> __forceinline							\
Action<Op<ValueT> > TPL_CALL fn(tpl::Var<ValueT>& result) {		\
	return Action<Op<ValueT> >(result.val);						\
}

#define TPL_SIMPLE_ACTION_UNARY_OP_(Op, fn, op) 				\
	TPL_SIMPLE_ACTION_UNARY_OP_CLASS_(Op, op)					\
	TPL_ACTION_OP_FN1_EX_(tpl::SimpleAction, Op, fn)

// =========================================================================

NS_TPL_BEGIN

// Usage: Rule/inc(var), Rule/dec(var)

TPL_SIMPLE_ACTION_UNARY_OP_(Inc_, inc, ++)
TPL_SIMPLE_ACTION_UNARY_OP_(Dec_, dec, --)

NS_TPL_END

// =========================================================================

#define TPL_SIMPLE_ACTION_BINARY_OP_CLASS_(Op, op)							\
template <class VarT, class ValueT>											\
class Op																	\
{																			\
private:																	\
	VarT& m_var;															\
	const ValueT m_value;													\
																			\
public:																		\
	Op(VarT& var_, const ValueT value_)										\
		: m_var(var_), m_value(value_) {}									\
																			\
	void TPL_CALL operator()() const {										\
		m_var op m_value;													\
	}																		\
};

#define TPL_SIMPLE_ACTION_METHOD0_CLASS_(Op, op)							\
template <class VarT>														\
class Op																	\
{																			\
private:																	\
	VarT& m_var;															\
																			\
public:																		\
	Op(VarT& var_)															\
		: m_var(var_) {}													\
																			\
	void TPL_CALL operator()() const {										\
		m_var.op();															\
	}																		\
};

#define TPL_SIMPLE_ACTION_METHOD1_CLASS_(Op, op)							\
template <class VarT, class ValueT>											\
class Op																	\
{																			\
private:																	\
	VarT& m_var;															\
	const ValueT m_value;													\
																			\
public:																		\
	Op(VarT& var_, const ValueT value_)										\
		: m_var(var_), m_value(value_) {}									\
																			\
	void TPL_CALL operator()() const {										\
		m_var.op(m_value);													\
	}																		\
};

#define TPL_ACTION_OP_FN2_EX_(Action, Op, op) 									\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, ValueT> > TPL_CALL op(VarT& var_, const ValueT& value_) {		\
	return Action<Op<VarT, ValueT> >(var_, value_);								\
} 																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, const ValueT&> > TPL_CALL op(VarT& var_, ValueT& value_) {		\
	return Action<Op<VarT, const ValueT&> >(var_, value_);						\
}																				\
																				\
template <class CharT, class VarT> __forceinline								\
Action<Op<VarT, const CharT*> > TPL_CALL op(VarT& var_, const CharT value_[]) { \
	return Action<Op<VarT, const CharT*> >(var_, value_);						\
} 																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, const ValueT&> > TPL_CALL op(VarT& var_, const tpl::Var<ValueT>& value_) { \
	return Action<Op<VarT, const ValueT&> >(var_, value_.val);					\
}																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, const ValueT&> > TPL_CALL op(VarT& var_, tpl::Var<ValueT>& value_) { \
	return Action<Op<VarT, const ValueT&> >(var_, value_.val);					\
}																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, ValueT> > TPL_CALL op(tpl::Var<VarT>& var_, const ValueT& value_) { \
	return Action<Op<VarT, ValueT> >(var_.val, value_);							\
}																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, const ValueT&> > TPL_CALL op(tpl::Var<VarT>& var_, ValueT& value_) { \
	return Action<Op<VarT, const ValueT&> >(var_.val, value_);					\
}																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, const ValueT&> > TPL_CALL op(tpl::Var<VarT>& var_, const tpl::Var<ValueT>& value_) { \
	return Action<Op<VarT, const ValueT&> >(var_.val, value_.val);				\
}																				\
																				\
template <class ValueT, class VarT> __forceinline								\
Action<Op<VarT, const ValueT&> > TPL_CALL op(tpl::Var<VarT>& var_, tpl::Var<ValueT>& value_) { \
	return Action<Op<VarT, const ValueT&> >(var_.val, value_.val);				\
}

#define TPL_SIMPLE_ACTION_BINARY_OP_(Op, fn, op)								\
	TPL_SIMPLE_ACTION_BINARY_OP_CLASS_(Op, op)									\
	TPL_ACTION_OP_FN2_EX_(tpl::SimpleAction, Op, fn)
	
#define TPL_SIMPLE_ACTION_METHOD0_(Op, fn)										\
	TPL_SIMPLE_ACTION_METHOD0_CLASS_(Op, fn)									\
	TPL_ACTION_OP_FN1_EX_(tpl::SimpleAction, Op, fn)

#define TPL_SIMPLE_ACTION_METHOD1_(Op, fn)										\
	TPL_SIMPLE_ACTION_METHOD1_CLASS_(Op, fn)									\
	TPL_ACTION_OP_FN2_EX_(tpl::SimpleAction, Op, fn)

// =========================================================================

NS_TPL_BEGIN

// Usage: Rule/assign(var, value)
// Usage: Rule/inc(var, value), Rule/dec(var, value)

TPL_SIMPLE_ACTION_BINARY_OP_(AssignVal, assign, =)
TPL_SIMPLE_ACTION_BINARY_OP_(IncVal, inc, +=)
TPL_SIMPLE_ACTION_BINARY_OP_(DecVal, dec, -=)

// Usage: Rule/push(cont, value)
// Usage: Rule/push_back(cont, value)
// Usage: Rule/push_front(cont, value)
// Usage: Rule/append(cont, value)
// Usage: Rule/insert(cont, value)
// Usage: Rule/clear(cont)

TPL_SIMPLE_ACTION_METHOD1_(PushVal, push)
TPL_SIMPLE_ACTION_METHOD1_(PushBackVal, push_back)
TPL_SIMPLE_ACTION_METHOD1_(PushFrontVal, push_front)
TPL_SIMPLE_ACTION_METHOD1_(AppendVal, append)
TPL_SIMPLE_ACTION_METHOD1_(InsertVal, insert)
TPL_SIMPLE_ACTION_METHOD0_(ClearVal, clear)

NS_TPL_END

// =========================================================================
// $Log: $

#endif /* TPL_REGEX_ASSIGN_H */
