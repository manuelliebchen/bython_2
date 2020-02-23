/*
 * TypeName.hpp
 *
 *  Created on: Aug 6, 2019
 *      Author: jemand
 */

#ifndef SRC_AST_ASTTYPENAME_HPP_
#define SRC_AST_ASTTYPENAME_HPP_

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <unordered_map>

#include "../ast/ASTBase.hpp"

namespace by::type {

struct TypeName {
	TypeName();
	TypeName(const std::shared_ptr<peg::Ast>& ast);
	TypeName(std::string, std::vector<TypeName>);
	TypeName(std::string);
	TypeName(TypeName const&);

	TypeName operator=(TypeName);

	TypeName deduct_type(TypeName) const;

	bool operator==(const TypeName&) const;
	operator bool() const;

	std::string name;
	std::vector<TypeName> subtypes;

	llvm::Type* get_llvm_type(llvm::LLVMContext&) const;

	friend std::ostream& operator<<(std::ostream&, const TypeName&);
	friend std::string to_string(by::type::TypeName const&);
};

using variable_map = std::unordered_map<std::string, TypeName>;

std::ostream& operator<<(std::ostream&, const TypeName&);
std::string to_string(TypeName const&);
} // namespace by::type

namespace std {
template <>
struct hash<by::type::TypeName> {
	typedef by::type::TypeName argument_type;
	typedef std::size_t result_type;
	result_type operator()(argument_type const&) const noexcept;
};
} // namespace std

#endif /* SRC_AST_TypeName_HPP_ */
