// Author: Cuong Nguyen

#ifndef BLAME_ANALYSIS_H_
#define BLAME_ANALYSIS_H_

#include <set>
#include <cmath>
#include <queue>
#include <unordered_map>

#include "../../src/IValue.h"
#include "../../src/InterpreterObserver.h"

#include "BlameNode.h"
#include "BlameShadowObject.h"

class BlameAnalysis : public InterpreterObserver {

private:
	// Return the file separator character depending on the underlying operating
	// system.
	inline char separator() {
#ifdef _WIN32
		return '\\';
#else
		return '/';
#endif
	};

	// Read debug information from $GLOG_log_dir/debug.bin and wrap into a
	// mapping from instruction IID to DebugInfo struct. The file debug.bin is
	// constructed during instrumentation phase.
	//
	// TODO: find a way to make the name debug.bin to be provided as input and
	// sounds more personal to the application under analysis.
	std::unordered_map<IID, DebugInfo> readDebugInfo();

	// A map from instruction IID to debug information. IID is computed during
	// instrumentation phase and is the unique id for each LLVM instruction.
	// Debug information includes the LoC, column and file of the instruction.
	const std::unordered_map<IID, DebugInfo> debugInfoMap = readDebugInfo();

	// A map from instruction IID to blame summary. IID is computed during
	// instrumentation phase and is the unique id for each LLVM instruction.
	// Blame summary is a tree-like data structure which represents the precision
	// dependency of values used and defined in this instruction.
	std::map<IID, std::vector<BlameNode*>> blameSummary;

	// Global information about the starting point of the analysis.
	PRECISION _precision;
	IID _iid;

public:
	BlameAnalysis(std::string name) : InterpreterObserver(name) {
		_precision = BITS_27;
		_iid = 0;
	}

	/*** API FUNCTIONS ***/
	virtual void pre_analysis();

	virtual void post_fadd(IID iid, IID liid, IID riid, SCOPE lScope, SCOPE rScope, int64_t lValue, int64_t rValue,
						   KIND type, int inx);

	virtual void post_fsub(IID iid, IID liid, IID riid, SCOPE lScope, SCOPE rScope, int64_t lValue, int64_t rValue,
						   KIND type, int inx);

	virtual void post_fmul(IID iid, IID liid, IID riid, SCOPE lScope, SCOPE rScope, int64_t lValue, int64_t rValue,
						   KIND type, int inx);

	virtual void post_fdiv(IID iid, IID liid, IID riid, SCOPE lScope, SCOPE rScope, int64_t lValue, int64_t rValue,
						   KIND type, int inx);

	virtual void post_analysis();

private:
	// Get the shadow object of an LLVM instruction. An LLVM instruction is
	// identified by its iid, scope and value or index.
	const BlameShadowObject getShadowObject(IID iid, SCOPE scope, int64_t value);

	void computeBlameSummary(const BlameShadowObject& bso, const BlameShadowObject& lbso, const BlameShadowObject& rbso,
							 BINOP op);

	const BlameNode computeBlameInformation(const BlameShadowObject& bso, const BlameShadowObject& lbso,
											const BlameShadowObject& rbso, BINOP op, PRECISION p);

	// Determine whether the result can blame lop and rop given the precision p
	// of the computation.
	bool canBlame(HIGHPRECISION result, HIGHPRECISION lop, HIGHPRECISION rop, BINOP op, PRECISION p);

	// Determine whether the compution requires high precision operator.
	bool isRequiredHigherPrecisionOperator(HIGHPRECISION result, HIGHPRECISION lop, HIGHPRECISION rop, BINOP op,
										   PRECISION p);

	void mergeBlame(BlameNode* summary, const BlameNode& blame);

	void initSummaryIfNotExist(IID id);

	static void* copyShadow(void* oldShadow);

	void post_fbinop(IID iid, IID liid, IID riid, SCOPE lScope, SCOPE rScope, int64_t lValue, int64_t rValue, KIND type,
					 int inx, BINOP op);
};

#endif
