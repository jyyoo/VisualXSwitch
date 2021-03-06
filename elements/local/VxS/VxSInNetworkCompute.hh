#ifndef __VXS_INNETWORK_COMPUTE___
#define __VXS_INNETWORK_COMPUTE___

#include <click/config.h>
#include <click/element.hh>
#include "VxSInNetworkBatchManager.hh"

#define VSX_MAX_COMPUTE_NAME 64

CLICK_DECLS


/** 
 * Desc: A base class for in-network processing
 */
class VxSInNetworkCompute {
public:

	VxSInNetworkCompute(const char *name);
	~VxSInNetworkCompute();

	int isThisCompute(const char *name);
	char * getName() { return _compute_name; };

	virtual int compute(VxSInNetworkSegment *segment) = 0;

private:
	char _compute_name[VSX_MAX_COMPUTE_NAME];
};

CLICK_ENDDECLS
#endif
