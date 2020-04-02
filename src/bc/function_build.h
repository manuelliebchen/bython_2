/*
 * function_build.h
 *
 *  Created on: Mar 18, 2020
 *      Author: Manuel Liebchen
 */

#ifndef SRC_BC_FUNCTION_BUILD_H_
#define SRC_BC_FUNCTION_BUILD_H_

#include <llvm/IR/Value.h>
#include <type/function_type.h>
#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <iosfwd>

#include "function_priority.h"

namespace llvm {
class Value;
}  // namespace llvm

namespace by::bc {
	class BuildContext;

	using build_functional =std::function<llvm::Value*( std::unique_ptr<BuildContext>&, std::vector<llvm::Value*>)>;

class FunctionBuilder {
protected:
	std::string name;
	FunctionPriority priority;
	type::FunctionType_ptr type;
	build_functional functional;

public:

	FunctionBuilder(std::string, FunctionPriority, type::FunctionType_ptr, build_functional);

	std::string get_name() const {return name;};
	FunctionPriority get_priority() const { return priority;};
	type::FunctionType_ptr get_type() const {
		  return type;};

	llvm::Value *build_ir(std::unique_ptr<BuildContext> &,  std::vector<llvm::Value*>) const;
  friend std::ostream &operator<<(std::ostream &, const FunctionBuilder &);
};

std::ostream &operator<<(std::ostream &, const FunctionBuilder &);
}


#endif /* SRC_BC_FUNCTION_BUILD_H_ */
