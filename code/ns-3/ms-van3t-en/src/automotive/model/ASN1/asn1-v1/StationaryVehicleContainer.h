/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "DENM-PDU-Descriptions"
 * 	found in "ASN1Files/ETSI DENM v1.2.2.asn"
 * 	`asn1c -fincludes-quoted`
 */

#ifndef	_StationaryVehicleContainer_H_
#define	_StationaryVehicleContainer_H_


#include "asn_application.h"

/* Including external dependencies */
#include "StationarySince.h"
#include "NumberOfOccupants.h"
#include "EnergyStorageType.h"
#include "constr_SEQUENCE.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CauseCode;
struct DangerousGoodsExtended;
struct VehicleIdentification;

/* StationaryVehicleContainer */
typedef struct StationaryVehicleContainer {
	StationarySince_t	*stationarySince	/* OPTIONAL */;
	struct CauseCode	*stationaryCause	/* OPTIONAL */;
	struct DangerousGoodsExtended	*carryingDangerousGoods	/* OPTIONAL */;
	NumberOfOccupants_t	*numberOfOccupants	/* OPTIONAL */;
	struct VehicleIdentification	*vehicleIdentification	/* OPTIONAL */;
	EnergyStorageType_t	*energyStorageType	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} StationaryVehicleContainer_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_StationaryVehicleContainer;
extern asn_SEQUENCE_specifics_t asn_SPC_StationaryVehicleContainer_specs_1;
extern asn_TYPE_member_t asn_MBR_StationaryVehicleContainer_1[6];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CauseCode.h"
#include "DangerousGoodsExtended.h"
#include "VehicleIdentification.h"

#endif	/* _StationaryVehicleContainer_H_ */
#include "asn_internal.h"
