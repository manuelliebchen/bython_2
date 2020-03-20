/*
 * function_build.h
 *
 *  Created on: Mar 18, 2020
 *      Author: Manuel Liebchen
 */

#ifndef SRC_BC_FUNCTION_BUILD_H_
#define SRC_BC_FUNCTION_BUILD_H_

#include <memory>
#include <functional>
#include <llvm/IR/Value.h>

#include <type/function_type.h>

#include "build_context.h"

namespace by::bc {
	class BuildContext;

using build_functional =std::function<llvm::Value*( std::unique_ptr<BuildContext>&, std::vector<llvm::Value*>)>;

class FunctionBuilder {
protected:
	std::string name;
	type::FunctionType_ptr type;
	build_functional functional;

public:
	FunctionBuilder(std::string, type::FunctionType_ptr, build_functional);
	FunctionBuilder(std::string, const type::FunctionType&, build_functional);

	std::string get_name() const {return name;};
	type::FunctionType_ptr get_type() const {
		  return type;};

	llvm::Value *build_ir(std::unique_ptr<BuildContext> &,  std::vector<llvm::Value*>) const;
};
}


#endif /* SRC_BC_FUNCTION_BUILD_H_ */
