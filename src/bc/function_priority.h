/*
 * BuildContext.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Manuel Liebchen
 */

#ifndef SRC_FUNCTION_PRIORITY_H
#define SRC_FUNCTION_PRIORITY_H

namespace by::bc {

enum FunctionPriority {
  VARIABLE = 0,
  CAST = 1,
  OPERATOR_DOT = 2,
  OPERATOR_LINE = 3,
  OPERATOR_COMPARE = 4,
  OPERATOR_LIST = 5,
  CALL = 6
};

}

#endif // SRC_FUNCTION_PRIORITY_H
