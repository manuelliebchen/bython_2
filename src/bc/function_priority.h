/*
 * BuildContext.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Manuel Liebchen
 */

#ifndef SRC_FUNCTION_PRIORITY_H
#define SRC_FUNCTION_PRIORITY_H

namespace by::bc {

enum FunctionType {
  VARIABLE = 0,
  CAST = 1,
  UNARY = 2,
  OPERATOR_DOT = 3,
  OPERATOR_LINE = 4,
  OPERATOR_COMPARE = 5,
  OPERATOR_LIST = 6,
  CALL = 7
};

}

#endif // SRC_FUNCTION_PRIORITY_H
