#include "Variable.h"


void Variable::setType(KIND t) {
  type = t;
}

void Variable::setValue(VALUE v) {
  value = v;
}

void Variable::setLocal(bool l) {
  local = l;
}

void Variable::setOrigSize(unsigned int s) {
  origSize = s;
}

void Variable::setCurrSize(unsigned int s) {
  currSize = s;
}

void Variable::setOffset(int o) {
  offset = o;
}

KIND Variable::getType() {
  return type;
}

VALUE Variable::getValue() {
  return value;
}

bool Variable::getLocal() {
  return local;
}

unsigned int Variable::getOrigSize() {
  return origSize;
}

unsigned int Variable::getCurrSize() {
  return currSize;
}

int Variable::getOffset() {
  return offset;
}

string Variable::toString() {
  std::stringstream s;

  if (local) {
    s << "LOCAL    => ";
  }
  else {
    s << "REGISTER => ";
  }

  switch(type) {
  case PTR_KIND:
    s << "[PTR: " << value.as_ptr << "]";
    break;
  case ARRAY_KIND:
    s << "[ARRAY: " << value.as_ptr << "]";
    break;
  case STRUCT_KIND:
    s << "[STRUCT: " << value.as_ptr << "]";
    break;
  case INT1_KIND:
    s << "[INT1: " << value.as_int << "]";
    break;
  case INT8_KIND:
    s << "[INT8: " << value.as_int << "]";
    break;
  case INT16_KIND:
    s << "[INT16: " << value.as_int << "]";
    break;
  case INT32_KIND:
    s << "[INT32: " << value.as_int << "]";
    break;
  case INT64_KIND:
    s << "[INT64: " << value.as_int << "]";
    break;
  case FLP32_KIND:
    // TODO: this assumes value.as_flp returns a long double value
    s << "[FLP32: " << (float)value.as_flp << "]";
    break;
  case FLP64_KIND:
    // TODO: this assumes value.as_flp returns a long double value
    s << "[FLP64: " << (double) value.as_flp << "]";
    break;
  case FLP80X86_KIND:
    // TODO: this is incorrect because value.as_flp returns a double
    s << "[FLP80X86: " << (long double) value.as_flp << "]";
    break;
  default: 
    cout << "Here?\n";
    safe_assert(false);
    break;
  }

  s << ", Current Size: " << currSize;
  if (currSize != origSize) {
    s << ", Original Size: " << origSize;
  }
  s << ", Offset: " << offset; 

  return s.str();
}


void Variable::copy(Variable *dest) {
  dest->setType(type);
  dest->setValue(value);
  dest->setOrigSize(origSize);
  dest->setCurrSize(currSize);
  dest->setOffset(offset);
  dest->setLocal(local);
}

bool Variable::isEqualPtrSize() {
  return origSize == currSize;
}

bool Variable::isSmallerPtrSize() {
  return currSize < origSize;
}

