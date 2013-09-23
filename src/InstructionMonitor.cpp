#include "InstructionMonitor.h"
#include "InstructionObserver.h"
#include "InterpreterObserver.h"

/*******************************************************************************************/
#define DISPATCH_TO_OBSERVERS(func, ...) \
	for(ObserverPtrList::iterator itr = observers_.begin(); itr < observers_.end(); ++itr) { \
		(*itr)->func(__VA_ARGS__); \
	}
/*******************************************************************************************/

// ***** Binary Operations ***** //
void llvm_add(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(add, iid, nuw, nsw, op1, op2, inx)
}

void llvm_fadd(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(fadd, iid, nuw, nsw, op1, op2, inx)
}

void llvm_sub(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(sub, iid, nuw, nsw, op1, op2, inx)
}

void llvm_fsub(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(fsub, iid, nuw, nsw, op1, op2, inx)
}

void llvm_mul(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(mul, iid, nuw, nsw, op1, op2, inx)
}

void llvm_fmul(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx){
	DISPATCH_TO_OBSERVERS(fmul, iid, nuw, nsw, op1, op2, inx)
}

void llvm_udiv(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(udiv, iid, nuw, nsw, op1, op2, inx)
}

void llvm_sdiv(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(sdiv, iid, nuw, nsw, op1, op2, inx)
}

void llvm_fdiv(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(fdiv, iid, nuw, nsw, op1, op2, inx)
}

void llvm_urem(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(urem, iid, nuw, nsw, op1, op2, inx)
}

void llvm_srem(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(srem, iid, nuw, nsw, op1, op2, inx)
}

void llvm_frem(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(frem, iid, nuw, nsw, op1, op2, inx)
}

// ***** Bitwise Binary Operations ***** //
void llvm_shl(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(shl, iid, nuw, nsw, op1, op2, inx)
}

void llvm_lshr(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(lshr, iid, nuw, nsw, op1, op2, inx)
}

void llvm_ashr(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(ashr, iid, nuw, nsw, op1, op2, inx)
}

void llvm_and_(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(and_, iid, nuw, nsw, op1, op2, inx)
}

void llvm_or_(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(or_, iid, nuw, nsw, op1, op2, inx)
}

void llvm_xor_(IID iid, bool nuw, bool nsw, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(xor_, iid, nuw, nsw, op1, op2, inx)
}

// ****** Vector Operations ****** //
void llvm_extractelement(IID iid, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(extractelement, iid, op1, op2, inx)
}

void llvm_insertelement() {
	DISPATCH_TO_OBSERVERS(insertelement)
}

void llvm_shufflevector() {
	DISPATCH_TO_OBSERVERS(shufflevector)
}

// ***** Aggregate Operations ***** //
void llvm_extractvalue(IID iid, KVALUE* op, int inx) {
	DISPATCH_TO_OBSERVERS(extractvalue, iid, op, inx)
}

void llvm_insertvalue(IID iid, KVALUE* op1, KVALUE* op2, int inx) {
	DISPATCH_TO_OBSERVERS(insertvalue, iid, op1, op2, inx)
}


// ***** Memory Access and Addressing Operations ***** //

void llvm_allocax(IID iid, KIND kind, int inx) {
  DISPATCH_TO_OBSERVERS(allocax, iid, kind, inx)
}

void llvm_load(IID iid, KVALUE* op, KVALUE* value, int inx) {
  DISPATCH_TO_OBSERVERS(load, iid, op, value, inx);
}


void llvm_store(IID iid, KVALUE* op, KVALUE* value, int inx) {
  DISPATCH_TO_OBSERVERS(store, iid, op, value, inx)
}

void llvm_fence() {
	DISPATCH_TO_OBSERVERS(fence)
}

void llvm_cmpxchg(IID iid, PTR addr, KVALUE* value1, KVALUE* value2, int inx) {
  DISPATCH_TO_OBSERVERS(cmpxchg, iid, addr, value1, value2, inx)
}

void llvm_atomicrmw() {
	DISPATCH_TO_OBSERVERS(atomicrmw)
}

void llvm_getelementptr(IID iid, bool isbound, KVALUE* value, int inx) {
	DISPATCH_TO_OBSERVERS(getelementptr, iid, isbound, value, inx)
}


