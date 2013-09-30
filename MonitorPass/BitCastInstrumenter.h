#ifndef BITCAST_INSTRUMENTER_H_
#define BITCAST_INSTRUMENTER_H_

#include "Common.h"
#include "Instrumenter.h"

class BitCastInstrumenter : public Instrumenter {
  public:
    BitCastInstrumenter(std::string name, Instrumentation* instrumentation) :
      Instrumenter(name, instrumentation) {};

    virtual bool CheckAndInstrument(Instruction* inst);
};

#endif