// ***** Conversion Operations ***** //
void llvm_trunc(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(trunc, iid, type, op, inx);
}

void llvm_zext(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(zext, iid, type, op, inx);
}

void llvm_sext(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(sext, iid, type, op, inx);
}

void llvm_fptrunc(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(fptrunc, iid, type, op, inx);
}

void llvm_fpext(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(fpext, iid, type, op, inx);
}

void llvm_fptoui(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(fptoui, iid, type, op, inx);
}

void llvm_fptosi(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(fptosi, iid, type, op, inx);
}

void llvm_uitofp(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(uitofp, iid, type, op, inx);
}

void llvm_sitofp(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(sitofp, iid, type, op, inx);
}

void llvm_ptrtoint(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(ptrtoint, iid, type, op, inx);
}

void llvm_inttoptr(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(inttoptr, iid, type, op, inx);
}

void llvm_bitcast(IID iid, KIND type, KVALUE* op, int inx) {
  DISPATCH_TO_OBSERVERS(bitcast, iid, type, op, inx);
}


// **** Terminator Instructions ***** //
void llvm_branch(IID iid, bool conditional, KVALUE* op1, int inx) {
	DISPATCH_TO_OBSERVERS(branch, iid, conditional, op1, inx)
}

void llvm_branch2(IID iid, bool conditional, int inx) {
	DISPATCH_TO_OBSERVERS(branch2, iid, conditional, inx)
}

void llvm_indirectbr(IID iid, KVALUE* op1, int inx) {
	DISPATCH_TO_OBSERVERS(indirectbr, iid, op1, inx)
}

void llvm_invoke(IID iid, KVALUE* op, int inx) {
	DISPATCH_TO_OBSERVERS(invoke, iid, op, inx)
}

void llvm_resume(IID iid, KVALUE* op1, int inx) {
	DISPATCH_TO_OBSERVERS(resume, iid, op1, inx)
}

void llvm_return_(IID iid, KVALUE* op1, int inx) {
	DISPATCH_TO_OBSERVERS(return_, iid, op1, inx)
}

void llvm_return2_(IID iid, int inx) {
	DISPATCH_TO_OBSERVERS(return2_, iid, inx)
}

void llvm_switch_(IID iid, KVALUE* op, int inx) {
	DISPATCH_TO_OBSERVERS(switch_, iid, op, inx)
}

void llvm_unreachable() {
	DISPATCH_TO_OBSERVERS(unreachable)
}


// ***** Other Operations ***** //
void llvm_icmp(IID iid, KVALUE* op1, KVALUE* op2, PRED pred, int inx) {
  DISPATCH_TO_OBSERVERS(icmp, iid, op1, op2, pred, inx)
}

void llvm_fcmp(IID iid, KVALUE* op1, KVALUE* op2, PRED pred, int inx) {
  DISPATCH_TO_OBSERVERS(fcmp, iid, op1, op2, pred, inx)
}

void llvm_phinode() {
	DISPATCH_TO_OBSERVERS(phinode)
}

void llvm_select(IID iid, KVALUE* cond, KVALUE* tvalue, KVALUE* fvalue, int inx) {
  DISPATCH_TO_OBSERVERS(select, iid, cond, tvalue, fvalue, inx)
}

void llvm_push_stack(KVALUE* value) {
	DISPATCH_TO_OBSERVERS(push_stack, value)
}

void llvm_create_stack_frame(int size) {
	DISPATCH_TO_OBSERVERS(create_stack_frame, size)
}

void llvm_call(IID iid, KIND type, KVALUE* value, int inx) {
	DISPATCH_TO_OBSERVERS(call, iid, type, value, inx)
}

void llvm_vaarg() {
	DISPATCH_TO_OBSERVERS(vaarg)
}

void llvm_landingpad() {
	DISPATCH_TO_OBSERVERS(landingpad)
}


/*******************************************************************************************/

ObserverPtrList observers_;

/*******************************************************************************************/

#include "PrintObserver.h"

// macro for adding observers
#define REGISTER_OBSERVER(T, N) \
		static RegisterObserver<T> T##_INSTANCE(N);

// active observers
REGISTER_OBSERVER(PrintObserver, "print")
// REGISTER_OBSERVER(InterpreterObserver, "interpreter")

/*******************************************************************************************/


